import serial
import time

print("Start")
port="COM8"
baudrate=9600

bt_conn = serial.Serial(port,baudrate)
print("Connected")
bt_conn.flushInput()

for i in range(5):
    print("ping")
    bt_conn.write(b"BOOP "+str.encode(str(i)))
    input_data=bt_conn.readline()
    print(input_data.decode())
    time.sleep(0.1)
bt_conn.close()