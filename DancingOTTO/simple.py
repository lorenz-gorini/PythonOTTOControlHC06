from typing import List

import serial
import time

print("Start")
# port_OTTO_1="/dev/tty.HC-06-DevB" #This will be different for various devices and on windows it will probably be a COM
BAUDRATE = 9600

ports = [
    # "COM14",
    "COM8",
    # "COM10",
    # # "COM11",
    # ,
    # "/dev/tty.HC-06-DevB",
    # "/dev/tty.HC-06-DevB",
]

command_list = {
    "home":'0',
    "back":'1',
    "forw":'2',
    "right":'3',
    "left":'4',
    "sing":'6',
    "swing":'8',
    "jump":'9',
    "dance": '5',
    "dance2":'7',
}

class ConnectionOTTO:

    def __init__(self, port):
        self.bluetooth = serial.Serial(port, BAUDRATE)  # Start communications with the bluetooth unit
        print("Connected")
        self.bluetooth.flushInput()

    def send_command(self, command: str):
        print(f"send command: {command}")
        self.bluetooth.write(str.encode(str(command)))  # These need to be bytes not unicode, plus a number #
        # b"Command: "+
        # input_data = self.bluetooth.readline()
        # print(f"Response: {input_data.decode()}")

    def close_connection(self):
        self.bluetooth.close()

class OTTOGroupClass:
    def __init__(self, ports:List[str]):
        self.otto_conns_list = []
        for port in ports:
            self.add_OTTO(port)

    def add_OTTO(self, port):
        self.otto_conns_list.append(ConnectionOTTO(port))

    def send_group_command(self, command:str):
        for otto in self.otto_conns_list:
            if command in command_list.keys():
                otto.send_command(command_list.get(command.lower()))
            else:
                print("Wrong command. You can do better than this!")

    def close_connections(self):
        for otto in self.otto_conns_list:
            otto.close_connection()



def start():
    otto_group = OTTOGroupClass(ports=ports)
    input_text = input("insert a command: ")
    while input_text.lower() != "esc":
        otto_group.send_group_command(input_text)
        input_text = input("insert a command: ")
    otto_group.close_connections()

if __name__ == "__main__":
    start()




# def create_connection():
# 	bluetooth=serial.Serial(port, 9600)#Start communications with the bluetooth unit
# 	print("Connected")
# 	bluetooth.flushInput() #This gives the bluetooth a little kick
# 	for i in range(5): #send 5 groups of data to the bluetooth
# 		print("Ping")
# 		bluetooth.write(b"BOOP "+str.encode(str(i)))#These need to be bytes not unicode, plus a number
# 		input_data=bluetooth.readline()#This reads the incoming data. In this particular example it will be the "Hello from Blue" line
# 		print(input_data.decode())#These are bytes coming in so a decode is needed
# 		time.sleep(0.1) #A pause between bursts
# 	bluetooth.close() #Otherwise the connection will remain open until a timeout which ties up the /dev/thingamabob
# 	print("Done")