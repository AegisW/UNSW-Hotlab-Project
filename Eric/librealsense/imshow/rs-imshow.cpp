// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2017 Intel Corporation. All Rights Reserved.

#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include <opencv2/opencv.hpp>   // Include OpenCV API

// OpenCV includes
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <string>
#include <map>
#include <algorithm>
#include <mutex>                    // std::mutex, std::lock_guard
#include <cmath>                    // std::ceil
#include <thread>
#include <atomic>

// Standard includes
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// TCPIP includes
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

using namespace cv;
using namespace std;

const std::string no_camera_message = "No camera connected, please connect 1 or more";
const std::string platform_camera_name = "Platform Camera";
const int width = 640;
const int height = 480;
const int cut = 107200;
const int packetSize = width*height+1-cut;
const int waitTime = 10000;

// Buffer structure
struct myPointCloudBuffer {
    uint8_t z[packetSize];
    uint8_t r[packetSize];
    uint8_t g[packetSize];
    uint8_t b[packetSize];
} pointCloudBuffer;

// Initialize socket parameters
int sockfd1, sockfd2;
int portno1, portno2;
struct sockaddr_in serv_addr1, serv_addr2;
struct hostent *server1, *server2;

// OpenCV funtions
void printHelp();

// Error message for networking
void error(const char *msg);

class device_container
{
    // Helper struct per pipeline
    struct view_port
    {
        std::map<int, rs2::frame> frames_per_stream;
        rs2::colorizer colorize_frame;
        rs2::pipeline pipe;
        rs2::pipeline_profile profile;
    };

public:

    void enable_device(rs2::device dev)
    {
        std::string serial_number(dev.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER));
        std::lock_guard<std::mutex> lock(_mutex);

        if (_devices.find(serial_number) != _devices.end())
        {
            return; //already in
        }

        // Ignoring platform cameras (webcams, etc..)
        if (platform_camera_name == dev.get_info(RS2_CAMERA_INFO_NAME))
        {
            return;
        }
        // Create a pipeline from the given device
        rs2::pipeline p;
        rs2::config c;
        c.enable_device(serial_number);
        c.enable_stream(RS2_STREAM_DEPTH,width,height,RS2_FORMAT_Z16);
        c.enable_stream(RS2_STREAM_COLOR,width,height,RS2_FORMAT_BGR8);

