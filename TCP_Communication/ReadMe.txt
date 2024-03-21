Cpp client code to send image or frames of video via TCP socket using opencv
-platform windows
-sendimg_TCP class is used to send image to the server via socket
-sendFrameOfVideo_TCP class is used to load frames of .mp4 , encode and send it via TCP socket with length prefix framing 
-TCP is built on winsock
-opencv version==4.4.0

Pythin receive code 
-receive encoded frames along with their length 
- read the length (N) and then read the N bytes subsequently 
