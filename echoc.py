import socket

while 1:
    s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    s.connect("/tmp/serveposition")
    message = raw_input("Enter your message: ")
    s.send(message)
    data = s.recv(1024)
    s.close()
    print 'Received', repr(data)

