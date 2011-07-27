import os
import socket

while 1:
    s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    s.connect("/tmp/serveposition")
    message = raw_input("Enter your message: ")
    s.send(message)
    data = s.recv(1024)
    s.close()
    print 'Received', repr(data)
    #this command will set the position of servo 0 to 5000
    #accepable ranges could be between 4000 - 8000 but that should be confirmed
    #os.system("~/src/maestro_linux/UscCmd --servo 0,5000)
