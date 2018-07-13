///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2017, STEREOLABS.
//
// All rights reserved.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////


/******************************************************************************************************************
 ** This sample demonstrates how to use two ZEDs with the ZED SDK, each grab are in a separated thread            **
 ** This sample has been tested with 3 ZEDs in HD720@30fps resolution                                             **
 ** This only works for Linux                                                                                     **
 *******************************************************************************************************************/

// Standard includes
#include <stdio.h>
#include <string.h>
#include <string.h>
#include <unistd.h>
#include <ctime>
#include <chrono>
#include <thread>

// OpenCV includes
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// TCP/IP includes
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

// ZED includes
#include <sl/Camera.hpp>

using namespace std;
using namespace sl;

// Camera parameters
const int NUM_CAMERAS = 2;
const int FPS = 15;
const sl::RESOLUTION ZED_RES = sl::RESOLUTION_HD720;

// Create ZED objects
sl::Camera* zed[NUM_CAMERAS];
sl::Mat point_cloud1;
sl::Mat point_cloud2;
cv::Mat SbSResult[NUM_CAMERAS];
cv::Mat ZED_LRes[NUM_CAMERAS];
int width, height;
long long ZED_Timestamp[NUM_CAMERAS];
bool stop_signal;

// Initialize socket parameters
int sockfd1, sockfd2;
int portno1, portno2;
struct sockaddr_in serv_addr1, serv_addr2;
struct hostent *server1, *server2;

// ZED functions
void grab_run(int x);

// OpenCV funtions
void slMat2cvMat(Mat& input, int x);
void printHelp();

// Error message for networking
void error(const char *msg);

// Color conversion
union myColor{
    float a;
    uint8_t b[4];
} pclColor;

// Clamp function
uint8_t clamp(float input);

