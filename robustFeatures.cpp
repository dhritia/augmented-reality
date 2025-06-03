/*
Dhriti Anjaria
Date: March 2, 2025
robustFeatures: This file displays video stream and corners in the pattern found using Harris features.
*/
#include <cstdio>
#include <cstring>
#include <cmath>
#include <opencv2/opencv.hpp> // the top include file
#include "processing.h"

// reading an image (path on command line), modifying it
int main(int argc, char *argv[]) {
  cv::VideoCapture *capdev;
  // open the video device
  capdev = new cv::VideoCapture(0);
  if( !capdev->isOpened() ) {
    printf("Unable to open video device\n");
    return(-1);
  }
  cv::Mat frame, new_frame, gray, dst, dst_norm;
  // get some properties of the image
  cv::Size refS( (int) capdev->get(cv::CAP_PROP_FRAME_WIDTH ),(int) capdev->get(cv::CAP_PROP_FRAME_HEIGHT));
  printf("Expected size: %d %d\n", refS.width, refS.height);
  cv::namedWindow("Video", 1); // identifies a window
  for(;;) {
    *capdev >> frame; // get a new frame from the camera, treat as a stream
    if( frame.empty() ) {
      printf("frame is empty\n");
      break;
    }
    cv::imshow("Original Video", frame);
    frame.copyTo(new_frame);
    cv::cvtColor(frame, gray, cv::COLOR_RGB2GRAY);
    cv::cornerHarris(gray, dst, 3, 3, 0.04);
    // Normalizing the range of values to 0-255
    cv::normalize(dst, dst_norm, 0, 255, cv::NORM_MINMAX, CV_32FC1);
    for (int i = 0; i < dst_norm.rows; i++) {
        for (int j = 0; j < dst_norm.cols; j++) {
            // Selecting only corners over a certain threshold
            if (dst_norm.at<float>(i, j) > 70) { 
                cv::circle(new_frame, cv::Point(j, i), 5, cv::Scalar(0, 0, 255), 2);
            }
        }
    }
    cv::imshow("Harris Corners", new_frame);
    char key = cv::waitKey(10);
    if( key == 'q') {
      break;
    }
  }
  delete capdev;
  return(0);
}