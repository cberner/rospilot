/*********************************************************************
 *
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2009, Robert Bosch LLC.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of the Robert Bosch nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 *********************************************************************/
#ifndef USB_CAM_USB_CAM_H
#define USB_CAM_USB_CAM_H

#include <string>
#include <sstream>
#include <vector>

typedef struct {
  int width;
  int height;
  int bytes_per_pixel;
  int image_size;
  char *image;
  int is_new;
} usb_cam_camera_image_t;

typedef enum {
  IO_METHOD_READ,
  IO_METHOD_MMAP,
  IO_METHOD_USERPTR,
} usb_cam_io_method;

// start camera
usb_cam_camera_image_t *usb_cam_camera_start(const char* dev, usb_cam_io_method io, uint32_t pf, int image_width, int image_height, int framerate);
// shutdown camera
void usb_cam_camera_shutdown(void);
// grabs a new image from the camera
void usb_cam_camera_grab_image(usb_cam_camera_image_t *image);
// grabs the raw bytes for an image
void usb_cam_camera_grab_raw(std::vector<unsigned char>* image);
// grabs the raw bytes for an mjpeg
void usb_cam_camera_grab_mjpeg(std::vector<unsigned char>* image);
// grabs the raw bytes for an h264 frame
void usb_cam_camera_grab_h264(std::vector<unsigned char>* image, bool *keyframe);
// enables/disable auto focus
void usb_cam_camera_set_auto_focus(int value);

#endif