int main(int argc, char **argv) {

#ifdef WIN32
    std::cout << "Multiple ZEDs are not available under Windows" << std::endl;
    return -1;
#endif

    // Establish socket and connection
    portno1 = 8888;
    portno2 = 8890;

    sockfd1 = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd1 < 0) error("Error opening socket1!");
    // Make socket reusable
    int flag = 1;
    if (setsockopt(sockfd1, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) < 0) error("Error setting socket option!");

    sockfd2 = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd2 < 0) error("Error opening socket2!");

    server1 = gethostbyname("149.171.37.126");
    //server = gethostbyname("127.0.0.1");
    if (server1 == NULL) {
        fprintf(stderr, "Error, no host1!\n");
        exit(0);
    } 
    server2 = gethostbyname("149.171.37.126");
    if (server2 == NULL) {
        fprintf(stderr, "Econnect(sockfd2,(struct sockaddr *)&serv_addr2,sizeof(serv_addr2)) < 0rror, no host2!\n");
    }               

    bzero((char *)&serv_addr1, sizeof(serv_addr1));
    bzero((char *)&serv_addr2, sizeof(serv_addr2));

    serv_addr1.sin_family = AF_INET;
    serv_addr2.sin_family = AF_INET;

    bcopy((char *)server1->h_addr, (char *)&serv_addr1.sin_addr.s_addr, server1->h_length);
    bcopy((char *)server2->h_addr, (char *)&serv_addr2.sin_addr.s_addr, server2->h_length);

    serv_addr1.sin_port = htons(portno1);
    serv_addr2.sin_port = htons(portno2);
    if (connect(sockfd1,(struct sockaddr *)&serv_addr1,sizeof(serv_addr1)) < 0) error("Error connecting1!");
    if (connect(sockfd2,(struct sockaddr *)&serv_addr2,sizeof(serv_addr2)) < 0) error("Error connecting2!");

    cout << endl;
    cout << "*************************************" << endl;
    cout << "**     Connection established!     **" << endl;
    cout << "*************************************" << endl;
    cout << endl;

    // Set configuration parameters for the ZED
    InitParameters params;
    params.depth_mode = DEPTH_MODE_PERFORMANCE;
    params.camera_resolution = ZED_RES;
    params.camera_fps = FPS;
    params.coordinate_system = COORDINATE_SYSTEM_RIGHT_HANDED_Y_UP;
    params.coordinate_units = UNIT_METER;

    // Create every ZED and init them
    for (int i = 0; i < NUM_CAMERAS; i++) {
        zed[i] = new sl::Camera();
        params.camera_linux_id = i;

        ERROR_CODE err = zed[i]->open(params);

        //cout << "ZED no. " << i << " -> Result : " << sl::errorCode2str(err) << endl;
        if (err != SUCCESS) {
            delete zed[i];
            return 1;
        }

        width = 512;//zed[i]->getResolution().width;
        height = 288;//zed[i]->getResolution().height;
        //SbSResult[i] = cv::Mat(height,width*2,CV_8UC4,1);
    }

    // Display help
    printHelp();

    char key = ' ';

    // Create each grabbing thread with the camera number as parameters
    std::vector<std::thread*> thread_vec;
    for (int i = 0; i < NUM_CAMERAS; i++)
        thread_vec.push_back(new std::thread(grab_run, i));

    // Create windows for viewing results with OpenCV
    //cv::Size DisplaySize(width, height);

    //for (int i = 0; i < NUM_CAMERAS; i++)20
    //    ZED_LRes[i] = cv::Mat(DisplaySize, CV_8UC4);

    // Loop until 'q' is pressed
    while (key != 'q') {
        // Resize and imshow
        /*for (int i = 0; i < NUM_CAMERAS; i++) {
            char wnd_name[21];
            sprintf(wnd_name, "ZED no. %d", i);
            ZED_LRes[i] = cv::Mat(DisplaySize, CV_8UC4);
            cv::resize(SbSResult[i], ZED_LRes[i], DisplaySize);
            cv::imshow(wnd_name, ZED_LRes[i]);
        }*/

        // Compare Timestamp between both camera (uncomment following line)
        //for (int i = 0; i < NUM_CAMERAS; i++) std::cout << " Timestamp " << i << ": " << ZED_Timestamp[i] << std::endl;

        key = cv::waitKey(5);
    }

    // Send the signal to stop every threads to finish
    stop_signal = true;

    // Wait for every thread to be stopped
    for (auto it : thread_vec) it->join();

    // Free buffer
    point_cloud1.free(MEM_CPU);
    point_cloud2.free(MEM_CPU);

    // Close all sockets
    close(sockfd1);
    close(sockfd2);

    return 0;
}

