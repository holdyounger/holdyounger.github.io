#!/usr/bin/python
# -*- coding: UTF-8 -*-
# 文件名：client.py
 
from ast import Try
import socket               # 导入 socket 模块
import sys

socketList = []

host = socket.gethostname() # 获取本地主机名
remoteHost = input('请输入远程IP,如<192.168.3.128>：')
remotePort = eval(input('请输入远程端口：'))                # 设置端口号

errNum = 0
ToTalNum = 0
errPorts = {}

print("请通过\'netsh int ipv4 show dynamicport tcp\'查看启动端口和端口数,一般情况下该值为49152")
start = eval(input("本地启动端口:"))
end = eval(input("本地终止端口:"))

for i in range(start , end): 
    ToTalNum += 1
    s = socket.socket()         # 创建 socket 对象
    localHostAndPort = (host,i)
    try:
        s.bind(localHostAndPort)
        if s.connect((remoteHost, remotePort)) == socket.error:
            print('FAILED', i)
        print(s.recv(1024).decode(), i)
        socketList.append(s)
    except OSError as e:
        errNum += 1
        errPorts[i] = (localHostAndPort, "OSError:", e)

print("Total Port Number(%r)"%ToTalNum, "Error Port Number(%r)"%errNum)
print("Error Ports:")
for err in errPorts.keys():
    print(err, errPorts[err])

if len(errPorts) == 0:
    print("All Ports success (%d) ~"%start, "(%d)"% end)

print("输入\'exit\'关闭CLIENT:")
char = input()
while char != "exit":
	# print(char)
	char = input()

for skt in socketList:
    skt.close()
# print(socketList)