/* Cpp client code to send image via TCP socket using opencv
-platform windows
-sendimg_TCP class is used to send image to the server via socket
-sendFrameOfVideo_TCP class is used to load frames of .mp4 , encode and send it via TCP socket with length prefix framing 
-TCP is built on winsock
-Uses opencv to load images
*/

/* Cpp client code to send image via TCP socket using opencv
-platform windows
-sendimg_TCP class is used to send image to the server via socket
-sendFrameOfVideo_TCP class is used to load frames of .mp4 , encode and send it via TCP socket
-TCP is built on winsock
-Uses opencv to load images
*/

#define WIN32_LEAN_AND_MEAN
#define _CRTDBG_MAP_ALLOC

//reqired for sleep
#include <windows.h> // Sleep
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

//check if these are needed
#include <iostream>
#include <conio.h>
#include <sstream>
#include <fstream>

//opencv headers
#include "opencv2/opencv.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

//winsock headers 
#include <winsock2.h>
#include <ws2tcpip.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

//define the port and the IP address of the TCP server. It should be "localhost" or 127.0.0.1 for client and server
//running on the same machine 
#define DEFAULT_PORT 5001 //the port on which to listen and send data
#define SERVER "127.0.0.1"  //"10.203.30.144" //"localhost" - ip address of TCP server

using namespace std;
using namespace cv;

class sendimg_TCP {

public:
	WSADATA ws;
	SOCKET client_socket;
	sockaddr_in server;

	//Member functions of the sendimg_TCP class
	int initialise_socket();
	int send_image();

};