// Thread for camera grabbing
void grab_run(int x) {
    //sl::Mat depthImage(width, height, MAT_TYPE_8U_C4);
    sl::Mat depthMap1, depthMap2;

    sl::RuntimeParameters rt_params;

    // Initialize buffer for depth and color values transmission
    int packetSize = 512*288+1;
    //uint8_t depthBufferx[packetSize];
    //uint8_t depthBuffery[packetSize];
    uint8_t depthBufferz[packetSize];
    uint8_t colorBufferR[packetSize];
    uint8_t colorBufferG[packetSize];
    uint8_t colorBufferB[packetSize];
    //bzero(depthBufferx,packetSize);
    //bzero(depthBuffery,packetSize);
    bzero(depthBufferz,packetSize);
    bzero(colorBufferR,packetSize);
    bzero(colorBufferG,packetSize);
    bzero(colorBufferB,packetSize);

    while (!stop_signal) {
        //rt_params.sensing_mode = SENSING_MODE_FILL;
        sl::ERROR_CODE res = zed[x]->grab(rt_params);

        if (res == sl::SUCCESS) {
            //ZED_Timestamp[x] = zed[x]->getCameraTimestamp();

            // Retrieve depth image and RGBA pointcloud in CPU memory for each ZED camera
            //zed[x]->retrieveImage(depthImage, VIEW_DEPTH, MEM_CPU);
            //slMat2cvMat(depthImage, x);

            // Initialize temp values for depth and color
            float depthValue=0;
            float colorValue=0;
            sl::float4 point3D;
            int stride = packetSize;
            int count=1;

            // Initialize header for buffers
            //depthBufferx[0]='x';
            //depthBuffery[0]='y';
            depthBufferz[0]='z';
            colorBufferR[0]='r';
            colorBufferG[0]='g';
            colorBufferB[0]='b';
            
            if (x==0) {
                zed[x]->retrieveMeasure(point_cloud1, MEASURE_XYZRGBA, MEM_CPU, width, height);
                zed[x]->retrieveMeasure(depthMap1, MEASURE_DEPTH, MEM_CPU, width, height);

                // Write depth value into depth buffer
                for (int j=0; j<height; j++)
                for (int i=0; i<width; i++) {    
                    // initialize buffer index
                    int count = i + width * j + 1;

                    // Extract points for moving objects
                    //if(count<stride) { 
                        // Grab depth and point cloud values
                        depthMap1.getValue(i,j,&depthValue);
                        point_cloud1.getValue(i,j,&point3D);

                        if(depthValue>10) depthValue=10;
                        else if(depthValue<0) depthValue=0;

                        // Get point cloud color
                        pclColor.a=point3D.w;

                        // Write depth values into buffer
                        //depthBufferx[count]=point3D.x*12.75+128;
                        //depthBuffery[count]=point3D.y*12.75+128;
                        depthBufferz[count]=(uint8_t)(depthValue*25);
                        
                        colorBufferR[count]=pclColor.b[0];
                        colorBufferG[count]=pclColor.b[1];
                        colorBufferB[count]=pclColor.b[2];

                        //count++;
                    //}
                }
                // Send depth buffer to server
                /*if(send(sockfd1,&depthBufferx,packetSize,0)<0) {
                    cout << endl;
                    cout << "Error writing to socket!" << endl;
                    stop_signal=true;
                }
                sl::sleep_ms(12);

                // Send depth buffer to server
                if(send(sockfd1,&depthBuffery,packetSize,0)<0) {
                    cout << endl;
                    cout << "Error writing to socket!" << endl;
                    stop_signal=true;
                }
                sl::sleep_ms(12);*/

                // Send depth buffer to serverG
                if(send(sockfd1,&depthBufferz,packetSize,0)<0) {
                    cout << endl;
                    cout << "Error writing to socket!" << endl;
                    stop_signal=true;
                }
                sl::sleep_ms(12);

                // Send color buffer to server
                if(send(sockfd1,&colorBufferR,packetSize,0)<0) {
                    cout << endl;
                    cout << "Error writing to socket!" << endl;
                    stop_signal=true;
                }
                sl::sleep_ms(12);

                // Send color buffer to server
                if(send(sockfd1,&colorBufferG,packetSize,0)<0) {
                    cout << endl;
                    cout << "Error writing to socket!" << endl;
                    stop_signal=true;
                }
                sl::sleep_ms(12);

                // Send color buffer to server
                if(send(sockfd1,&colorBufferB,packetSize,0)<0) {
                    cout << endl;
                    cout << "Error writing to socket!" << endl;
                    stop_signal=true;
                }
                sl::sleep_ms(12);
            }
            else if (x==1) {
                zed[x]->retrieveMeasure(point_cloud2, MEASURE_XYZRGBA, MEM_CPU, width, height);
                zed[x]->retrieveMeasure(depthMap2, MEASURE_DEPTH, MEM_CPU, width, height);
                int count=1;
                int stride = packetSize;

                // Write depth value into depth buffer
                for (int j=0; j<height; j++)
                for (int i=0; i<width; i++) {    
                    // initialize buffer index
                    int count = i + width * j + 1;

                    // Extract points for moving objec2ts
                    //if(count<stride) { !
                        // Grab depth and point cloud values
                        depthMap2.getValue(i,j,&depthValue);
                        point_cloud2.getValue(i,j,&point3D);

                        if(depthValue>10) depthValue=10;
                        else if(depthValue<0) depthValue=0;

                        // Get point cloud color
                        pclColor.a=point3D.w;

                        // Write depth values into buffer
                        //depthBufferx[count]=point3D.x*12.75+128;
                        //depthBuffery[count]=point3D.y*12.75+128;
                        depthBufferz[count]=(uint8_t)(depthValue*25);
                        
                        colorBufferR[count]=pclColor.b[0];
                        colorBufferG[count]=pclColor.b[1];
                        colorBufferB[count]=pclColor.b[2];

                        //count++;
                    //}
                }
                // Send depth buffer to server
                /*if(send(sockfd2,&depthBufferx,packetSize,0)<0) {
                    cout << endl;
                    cout << "Error writing to socket!" << endl;
                    stop_signal=true;
                }
                sl::sleep_ms(12);

                // Send depth buffer to server
                if(send(sockfd2,&depthBuffery,packetSize,0)<0) {
                    cout << endl;
                    cout << "Error writing to socket!" << endl;
                    stop_signal=true;
                }
                sl::sleep_ms(12);*/

                // Send depth buffer to serverG
                if(send(sockfd2,&depthBufferz,packetSize,0)<0) {
                    cout << endl;
                    cout << "Error writing to socket!" << endl;
                    stop_signal=true;
                }
                sl::sleep_ms(12);

                // Send color buffer to server
                if(send(sockfd2,&colorBufferR,packetSize,0)<0) {
                    cout << endl;
                    cout << "Error writing to socket!" << endl;
                    stop_signal=true;
                }
                sl::sleep_ms(12);

                // Send color buffer to server
                if(send(sockfd2,&colorBufferG,packetSize,0)<0) {
                    cout << endl;
                    cout << "Error writing to socket!" << endl;
                    stop_signal=true;
                }
                sl::sleep_ms(12);

                // Send color buffer to server
                if(send(sockfd2,&colorBufferB,packetSize,0)<0) {
                    cout << endl;
                    cout << "Error writing to socket!" << endl;
                    stop_signal=true;
                }
                sl::sleep_ms(12);
            }
        }
        sl::sleep_ms(1);
    }
    zed[x]->close();
    delete zed[x];
}

