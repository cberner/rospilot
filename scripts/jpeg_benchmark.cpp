/*********************************************************************
 *
 * Copyright 2012 the original author or authors.
 * See the NOTICE file distributed with this work for additional
 * information regarding copyright ownership.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *    http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *********************************************************************/
#include<stdio.h>
#include<time.h>
#include<fstream>
#include<chrono>
#include<boost/filesystem.hpp>
#include<wordexp.h>
#include<dirent.h>
#include<sys/ioctl.h>
#include<fcntl.h>
#include<errno.h>
#include<mutex>

#include<ros/ros.h>
#include<rospilot/CaptureImage.h>
#include<rospilot/Resolutions.h>
#include<std_srvs/Empty.h>
#include<sensor_msgs/CompressedImage.h>

#include<background_image_sink.h>
#include<ptp.h>
#include<usb_camera.h>
#include<video_recorder.h>
#include<transcoders.h>
#include<resizer.h>
#include<h264_server.h>

extern "C" {
#include <linux/videodev2.h>
}

using namespace rospilot;

int main(int argc, char **argv)
{
    // XXX: Assumes that the input image 1600x1200
    PixelFormat pixelFormat = PIX_FMT_YUV420P;
    JpegDecoder *jpegDecoder = new JpegDecoder(1600, 1200, pixelFormat);

    sensor_msgs::CompressedImage image;
    image.format = "jpeg";

    std::string path(argv[1]);
    std::fstream f(path, std::ifstream::binary | std::ios_base::in);
    if (!f) {
        std::cout << "Failed to open " << path << std::endl;
        return 1;
    }
    f.seekg(0, f.end);
    int length = f.tellg();
    f.seekg(0, f.beg);
    char *buffer = new char[length];
    f.read(buffer, length);
    f.close();
    for (int i = 0; i < length; i++) {
        image.data.push_back(buffer[i]);
    }

    sensor_msgs::CompressedImage images[10];
    for (int i = 0; i < 10; i++) {
        images[i] = image;
    }

    int junk = 0;
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    for (int i = 0; i < 10; i++) {
        jpegDecoder->decodeInPlace(&images[i]);
        // Make sure there's a side effect
        junk += images[i].data.size();
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    double us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000000.0;
    std::cout << "FPS " << 10 / us << std::endl;

    delete buffer;
    delete jpegDecoder;
    return 0;
}
