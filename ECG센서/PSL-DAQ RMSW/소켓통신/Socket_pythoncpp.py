from socket import *

class SocketInfo():
    HOST = '127.0.0.1'
    PORT = 1234
    BUFSIZE = 6
    ADDR = (HOST, PORT)
    
class socketInfo(SocketInfo):
    HOST = '127.0.0.1'
    

csock = socket(AF_INET, SOCK_STREAM)
csock.connect(socketInfo.ADDR)
    
while(True):
   
    commend = csock.recv(socketInfo.BUFSIZE, MSG_WAITALL)
    data = commend.decode("utf-8").partition('\n')[0].split(', ')
    num = float(data[0][:5])
    print(num)
    #print(type(num))
    
    #print("commend: ",commend,"\n")
    #print("data: ", data[:4],'\n')


csock.close()