/**
* Conversion function between sl::Mat and cv::Mat
**/
void slMat2cvMat(Mat& input, int x) {
    // Mapping between MAT_TYPE and CV_TYPE
    int cv_type = -1;
    switch (input.getDataType()) {
	    case MAT_TYPE_32F_C1: cv_type = CV_32FC1; break;
        case MAT_TYPE_32F_C2: cv_type = CV_32FC2; break;
        case MAT_TYPE_32F_C3: cv_type = CV_32FC3; break;
        case MAT_TYPE_32F_C4: cv_type = CV_32FC4; break;
        case MAT_TYPE_8U_C1: cv_type = CV_8UC1; break;
        case MAT_TYPE_8U_C2: cv_type = CV_8UC2; break;
        case MAT_TYPE_8U_C3: cv_type = CV_8UC3; break;
        case MAT_TYPE_8U_C4: cv_type = CV_8UC4; break;
	    default: break;
    }
    // Since cv::Mat data requires a uchar* pointer, we get the uchar1 pointer from sl::Mat (getPtr<T>())
    // cv::Mat and sl::Mat will share a single memory structure
    cv::Mat(input.getHeight(), input.getWidth(), cv_type, input.getPtr<sl::uchar1>(MEM_CPU)).copyTo(SbSResult[x]);
}

/**
* This function displays help in console
**/
void printHelp() {
    cout << endl;
    cout << "*************************************" << endl;
    cout << "**  Press 'q' to quit application  **" << endl;
    cout << "*************************************" << endl;
    cout << endl;
}

/**
 * This function displays error in case of network faliure
 **/
void error(const char *msg) {
    perror(msg);
    exit(0);
}

/**
 * This function clamps values to between 0 and 255
 **/
uint8_t clamp(float input) {
    if(input>255) input=255;
    else if(input<0) input=0;
    return (uint8_t)input;
}