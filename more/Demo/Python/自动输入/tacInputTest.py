"""
python -m PyInstaller -F -w  -n auto_input ./auto_input.py
"""
import pathlib
import pyautogui

# 控制速度
interval = 0.05

tac = {
    'admin':'tab',
    'Admin@2022':'enter'
}

wiki = {
    'admin':'tab',
    'Admin@2022':'enter'
}

text_alert = """开始前请确保完成以下操作
    1. 请切换为英文输入法
    2. 请打开输入界面
    3. 将光标放至开始输入的位置
    4. 选择要输入的代码
"""

def main():
    global codes
    option = pyautogui.confirm(text=text_alert, title='自动输入小工具', buttons=['tac','wiki'])
    if option == 'tac':
        for key in tac:
            print(key, ":", tac[key])
            pyautogui.typewrite(key, interval=interval)
            pyautogui.press(tac[key], interval=interval)
    elif option == 'wiki':
        for key in tac:
            print(key, ":", wiki[key])
            pyautogui.typewrite(key, interval=interval)
            pyautogui.press(wiki[key], interval=interval)


if __name__ == '__main__':
    main()

