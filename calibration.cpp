/*
Dhriti Anjaria
Date: March 2, 2025
calibration: This file calibrates the camera with respect to the chessboard pattern.
*/
#include <cstdio>
#include <cstring>
#include <cmath>
#include <opencv2/opencv.hpp> // the top include file
#include "processing.h"

// reading an image (path on command line), modifying it
int main(int argc, char *argv[]) {
  std::vector<cv::Point2f> corner_set;
  std::vector<std::vector<cv::Point2f>> corner_list;  
  std::vector<std::vector<cv::Vec3f>> point_list;    
  std::vector<cv::Vec3f> point_set; 
  cv::VideoCapture *capdev;
  // open the video device
  capdev = new cv::VideoCapture(1);
  if( !capdev->isOpened() ) {
    printf("Unable to open video device\n");
    return(-1);
  }
  // get some properties of the image
  cv::Size refS( (int) capdev->get(cv::CAP_PROP_FRAME_WIDTH ),(int) capdev->get(cv::CAP_PROP_FRAME_HEIGHT));
  printf("Expected size: %d %d\n", refS.width, refS.height);

  cv::namedWindow("Video", 1); // identifies a window
  cv::Mat frame, new_frame, camera_matrix, distortion_coeffs;
  std::vector<cv::Mat> rot, trans;
  for(;;) {
    *capdev >> frame; // get a new frame from the camera, treat as a stream
    if( frame.empty() ) {
      printf("frame is empty\n");
      break;
    }
    // Initializing camera matrix
    if (camera_matrix.empty()) {
      camera_matrix = (cv::Mat_<double>(3,3) << 1, 0, frame.cols / 2, 0, 1, frame.rows / 2, 0, 0, 1);
    }
    cv::imshow("Original Video", frame);
    cv::Size image_size(frame.cols, frame.rows);
    // Checking if the function was successfully able to extract corners and only then display them
    bool f = extractCorners(frame, new_frame, corner_set, true);
    if (f) {
      cv::imshow("Chessboard with Corners", new_frame);
    }
    // see if there is a waiting keystroke
    char key = cv::waitKey(10);
    // Save the points only if the corners were successfully found
    if (key == 's' && f) {
      std::cout<<"Number of corners:"<<corner_set.size()<<std::endl;
      std::cout<<"Coordinates:"<<std::endl<<"x: "<<corner_set[0].x<<std::endl<<"y: "<<corner_set[0].y<<std::endl;
      corner_list.push_back(corner_set);
      generate3DWorldPoints(point_set);
      point_list.push_back(point_set);
      // Automatically calibrate the camera once the user has selected 5 or more images
      if (point_list.size() >= 5) {
        std::cout<<"Before Calibration"<<std::endl;
        std::cout<<"Camera Matrix:\n"<<camera_matrix<<std::endl;
        std::cout<<"Distortion Coefficients:\n"<<distortion_coeffs<<std::endl;
        double error = cv::calibrateCamera(point_list, corner_list, image_size, camera_matrix, distortion_coeffs, rot, trans);
        std::cout<<"\nAfter calibration"<<std::endl;
        std::cout<<"Camera Matrix:\n"<<camera_matrix<<std::endl;
        std::cout<<"Distortion Coefficients:\n"<<distortion_coeffs<<std::endl;
        std::cout<<"Reprojection Error: "<<error<<" pixels"<<std::endl;
      }
    }
    // Save the camera matrix and distortion coeffs to a file
    if (key == 'w') {
      writeParamsToFile(camera_matrix, distortion_coeffs);
    }
    if( key == 'q') {
      cv::imwrite("../calibrate.png", new_frame);
      break;
    }
  }
  delete capdev;
  return(0);
}