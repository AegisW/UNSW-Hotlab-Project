#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/registration.h>
#include <libfreenect2/packet_pipeline.h>
#include <libfreenect2/logger.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <pthread.h>
#include <signal.h>

// OpenCV includes
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// TCPIP includes
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

// Standard includes
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#ifdef EXAMPLES_WITH_OPENGL_SUPPORT
#include "viewer.h"
#endif

using namespace cv;
using namespace std;

bool protonect_shutdown = false; ///< Whether the running application should shut down.

// Initialize socket parameters
int sockfd, portno, n;
struct sockaddr_in serv_addr;
struct hostent *server;

// Buffer storing last frame data
const int width = 512;
const int height = 424;
const int packetSize = width*height+1;

// Buffer structure
struct myPointCloudBuffer {
    uint8_t z[packetSize];
    uint8_t r[packetSize];
    uint8_t g[packetSize];
    uint8_t b[packetSize];
} pointCloudBuffer;

// Color conversion
union myColor{
    uint8_t a;
    uint8_t b[4];
} color;

void sigint_handler(int s);

// Error message for networking
void error(const char *msg);

class myMutex {
	public:
	myMutex() {
		pthread_mutex_init(&m_mutex, NULL);
	}
	void lock() {
		pthread_mutex_lock(&m_mutex);
	}
	void unlock() {
		pthread_mutex_unlock(&m_mutex);
	}

	private:
	pthread_mutex_t m_mutex;
};

