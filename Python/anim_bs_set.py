import time
import pyautogui

time.sleep(2)

for i, txt in enumerate(['RightSpeed', '-300', '300', '8', '0.1']):
    pyautogui.click(400, (255+25*i)*2)
    pyautogui.typewrite(txt, interval=0.1)

for i, txt in enumerate(['ForwardSpeed', '-300', '300', '8', '0.1']):
    pyautogui.click(400, (420+25*i)*2)
    pyautogui.typewrite(txt, interval=0.1)

for txt in ['idle', 'jog_left_start', 'jog_left', 'jog_right_start', 'jog_right', 'jog_fwd_start', 'jog_fwd', 'jog_bwd_start', 'jog_bwd']:
    pyautogui.click(1465*2, 725*2)
    pyautogui.hotkey('ctrl', 'a')
    pyautogui.press('delete')
    pyautogui.typewrite(txt, interval=0.1)

    if txt == 'idle':
        time.sleep(15.0)
    else:
        time.sleep(5.0)




