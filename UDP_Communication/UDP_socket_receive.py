import socket
import cv2
import os
import numpy as np

# UDP server IP and port
UDP_IP = "127.0.0.1"
UDP_PORT = 5001

# Create UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

# Create folder to store received frames
folder_path = "received_frames"
os.makedirs(folder_path, exist_ok=True)

# Receive and process video frames
while True:
    print(f"Waiting to receive data..")
    # Receive frame data
    data, addr = sock.recvfrom(40000)  # Adjust buffer size as needed
    print(f"Received data from addr to {addr}")
    #print(f"Received data from addr to {data}")

    #decode the data received from the socket
    frame = np.frombuffer(data, dtype=np.uint8)
    frame = cv2.imdecode(frame, cv2.IMREAD_COLOR)

    # Display frame
    cv2.imshow("Received Frame", frame)
    cv2.waitKey(1)  # Adjust wait time as needed

    # Save frame to folder
    #frame_path = os.path.join(folder_path, f"frame_{time.time()}.jpg")
    #cv2.imwrite(frame_path, frame)

# Cleanup
cv2.destroyAllWindows()
sock.close()