int sendimg_TCP::initialise_socket()
{
	cout << "Initialising winsock" << endl;
	if (WSAStartup(MAKEWORD(2, 2), &ws) != 0) {
		printf("Failed. Error Code: %d", WSAGetLastError());
		return 0;
	}

	// create socket
	if ((client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == SOCKET_ERROR) {
		printf("socket() failed with error code: %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	// setup address structure
	memset((char*)&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(DEFAULT_PORT);
	inet_pton(AF_INET, SERVER, &server.sin_addr.s_addr);

	// sending connection request 
	while (true) {
		//keep trying to connect until
		if ((connect(client_socket, (struct sockaddr*)&server, sizeof(server))) == -1) {
			cout << "Server is not responding. Connect again.\n";
		}
		else {
			cout << "Connected to the server" << endl;
			break;
		}
	}
}

int sendimg_TCP::send_image() {
	//Open the imagefile file using opencv
	std::string image_path = samples::findFile("C:\\Users\\nadine\\ams\\NanEye2D_EvalSW_API_NanoUSB2_cpackage_pWin_v1-1-3-1\\WorkingDirectory\\SurgeonsEye\\100ptx_image.jpeg");
	Mat img = imread(image_path, IMREAD_COLOR);
	if (img.empty())
	{
		std::cout << "Could not read the image: " << image_path << std::endl;
	}

	//create params for encoding the image to be sent 
	std::vector<uchar> buf;
	std::vector<int> param(2);
	param[0] = cv::IMWRITE_JPEG_QUALITY;
	param[1] = 80;

	//encode the image into a 1D vector buf
	cv::imencode(".JPG", img, buf, param);
	//get the length of the buffer 
	int bufsize = buf.size();

	//display the image for checking
	imshow("Display window", img);
	int k = waitKey(1); // Wait for a keystroke in the window */

	//Send Image to the server 
	send(client_socket, (const char*)&buf[0], bufsize, 0);
	cout << "sending image successful.." << endl;

	closesocket(client_socket);

}


class sendFrameOfVideo_TCP {

public:
	WSADATA ws;
	SOCKET client_socket;
	sockaddr_in server;

	//Member functions of the sendimg_TCP class
	int initialise_socket();
	int send_frames();

};

int sendFrameOfVideo_TCP::initialise_socket()
{
	//cout << "Initialising winsock" << endl;
	if (WSAStartup(MAKEWORD(2, 2), &ws) != 0) {
		printf("Failed. Error Code: %d", WSAGetLastError());
		return 0;
	}

	// create socket
	if ((client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == SOCKET_ERROR) {
		printf("socket() failed with error code: %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	// setup address structure
	memset((char*)&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(DEFAULT_PORT);
	inet_pton(AF_INET, SERVER, &server.sin_addr.s_addr);

	// sending connection request 
	while (true) {
		//keep trying to connect until
		if ((connect(client_socket, (struct sockaddr*)&server, sizeof(server))) == -1) {
			cout << "Server is not responding. Connect again.\n";
		}
		else {
			cout << "Connected to the server" << endl;
			break;
		}
	}
}

int sendFrameOfVideo_TCP::send_frames() {
	//Open the video file
	// initialize a video capture object
	VideoCapture vid_capture("C:/Users/nadine/Downloads/examplevideo_UDP.mp4");

	//vector to be sent to the server = length of the encoded  image in bytes + encoded image in bytes
	std::vector<uchar> buf;
	//length of encoded image 
	int bufsize = 0;
	//parameters required for encoding each frame 
	std::vector<int> param(2);
	param[0] = cv::IMWRITE_JPEG_QUALITY;
	param[1] = 80;//default(95) 0-100


	// Print error message if the stream is invalid
	if (!vid_capture.isOpened()) {
		cout << "Error opening video stream or file" << endl;
		exit(EXIT_FAILURE);
	}
	else {
		// Obtain fps and frame count by get() method and print
		// You can replace 5 with CAP_PROP_FPS as well, they are enumerations
		int fps = vid_capture.get(CAP_PROP_FPS);
		cout << "Frames per second :" << fps;

		// Obtain frame_count using opencv built in frame count reading method
		// You can replace 7 with CAP_PROP_FRAME_COUNT as well, they are enumerations
		int frame_count = vid_capture.get(CAP_PROP_FRAME_COUNT);
		cout << "  Frame count :" << frame_count;
	}


	// Read the frames to the last frame
	while (vid_capture.isOpened())
	{
		// Initialise frame matrix
		Mat frame;

		// Initialize a boolean to check if frames are there or not
		bool isSuccess = vid_capture.read(frame);

		// If frames are present, show it
		if (isSuccess == true)
		{
			try {
				//until the end of file read video 
				while (vid_capture.read(frame))
				{
					//show the frame for debugging purposes
					cv::imshow("frame", frame);
					int key = cv::waitKey(1);
					//encode the frame into 1D vector buf 
					cv::imencode(".JPG", frame, buf, param);
					//get size of buffer to be passed as prefix
					bufsize = buf.size();
					buf.reserve(sizeof(INT) + bufsize);
					//insert the buffer size as prefix to the encoded image bytes 
					buf.insert(buf.begin(), reinterpret_cast<unsigned char*>(&bufsize),
						reinterpret_cast<unsigned char*>(&bufsize) + sizeof(INT));
					//update the buf size to be sent to the server
					bufsize = buf.size();
					//send the buf byte array to the server 
					send(client_socket, (const char*)&buf[0], bufsize, 0);
					cout << bufsize << endl;
				}
			}
			catch (const std::exception& ex)
			{
				cout << "Exception raised: " << ex.what() << endl;
			}
		}

		// If frames are not there, close it
		if (isSuccess == false)
		{
			cout << "Video camera is disconnected" << endl;
			exit(EXIT_FAILURE);
		}
	}


	// Close the video file and socket
	vid_capture.release();
	closesocket(client_socket);
}


int main() {

	cout << "c++ client code to send a image via TCP using opencv" << endl;
	//to send image 
	/*sendimg_TCP sendimagetoserver;
	sendimagetoserver.initialise_socket();
	sendimagetoserver.send_image();*/

	//to send frames of a .mp4 video 
	sendFrameOfVideo_TCP sendframestoserver;
	sendframestoserver.initialise_socket();
	sendframestoserver.send_frames();
	

	Sleep(10000);
	return 0;
}