int main(int argc, char *argv[]) {
	// Establish socket and connection
    portno = 8890;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("Error opening socket!");
    server = gethostbyname("149.171.37.126");
    //server = gethostbyname("127.0.0.1");
    if (server == NULL) {
        fprintf(stderr, "Error, no such host!\n");
        exit(0);
    }                
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
        (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) error("Error connecting!");

    cout << endl;
    cout << "*************************************" << endl;
    cout << "**     Connection established!     **" << endl;
    cout << "*************************************" << endl;
    cout << endl;

	// Initialize buffer for depth value transmission
    bzero(pointCloudBuffer.z,packetSize);
    bzero(pointCloudBuffer.r,packetSize);
    bzero(pointCloudBuffer.g,packetSize);
    bzero(pointCloudBuffer.b,packetSize);

    // Initialize header for buffers
    pointCloudBuffer.z[0]='z';
    pointCloudBuffer.r[0]='r';
    pointCloudBuffer.g[0]='g';
    pointCloudBuffer.b[0]='b';

	libfreenect2::Freenect2 freenect2;
  	libfreenect2::Freenect2Device *dev = 0;
  	libfreenect2::PacketPipeline *pipeline = 0;

	bool viewer_enabled = true;
	bool enable_rgb = true;
	bool enable_depth = true;
	int deviceId = -1;
	std::string serial = "";
	size_t framemax = -1;

	for(int argI = 1; argI < argc; ++argI) {

    	const std::string arg(argv[argI]);

    	if(arg == "-help" || arg == "--help" || arg == "-h" || arg == "-v" || arg == "--version" || arg == "-version")
    	{
      		// Just let the initial lines display at the beginning of main
      		return 0;
    	}
    	else if(arg.find("-gpu=") == 0)
    	{
			if (pipeline)
			{
				std::cerr << "-gpu must be specified before pipeline argument" << std::endl;
				return -1;
			}
      		deviceId = atoi(argv[argI] + 5);
    	}
    	else if(arg == "cpu")
    	{
      		if(!pipeline)
/// [pipeline]
        	pipeline = new libfreenect2::CpuPacketPipeline();
/// [pipeline]
    	}
    	else if(arg == "gl")
    	{
#ifdef LIBFREENECT2_WITH_OPENGL_SUPPORT
      		if(!pipeline) pipeline = new libfreenect2::OpenGLPacketPipeline();
#else
      		std::cout << "OpenGL pipeline is not supported!" << std::endl;
#endif
    	}
    	else if(arg == "cl")
    	{
#ifdef LIBFREENECT2_WITH_OPENCL_SUPPORT
      		if(!pipeline) pipeline = new libfreenect2::OpenCLPacketPipeline(deviceId);
#else
      		std::cout << "OpenCL pipeline is not supported!" << std::endl;
#endif
    	}
    	else if(arg == "clkde")
    	{
#ifdef LIBFREENECT2_WITH_OPENCL_SUPPORT
      		if(!pipeline) pipeline = new libfreenect2::OpenCLKdePacketPipeline(deviceId);
#else
      		std::cout << "OpenCL pipeline is not supported!" << std::endl;
#endif
    	}
    	else if(arg == "cuda")
    	{
#ifdef LIBFREENECT2_WITH_CUDA_SUPPORT
      		if(!pipeline)
        	pipeline = new libfreenect2::CudaPacketPipeline(deviceId);
#else
      		std::cout << "CUDA pipeline is not supported!" << std::endl;
#endif
    	}
    	else if(arg == "cudakde")
    	{
#ifdef LIBFREENECT2_WITH_CUDA_SUPPORT
      		if(!pipeline) pipeline = new libfreenect2::CudaKdePacketPipeline(deviceId);
#else
      		std::cout << "CUDA pipeline is not supported!" << std::endl;
#endif
		}
		else if(arg.find_first_not_of("0123456789") == std::string::npos) //check if parameter could be a serial number
		{
			serial = arg;
		}
		else if(arg == "-noviewer" || arg == "--noviewer")
		{
			viewer_enabled = false;
		}
		else if(arg == "-norgb" || arg == "--norgb")
		{
			enable_rgb = false;
		}
		else if(arg == "-nodepth" || arg == "--nodepth")
		{
			enable_depth = false;
		}
    	else if(arg == "-frames") {
      		++argI;
      		framemax = strtol(argv[argI], NULL, 0);
      		if (framemax == 0) {
        		std::cerr << "invalid frame count '" << argv[argI] << "'" << std::endl;
        		return -1;
      		}
    	}
    	else
    	{
      		std::cout << "Unknown argument: " << arg << std::endl;
    	}
  	}

  	if (!enable_rgb && !enable_depth) {
    	std::cerr << "Disabling both streams is not allowed!" << std::endl;
    	return -1;
	}

	if(freenect2.enumerateDevices() == 0) {
    	std::cout << "no device connected!" << std::endl;
    	return -1;
  	}
  	if (serial == "") {
    	serial = freenect2.getDefaultDeviceSerialNumber();
  	}

	if(pipeline)
  	{
/// [open]
    	dev = freenect2.openDevice(serial, pipeline);
/// [open]
  	}
	else
	{
		dev = freenect2.openDevice(serial);
	}

	if(dev == 0)
	{
		std::cout << "failure opening device!" << std::endl;
		return -1;
	}

	signal(SIGINT,sigint_handler);
	protonect_shutdown = false;

	int types = 0;
	if (enable_rgb) types |= libfreenect2::Frame::Color;
	if (enable_depth) types |= libfreenect2::Frame::Ir | libfreenect2::Frame::Depth;
	libfreenect2::SyncMultiFrameListener listener(types);
	libfreenect2::FrameMap frames;
	dev->setColorFrameListener(&listener);
	dev->setIrAndDepthFrameListener(&listener);

	if (enable_rgb && enable_depth) {
		if (!dev->start())
		return -1;
	}
	else {
		if (!dev->startStreams(enable_rgb, enable_depth))
		return -1;
	}
	std::cout << "device serial: " << dev->getSerialNumber() << std::endl;
	std::cout << "device firmware: " << dev->getFirmwareVersion() << std::endl;

	libfreenect2::Registration* registration = new libfreenect2::Registration(dev->getIrCameraParams(), dev->getColorCameraParams());
  	libfreenect2::Frame undistorted(width, height, 4), registered(width, height, 4);

	size_t framecount = 0;

#ifdef EXAMPLES_WITH_OPENGL_SUPPORT
  	Viewer viewer;
  	if (viewer_enabled) viewer.initialize();
#else
  	viewer_enabled = false;
#endif

	while(!protonect_shutdown && (framemax == (size_t)-1 || framecount < framemax)) {
		if (!listener.waitForNewFrame(frames, 10*1000)) // 10 sconds
		{
			std::cout << "timeout!" << std::endl;
			return -1;
		}
		libfreenect2::Frame *rgb = frames[libfreenect2::Frame::Color];
		libfreenect2::Frame *ir = frames[libfreenect2::Frame::Ir];
		libfreenect2::Frame *depth = frames[libfreenect2::Frame::Depth];

		if (enable_rgb && enable_depth)
    	{
/// [registration]
      		registration->apply(rgb, depth, &undistorted, &registered, true);
/// [registration]
		}

		float xData;
		float yData;
		float zData;
		float rgbData;

		for(int i=0;i<height;i++)
		for(int j=0;j<width;j++) {
			int index=i*width+j+1;
			registration->getPointXYZRGB(&undistorted,&registered,i,j,xData,yData,zData,rgbData);
			const uint8_t *p=reinterpret_cast<uint8_t*>(&rgbData);

			if(zData>8) zData=8;
			else if(zData<0) zData=0;
			pointCloudBuffer.z[index]=(uint8_t)(zData*32);
			pointCloudBuffer.r[index]=p[2];
			pointCloudBuffer.g[index]=p[1];
            pointCloudBuffer.b[index]=p[0];
			//cout<<pointCloudBuffer.z[index]<<endl;
		}

		// Send depth buffer to server
		if(send(sockfd,&pointCloudBuffer.z,packetSize,0)<0) {
			cout << endl;
			cout << "Error writing to socket!" << endl;
			dev->stop();
  			dev->close();
		}
		usleep(8000);

		// Send color buffer to server
		if(send(sockfd,&pointCloudBuffer.r,packetSize,0)<0) {
			cout << endl;
			cout << "Error writing to socket!" << endl;
			dev->stop();
  			dev->close();
		}
		usleep(8000);

		// Send color buffer to server
		if(send(sockfd,&pointCloudBuffer.g,packetSize,0)<0) {
			cout << endl;
			cout << "Error writing to socket!" << endl;
			dev->stop();
  			dev->close();
		}
		usleep(8000);

		// Send color buffer to server
		if(send(sockfd,&pointCloudBuffer.b,packetSize,0)<0) {
			cout << endl;
			cout << "Error writing to socket!" << endl;
			dev->stop();
  			dev->close();
		}
		usleep(8000);

		framecount++;
		if (!viewer_enabled)
		{
		if (framecount % 100 == 0)
			std::cout << "The viewer is turned off. Received " << framecount << " frames. Ctrl-C to stop." << std::endl;
		listener.release(frames);
		continue;
		}

#ifdef EXAMPLES_WITH_OPENGL_SUPPORT
		if (enable_rgb)
		{
			viewer.addFrame("RGB", rgb);
		}
		if (enable_depth)
		{
			viewer.addFrame("ir", ir);
			viewer.addFrame("depth", depth);
		}
		if (enable_rgb && enable_depth)
		{
			viewer.addFrame("registered", &registered);
		}

		protonect_shutdown = protonect_shutdown || viewer.render();
#endif

		listener.release(frames);
	}

	dev->stop();
  	dev->close();

	delete registration;

	return 0;
}

void sigint_handler(int s) {
	protonect_shutdown = true;
}

/**
 * This function displays error in case of network faliure
 **/
void error(const char *msg) {
    perror(msg);
    exit(0);
}