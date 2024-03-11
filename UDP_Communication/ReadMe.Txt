C++ client and Python server to send and receives encoded data via UDP 

C++ client 
-platform windows
-The client has two classes 'sendimg_UDP class' and 'sendFrameOfVideo_UDP'
-sendimg_UDP class is used to send image to the server via socket
-sendFrameOfVideo_UDP class is used to load frames of .mp4 , encode and send it via UDP socket
-UDP is built on winsock
-Uses opencv to load and encode images

Python server
-platform windows
-Server receives the data buffer, decodes and shows it using opencv 

Required libraries - opencv, winsock 
