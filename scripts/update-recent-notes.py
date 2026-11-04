#!/usr/bin/env python3
"""
update-recent-notes.py — 自动更新 _posts/index.md 中的「🔗 最近更新」列表

参考 quartz-community/recent-notes 插件逻辑，但做了关键优化：
  - 按 commit 分组，每个 commit 取 1 个代表性文件（避免批量提交淹没列表）
  - 跳过纯格式/样式提交（commit message 含 style/format 等）
  - 跳过批量提交（单次 > 20 个文件的）
  - 从 frontmatter / H1 / 文件名提取标题
  - 生成与现有风格一致的 Markdown 列表

用法:
  python3 update-recent-notes.py [--limit 7] [--posts-dir _posts] [--index _posts/index.md]
  python3 update-recent-notes.py --dry-run    # 预览不写入
"""

import argparse
import re
import subprocess
import sys
from pathlib import Path
from datetime import datetime


# ── 分类图标映射 ──────────────────────────────────────────────
CATEGORY_ICONS = [
    (r"^A_OS/(Linux|Windows)", "⚙️"),
    (r"^A_OS/(MySQL8|Docker)", "🐳"),
    (r"^wiki/安全", "🛡️"),
    (r"^wiki/弱密码", "🔑"),
    (r"^wiki/PE", "📄"),
    (r"^wiki/Windows驱动", "⚙️"),
    (r"^wiki/AI大模型/样本分析", "🦠"),
    (r"^wiki/AI大模型", "🤖"),
    (r"^wiki/调试技术", "🔍"),
    (r"^AI大模型", "🧠"),
    (r"^B_Code", "📂"),
    (r"^E_手册/漏洞", "🛡️"),
    (r"^E_手册", "📘"),
    (r"^G_工具", "🛠️"),
    (r"^A_内功", "🏗️"),
    (r"^D_立志博览群书", "📖"),
    (r"^Z_杂记", "📓"),
    (r"^Z_研", "🔬"),
    (r"^wiki", "📝"),
    (r".*", "📝"),
]

# 跳过这些 commit message 关键词（纯格式/样式提交）
SKIP_COMMIT_KEYWORDS = ["style", "format", "lint", "tidy", "rename", "chore"]


def get_category_icon(rel_path: str) -> str:
    for pattern, icon in CATEGORY_ICONS:
        if re.match(pattern, rel_path):
            return icon
    return "📝"


def extract_title(file_path: Path) -> str:
    """从 Markdown 文件的 frontmatter 或第一个 H1 中提取标题。"""
    try:
        with open(file_path, "r", encoding="utf-8-sig") as f:
            content = f.read(4096)

        # frontmatter title
        fm_match = re.search(r'^title:\s*(.+?)(?:\n|$)', content, re.MULTILINE)
        if fm_match:
            title = fm_match.group(1).strip().strip('"').strip("'")
            if title:
                return title

        # first H1
        h1_match = re.search(r'^#\s+(.+?)(?:\n|$)', content, re.MULTILINE)
        if h1_match:
            return h1_match.group(1).strip()

        return file_path.stem
    except Exception:
        return file_path.stem


def get_recent_commits(repo_root: str, limit: int) -> list[dict]:
    """
    遍历 git log，按 commit 分组，每个 commit 取 1 个代表性文件。
    跳过批量提交和纯格式提交。
    """
    # 获取 commit 列表（hash + date + message + 文件列表）
    # 只查 _posts/ 路径下的历史，加速查询
    result = subprocess.run(
        ["git", "log", "-z", "--name-only", "--format=@@COMMIT@@%h|@|%ai|@|%s", "--", "_posts/"],
        cwd=repo_root,
        capture_output=True,
        timeout=120,
        encoding="utf-8",
        errors="replace",
    )
    stdout = result.stdout or ""
    parts = stdout.split("\0")

    commits = []
    current_commit = None
    current_files = []

    for p in parts:
        p = p.strip()
        if not p:
            continue
        if p.startswith("@@COMMIT@@"):
            # 保存上一个 commit
            if current_commit:
                current_commit["files"] = current_files
                commits.append(current_commit)
            # 解析新 commit
            info = p[len("@@COMMIT@@"):]
            fields = info.split("|@|")
            if len(fields) >= 3:
                current_commit = {
                    "hash": fields[0],
                    "date_str": fields[1],
                    "message": fields[2] if len(fields) > 2 else "",
                }
                try:
                    current_commit["date"] = datetime.strptime(
                        fields[1], "%Y-%m-%d %H:%M:%S %z"
                    )
                except ValueError:
                    current_commit["date"] = None
                current_files = []
            else:
                current_commit = None
                current_files = []
        else:
            if current_commit is not None:
                p = p.lstrip("\n")
                if p.startswith("source/_posts/") and p.endswith(".md"):
                    rel = p[len("source/_posts/"):]
                    current_files.append(rel)

    # 最后一个 commit
    if current_commit:
        current_commit["files"] = current_files
        commits.append(current_commit)

    # 过滤并选取代表性文件
    seen_files = set()  # 去重用
    selected = []
    for commit in commits:
        files = commit["files"]
        msg = commit.get("message", "").lower()
        date = commit.get("date")

        if not date:
            continue
        if not files:
            continue

        # 排除 index.md
        files = [f for f in files if f != "index.md"]
        if not files:
            continue

        # 排除 excalidraw
        files = [f for f in files if "excalidraw" not in f.lower()]
        if not files:
            continue

        # 排除 _drafts
        files = [f for f in files if "_drafts" not in f]
        if not files:
            continue

        # 跳过批量提交（> 20 个文件）
        if len(files) > 20:
            continue

        # 跳过纯格式/样式提交
        skip = False
        for kw in SKIP_COMMIT_KEYWORDS:
            if kw in msg:
                skip = True
                break
        if skip:
            continue

        # 选取代表性文件：
        # 如果 commit message 提到了某个关键词，优先选匹配的文件
        # 否则选第一个文件
        rep_file = files[0]
        if len(files) > 1:
            # 尝试从 commit message 中提取关键词匹配文件名
            msg_clean = commit.get("message", "")
            for f in files:
                # 取文件名（不含扩展名）作为关键词
                fname = Path(f).stem.lower()
                if fname and fname in msg_clean.lower():
                    rep_file = f
                    break

        # 去重：同一个文件只取第一次出现（最近的 commit）
        if rep_file in seen_files:
            continue
        seen_files.add(rep_file)

        selected.append({
            "hash": commit["hash"],
            "date": date,
            "message": commit.get("message", ""),
            "file": rep_file,
            "file_count": len(files),
        })

        if len(selected) >= limit:
            break

    return selected


