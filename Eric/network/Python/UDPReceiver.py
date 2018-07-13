import socket
from random import randint

color = ('00FF00', '00FFFF', '0000FF')
cl = color.__len__()

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server_address = ('127.0.0.1', 8890)

# index for coordinate
i = 0
# index for package
p = 1
# total number of coordinates in a single package
index_max = 200
# total number of packages for a frame
p_total = 50

while 1:
    while p <= p_total:
        # header for each package
        message = str(p) + '+' + str(p_total)
        # coordinates following the package header
        while i < index_max:
            x = str(randint(0, 255))
            y = str(randint(0, 255))
            z = str(randint(0, 255))
            message = message + '+' + x + '/' + y + '/' + z + '/' + color[cl % 3]
            i = i + 1
        print("Message length: ", message.__len__())
        print("Sending: ", message)
        sock.sendto(message.encode('utf-8'), server_address)
        print("Send Success!")
        # time.sleep(0.0001)
        p = p + 1
        i = 0
    p = 1
    cl = cl + 1

# Each package will look something like:
#
# p+p_total+x1/y1/z1/color1+x2/y2/z2/color2+x3/y3/z3/color3+......
# --------- --------------- --------------- --------------- ------
# header    1st coordinate  2nd coordinate  3rd coordinate  4th...
