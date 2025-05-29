from ctypes import *
import threading
import sys,os
import pystray
from PIL import Image
from pystray import MenuItem

def resource_path(relative_path):
    """ Get absolute path to resource, works for dev and for PyInstaller """
    try:
        # PyInstaller creates a temp folder and stores path in _MEIPASS
        base_path = sys._MEIPASS
    except Exception:
        base_path = os.path.abspath(".")

    return os.path.join(base_path, relative_path)

img_dir = resource_path('./icon.png')
so_dir = resource_path('./setWindowtoBackground.so')

result = cdll.LoadLibrary(so_dir)

def _f2(icon, item):
    result.show_bg()


def icon_stop(icon, item):
    os._exit(0)


def start_icon():
    global icon
    menu = (MenuItem(text='还原窗口',action=_f2), MenuItem(text='退出',action=icon_stop))
    image = Image.open(img_dir)
    icon = pystray.Icon('setWindowtoBackgroung', image, '任意窗口设置桌面壁纸', menu)
    icon.run()

thread = threading.Thread(target=start_icon)
thread.start()
result.main()
icon.stop()
sys.exit()