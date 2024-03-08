import cv2
import socket
import struct
import pickle
import time

# Configure video capture
cap = cv2.VideoCapture(0)
cap.set(3, 1280) # Set width
cap.set(4, 720) # Set height
cap.set(cv2.CAP_PROP_FPS, 30) # Set frame rate

# Configure network connection
HOST = '192.168.1.100' # IP address of the receiving end
PORT = 12345 # Choose a port number
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect((HOST, PORT))
connection = client_socket.makefile('wb')

try:
  while True:
 # Capture frame-by-frame
    ret, frame = cap.read()
    if not ret:
        break

 # Serialize frame
    data = pickle.dumps(frame)
 # Send frame size and data
    size = len(data)
    client_socket.sendall(struct.pack(">L", size) + data)
 # Wait for 1 second
    time.sleep(1)
    
finally:
 # Release resources
 connection.close()
 client_socket.close()
 cap.release()