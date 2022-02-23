"""
생체데이터 수집기(C++)로부터 데이터를 얻어 파이썬으로 보내는 코드
소켓통신

"""


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
    try:
        data = commend.decode("utf-8").partition('\n')[0].split(', ')
        num = float(data[0]) #num이 들어오는 생체값임
        print(num)
    except:
        print("종료합니다")
        break

csock.close()