        // Start the pipeline with the configuration
        rs2::pipeline_profile profile = p.start(c);
        // Hold it internally
        _devices.emplace(serial_number, view_port{ {},{}, p, profile });
    }

    void remove_devices(const rs2::event_information& info)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        // Go over the list of devices and check if it was disconnected
        auto itr = _devices.begin();
        while(itr != _devices.end())
        {
            if (info.was_removed(itr->second.profile.get_device()))
            {
                itr = _devices.erase(itr);
            }
            else
            {
                ++itr;
            }
        }
    }

    size_t device_count()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        return _devices.size();
    }

    int stream_count()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        int count = 0;
        for (auto&& sn_to_dev : _devices)
        {
            for (auto&& stream : sn_to_dev.second.frames_per_stream)
            {
                if (stream.second)
                {
                    count++;
                }
            }
        }
        return count;
    }

    void poll_frames()
    {
        std::lock_guard<std::mutex> lock(_mutex);

        // Go over all device
        for (auto&& view : _devices)
        {
            // Ask each pipeline if there are new frames available
            rs2::frameset frameset;
            if (view.second.pipe.poll_for_frames(&frameset))
            {
                for (int i = 0; i < frameset.size(); i++)
                {
                    rs2::frame new_frame = frameset[i];
                    int stream_id = new_frame.get_profile().unique_id();
                    view.second.frames_per_stream[stream_id] = new_frame;//view.second.colorize_frame(new_frame); //update view port with the new stream
                }
            }
        }
    }

    void render_textures(int sockfd1, int sockfd2, rs2::spatial_filter spat_filter)
    {
        rs2::frame depthFrame;
        rs2::frame colorFrame;
        //spat_filter.set_option(RS2_OPTION_FILTER_MAGNITUDE, 5);
        //spat_filter.set_option(RS2_OPTION_FILTER_SMOOTH_ALPHA, 0.65);
        //spat_filter.set_option(RS2_OPTION_FILTER_SMOOTH_DELTA, 29);
        //temp_filter.set_option(RS2_OPTION_FILTER_SMOOTH_ALPHA, 0.1);
        //temp_filter.set_option(RS2_OPTION_FILTER_SMOOTH_DELTA, 100);

        rs2::pointcloud pc;
        rs2::points points;

        float depth_in_meters=0.0;
        
        std::lock_guard<std::mutex> lock(_mutex);
        int stream_no = 0;
        int dev_no = 0;
        bool isColor = true;
        std::string window_name[4]={"Depth1","Color1","Depth2","Color2"};

        for (auto&& view : _devices)
        {
            auto sensor = view.second.profile.get_device().first<rs2::depth_sensor>();
            auto depth_scale = sensor.get_depth_scale();
            //cout<<depth_scale<<endl;

            // For each device get its frames
            for (auto&& id_to_frame : view.second.frames_per_stream)
            {
                namedWindow(window_name[stream_no], WINDOW_NORMAL);
                resizeWindow(window_name[stream_no],640,480);
                // If the frame is available
                if (id_to_frame.second)
                {
                    int w = id_to_frame.second.as<rs2::video_frame>().get_width();
                    int h = id_to_frame.second.as<rs2::video_frame>().get_height();

                    // Process depth frames
                    if(stream_no%2==0)
                    {
                        // Only depth frames are processed
                        //rs2::frame processFrame;
                        depthFrame = id_to_frame.second;
                        //depthFrame = spat_filter.process(depthFrame);
                        //depthFrame = temp_filter.process(depthFrame);
                        points=pc.calculate(depthFrame);

                        //Mat depthMat(Size(w, h), CV_16U, (void*)depthFrame.get_data(), Mat::AUTO_STEP);
                        //Mat original(Size(w, h), CV_16U, (void*)id_to_frame.second.get_data(), Mat::AUTO_STEP);
                        //depthMat.convertTo(depthMat, CV_8UC1, depth_scale*25.5);
                        //original.convertTo(original, CV_8UC1, depth_scale*25.5);
                        //imshow(window_name[stream_no], depthMat); 
                        //imshow("Original", original);

                        /*for(int i=0;i<depthMat.size().area()-cut;i++)
                        {
                            pointCloudBuffer.z[i+1]=(uint8_t)depthMat.data[i];
                            if(pointCloudBuffer.z[i+1]>128)
                            {
                                pointCloudBuffer.z[i+1]=0;
                                outOfRange=true;
                            }
                        }*/

                        // Get frame pointer
                        /*if(dev_no==0)
                        {
                            // Send depth to server 1
                            if(send(sockfd1,&pointCloudBuffer.z,packetSize,0)<0) {
                                cout << endl;
                                cout << "Error writing to socket!" << endl;
                            }
                            usleep(6000);
                        }
                        else if(dev_no==1)
                        {
                            // Send depth to server 2
                            if(send(sockfd2,&pointCloudBuffer.z,packetSize,0)<0) {
                                cout << endl;
                                cout << "Error writing to socket!" << endl;
                            }
                            usleep(6000);
                        }*/
                    }
                    // Process color frames
                    else
                    {
                        colorFrame = id_to_frame.second;

                        if(!colorFrame)
                        {
                            colorFrame=view.second.colorize_frame(depthFrame);
                        }
                        
                        pc.map_to(colorFrame);
                        auto verts=points.get_vertices();

                        Mat colorMat(Size(w, h), CV_8UC3, (void*)colorFrame.get_data(), Mat::AUTO_STEP);
                        //imshow(window_name[stream_no], colorMat);

                        for(int i=0, j=0;i<points.size()-cut;i++, j=j+3)
                        {
                            // Get buffer index
                            //int index=i*width+j+1;
                            if(verts[i].z<=2 && verts[i].z>0)
                            {
                                pointCloudBuffer.z[i+1]=(uint8_t)(verts[i].z*127);
                                pointCloudBuffer.r[i+1]=colorMat.data[j+2];
                                pointCloudBuffer.g[i+1]=colorMat.data[j+1];
                                pointCloudBuffer.b[i+1]=colorMat.data[j];
                                //cout<<depth_scale/verts[i].z*255<<endl;
                            }
                            else
                            {
                                pointCloudBuffer.z[i+1]=0;
                                pointCloudBuffer.r[i+1]=0;
                                pointCloudBuffer.g[i+1]=0;
                                pointCloudBuffer.b[i+1]=0;
                            }
                        }

                        // Get frame pointer
                        //const uint16_t* p_depth_data = reinterpret_cast<const uint16_t*>(id_to_frame.second.get_data());
                        //float zData;
                        if(dev_no==0)
                        {
                            // Send depth to server 1
                            if(send(sockfd1,&pointCloudBuffer.z,packetSize,0)<0) {
                                cout << endl;
                                cout << "Error writing to socket!" << endl;
                            }
                            usleep(waitTime);

                            // Send color buffer to server1
                            if(send(sockfd1,&pointCloudBuffer.r,packetSize,0)<0) {
                                cout << endl;
                                cout << "Error writing to socket!" << endl;
                            }
                            usleep(waitTime);

                            // Send color buffer to server1
                            if(send(sockfd1,&pointCloudBuffer.g,packetSize,0)<0) {
                                cout << endl;
                                cout << "Error writing to socket!" << endl;
                            }
                            usleep(waitTime);

                            // Send color buffer to server1
                            if(send(sockfd1,&pointCloudBuffer.b,packetSize,0)<0) {
                                cout << endl;
                                cout << "Error writing to socket!" << endl;
                            }
                            usleep(waitTime);
                        }
                        else if(dev_no==1)
                        {
                            // Send depth to server 2
                            if(send(sockfd2,&pointCloudBuffer.z,packetSize,0)<0) {
                                cout << endl;
                                cout << "Error writing to socket!" << endl;
                            }
                            usleep(waitTime);

                            // Send color buffer to server2
                            if(send(sockfd2,&pointCloudBuffer.r,packetSize,0)<0) {
                                cout << endl;
                                cout << "Error writing to socket!" << endl;
                            }
                            usleep(waitTime);

                            // Send color buffer to server2
                            if(send(sockfd2,&pointCloudBuffer.g,packetSize,0)<0) {
                                cout << endl;
                                cout << "Error writing to socket!" << endl;
                            }
                            usleep(waitTime);

                            // Send color buffer to server2
                            if(send(sockfd2,&pointCloudBuffer.b,packetSize,0)<0) {
                                cout << endl;
                                cout << "Error writing to socket!" << endl;
                            }
                            usleep(waitTime);
                        }
                    }
                    stream_no++;
                }
            }
            dev_no++;
        }
    }
