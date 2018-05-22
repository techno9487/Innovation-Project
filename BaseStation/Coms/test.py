import socket, json

sock = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
sock.connect(('127.0.0.1',1025))

obj = {"type":"initial","device_id":6}
msg = json.dumps(obj)+"\n"

sock.send(msg.encode("utf-8"))
sock.close()