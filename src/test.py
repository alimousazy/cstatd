#!/usr/bin/env python
import sys 
import socket
import time
 
TCP_IP = '127.0.0.1'
TCP_PORT = 8888 
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))
for x in range(1, 1000000):
	MESSAGE = str(int(time.time())) + ",nameX" + str(x) + ",1222"
	s.send(str(len(MESSAGE)) + "-" + MESSAGE)
s.close()