private:
    std::mutex _mutex;
    std::map<std::string, view_port> _devices;
};

int main(int argc, char * argv[]) try
{
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
    if (server1 == NULL) {
        fprintf(stderr, "Error, no host1!\n");
        exit(0);
    } 
    server2 = gethostbyname("149.171.37.126");
    if (server2 == NULL) {
        fprintf(stderr, "Error, no host2!\n");
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

    // Display help
    printHelp();

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

    // Declere filters
    rs2::spatial_filter spat_filter;
    rs2::temporal_filter temp_filter;

    // Initialize realsense devices
    device_container connected_devices;

    rs2::context ctx;    // Create librealsense context for managing devices

    // Register callback for tracking which devices are currently connected
    ctx.set_devices_changed_callback([&](rs2::event_information& info)
    {
        connected_devices.remove_devices(info);
        for (auto&& dev : info.get_new_devices())
        {
            connected_devices.enable_device(dev);
        }
    });

    // Initial population of the device list
    for (auto&& dev : ctx.query_devices()) // Query the list of connected RealSense devices
    {
        connected_devices.enable_device(dev);
    }

    while (waitKey(1) < 0)
    {
        connected_devices.poll_frames();
        auto total_number_of_streams = connected_devices.stream_count();
        if (total_number_of_streams == 0)
        {
            cout<<"No camera connected!"<<endl;
            continue;
        }
        connected_devices.render_textures(sockfd1, sockfd2, spat_filter);
    }

    return EXIT_SUCCESS;
}
catch (const rs2::error & e)
{
    std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
    return EXIT_FAILURE;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}

/**
* This function displays help in console
**/
void printHelp() {
    cout << endl;
    cout << "*************************************" << endl;
    cout << "** Press 'ESC' to quit application **" << endl;
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
