import socket
import os
import numpy as np
import cv2
import sys
sys.set_int_max_str_digits(0)
# Define the length of

max_receive_bytes = 65000  # may size of the received bytes through TCP connection
SIZE_OF_INT = 4

# Create folder to store received frames
folder_path = "received_frames"
os.makedirs(folder_path, exist_ok=True)


class TCP_protocol:
    def __init__(self, IP, port):
        """Initialise the TCP protocol for receiving image data
        IP = the IPv4 address of the server
        port = the port TCP listens at"""
        # number of frames received
        self.framecount = 0
        #Create TCP socket using the server IP and port
        self.TCP_IP = IP
        self.TCP_PORT = port
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.bind((self.TCP_IP, self.TCP_PORT))
        self.sock.listen(5)

        print("Server is listening on", self.TCP_IP, self.TCP_PORT)

    def __close__(self):
        """Cleanup - close the socket and destroy the cv2 window"""
        self.sock.close()
        cv2.destroyAllWindows()

    def show(self,bytesarray):
        frame = cv2.imdecode(np.frombuffer(bytesarray, dtype=np.uint8), cv2.IMREAD_COLOR)
        cv2.imshow("Received frames", frame)
        cv2.waitKey(1)

    def connect(self):
        """Wait to connect to the client
        -- the socket will try to connect to the server for 10 minutes and then break
        -- once the connection with client is established , the server receives the byte array via TCP
        -- The byte array has length-prefix framing since frames of variable length are sent by server
            -- the first 4 bytes contain the length of the incoming encoded frame/image
        -- Read the byte array upto the prefix length and show the image using opencv """

        #try to connect to the server for 10 minutes then break
        self.sock.settimeout(600)
        #variable that holds the incoming data in bytes
        data = b''
        while True:
            print("Waiting for a connection..")
            conn, addr = self.sock.accept()

            with conn:
                print(f"Connected to client with address : {addr}")
                data += conn.recv(max_receive_bytes)
                #get the size of the first frame immediately after connecting
                next_frame_len = int.from_bytes(data[:SIZE_OF_INT], "little")
                print(f"Next frame size : {next_frame_len}")
                print(len(data))
                data = data[SIZE_OF_INT:]
                total_len = (next_frame_len + SIZE_OF_INT)

                while (len(data)>0):
                    if (len(data) < (total_len)):
                        data += conn.recv(max_receive_bytes)
                    else:
                        ##flush the required size of bytes into img_bytes
                        img_in_bytes = data[:next_frame_len]
                        ##update the length of next frame
                        next_frame_len = int.from_bytes(data[next_frame_len:total_len], "little")
                        ##reset data as the image frame is already read
                        data = data[total_len:]
                        ##update total length
                        total_len = next_frame_len + SIZE_OF_INT
                        print(f"Next frame size : {next_frame_len}")
                        self.show(img_in_bytes)

                print("The client is no longer connected to the server")
                self.__close__()


def main():
    SERVER_IP = "127.0.0.1"
    PORT = 5001
    try :
        tcp = TCP_protocol(SERVER_IP, PORT)
        tcp.connect()
    except TimeoutError:
        print("Socket connection timed out. Unable to connect to client")
        tcp.connect()
        pass
    except KeyboardInterrupt:
        print("Keyboard interrupt")
        tcp.__close__()
        pass
    except ConnectionResetError:
        print("Connection was closed by the client")
        tcp.__close__()
        pass
    except Exception as error:
        tcp.__close__()
        raise



if __name__ == "__main__":
    main()







            #print(data)
            #jpeg_len = int.from_bytes(data[:4], "little")
            #print(jpeg_len)









            # decode the data received from the socket
            #framearray = np.frombuffer(data, dtype=np.uint8)
            #print(framearray)
            #frame = cv2.imdecode(framearray, cv2.IMREAD_COLOR)

            # if frame is None :
            #     counter += 1
            #     print(counter, " frames are None out of", framecount)
            #
            #
            # # Display frame
            # if frame is not None:
            #     cv2.imshow("Received Frame", frame)
            #     cv2.waitKey(1)  # Adjust wait time as needed

    # Save frame to folder
    #frame_path = os.path.join(folder_path, f"frame_{time.time()}.jpg")
    #cv2.imwrite(frame_path, frame)


