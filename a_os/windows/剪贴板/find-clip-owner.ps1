# find-clip-owner.ps1
# 查找当前持有 Windows 剪贴板（OpenClipboard 未 CloseClipboard）的窗口及其所属进程。
# 使用时机：粘贴/复制无响应、Set-Clipboard 报"所请求的剪贴板操作失败"时，立即运行。
#   powershell -ExecutionPolicy Bypass -File find-clip-owner.ps1

Add-Type -MemberDefinition @'
[DllImport("user32.dll")] public static extern IntPtr GetOpenClipboardWindow();
[DllImport("user32.dll")] public static extern uint GetWindowThreadProcessId(IntPtr hWnd, out uint processId);
'@ -Name Clip -Namespace Native

$hwnd = [Native.Clip]::GetOpenClipboardWindow()
Write-Output ("Open clipboard window handle: 0x{0:X}" -f $hwnd.ToInt64())

if ($hwnd -eq [IntPtr]::Zero) {
    Write-Output "No process currently holds the clipboard open (the occupation may be transient). Run again at the moment it is stuck."
    exit 0
}

$ownerPid = [uint32]0
[void][Native.Clip]::GetWindowThreadProcessId($hwnd, [ref]$ownerPid)

if ($ownerPid -eq 0) {
    Write-Output "Window found but no owning process id could be read."
    exit 1
}

Get-Process -Id $ownerPid -ErrorAction SilentlyContinue |
    Select-Object Id, ProcessName, Responding, Path |
    Format-List
