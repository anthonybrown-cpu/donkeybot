import time
import math
import socket
import XboxController

DONKEY_HOST = "192.168.1.174"
DONKEY_PORT = 80

joy = XboxController.XboxController()

def loop():
    # Try to connect to drone
    while True:
        #print("Connecting...")
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            x, y, lt, rt = joy.read()
            thr = round(0.5 + (lt/2) - (rt/2), 2)
            str = round(0.5 + (x/2), 2)
            msg = "THR %s STR %s" % (thr, str)
            
            s.connect((DONKEY_HOST, DONKEY_PORT))
            s.sendall(msg.encode())
            #print(msg)
            time.sleep(1/60)

if (__name__ == '__main__'):
    loop()