def main():
    parser = argparse.ArgumentParser(description="更新 index.md 中的「🔗 最近更新」列表")
    parser.add_argument("--limit", type=int, default=7, help="显示最近 N 篇文章（默认 7）")
    parser.add_argument("--posts-dir", type=str, default="_posts", help="_posts 目录路径")
    parser.add_argument("--index", type=str, default=None, help="index.md 路径")
    parser.add_argument("--repo-root", type=str, default=".", help="Git 仓库根目录")
    parser.add_argument("--dry-run", action="store_true", help="只输出结果，不修改文件")
    args = parser.parse_args()

    posts_dir = Path(args.posts_dir).resolve()
    index_path = Path(args.index).resolve() if args.index else posts_dir / "index.md"
    repo_root = str(Path(args.repo_root).resolve())

    if not posts_dir.exists():
        print(f"❌ _posts 目录不存在: {posts_dir}", file=sys.stderr)
        sys.exit(1)
    if not index_path.exists():
        print(f"❌ index.md 不存在: {index_path}", file=sys.stderr)
        sys.exit(1)

    print(f"📋 扫描 git log...")
    commits = get_recent_commits(repo_root, args.limit)

    if not commits:
        print("⚠️  未找到符合条件的提交记录", file=sys.stderr)
        sys.exit(1)

    print(f"\n✅ 选取了 {len(commits)} 个最近更新:")
    entries = []
    for c in commits:
        md_file = posts_dir / c["file"]
        if not md_file.exists():
            print(f"   ⚠️  文件不存在: {c['file']}")
            continue
        title = extract_title(md_file)
        icon = get_category_icon(c["file"])
        link_path = c["file"].replace(" ", "%20")
        date_str = c["date"].strftime("%Y-%m-%d")
        entries.append({
            "icon": icon,
            "title": title,
            "link_path": link_path,
            "date": date_str,
            "hash": c["hash"],
        })
        extra = f" ({c['file_count']} files)" if c["file_count"] > 1 else ""
        print(f"   {icon} [{date_str}] {title} ← {c['hash']}{extra}")

    if not entries:
        print("⚠️  没有有效的条目", file=sys.stderr)
        sys.exit(1)

    # 生成 Markdown 片段
    lines = ["## 🔗 最近更新", ""]
    for e in entries:
        lines.append(f"- {e['icon']} [{e['title']}](./{e['link_path']})")
    new_section = "\n".join(lines)

    if args.dry_run:
        print("\n--- 生成的 Markdown 片段 ---")
        print(new_section)
        print("---")
        return

    # 替换 index.md 中的段落
    content = index_path.read_text(encoding="utf-8-sig")

    # 匹配: ## 🔗 最近更新 到下一个 ---
    pattern = r'(## 🔗 最近更新\n)(.*?)(\n---\n)'
    replacement = r'\1\n' + new_section.split("## 🔗 最近更新\n", 1)[-1] + r'\3'

    new_content = re.sub(pattern, replacement, content, flags=re.DOTALL)

    if new_content == content:
        print("\n⚠️  内容无变化", file=sys.stderr)
        sys.exit(1)

    index_path.write_text(new_content, encoding="utf-8")
    print(f"\n✅ 已更新 {index_path}")


if __name__ == "__main__":
    main()
