import time
import serial
import XboxController

joy = XboxController.XboxController()

def loop():
    ready_to_send = False
    # Try to connect to drone
    serial_port = input("Enter serial port: ")

    with serial.Serial(serial_port, 115200, timeout=0.1) as ser:
        while(True):
            if(ready_to_send):
                debug("Creating command string")
                x, y, lt, rt = joy.read()
                thr = round(0.5 + (lt/2) - (rt/2), 2)
                str = round(0.5 + (x/2), 2)
                msg = "THR %s STR %s" % (thr, str)
                debug("Command string ready. Sending...")
                ser.write(msg.encode() + '\n'.encode())
                debug("Sent.")
            
            debug("Getting responses...")

            while(ser.in_waiting): 
                resp = ser.readline().decode()
                if(resp.find("Init complete!") != -1):
                    ready_to_send = True
                #print("ESP32: " + resp.strip())

            debug("Sleeping...")
            time.sleep(1/30)
            debug("Here we go again")

def debug(msg):
    pass
    #print("[" + str(time.time() * 1000) + "] " + msg)

if (__name__ == '__main__'):
    loop()