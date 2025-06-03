/*
Dhriti Anjaria
Date: March 2, 2025
ardisplay: This file loads the calibrations for the camera and then displays virtual objects.
*/
#include <cstdio>
#include <cstring>
#include <cmath>
#include <opencv2/opencv.hpp> // the top include file
#include "processing.h"

// Draws a barn shaped structure onto the screen
int drawShape(cv::Mat &new_frame, std::vector<cv::Point2f> &projected_shape) {
  std::vector<std::vector<int>> lines = {
    {0, 1}, {1, 2}, {2, 3}, {3, 0}, {0, 4}, {1, 5}, {2, 6}, {3, 7}, {5, 6}, {7, 4}, {4, 8}, {5, 8}, {6, 9}, {7, 9}, {8, 9}
  };
  for (int i = 0; i < lines.size(); i++) {
    cv::line(new_frame, projected_shape[lines[i][0]], projected_shape[lines[i][1]], cv::Scalar(244, 194, 194), 2);
  }
  return 0;
}


// reading an image (path on command line), modifying it
int main(int argc, char *argv[]) {

  std::vector<cv::Point2f> corner_set;
  std::vector<std::vector<cv::Point2f>> corner_list;  
  std::vector<std::vector<cv::Vec3f>> point_list;    
  std::vector<cv::Vec3f> point_set; 
  cv::Mat frame, new_frame, camera_matrix, distortion_coeffs;
  cv::Mat rot = cv::Mat::zeros(3, 1, CV_64F);
  cv::Mat trans = cv::Mat::zeros(3, 1, CV_64F);

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

  camera_matrix = cv::Mat::zeros(3, 3, CV_64F);
  distortion_coeffs = cv::Mat::zeros(1, 5, CV_64F);
  getParamsFromFile(camera_matrix, distortion_coeffs);
  // Defining the world coordinates for the virtual objects to be displayed on screen
  std::vector<cv::Point3f> axis = {{0, 0, 0}, {2, 0, 0}, {0, 2, 0}, {0, 0, 2}};
  std::vector<cv::Point3f> shape_points = {
    {3, -1, 1}, 
    {5, -1, 1}, 
    {5, -3, 1}, 
    {3, -3, 1},
    {3, -1, 3}, 
    {5, -1, 3}, 
    {5, -3, 3}, 
    {3, -3, 3},
    {4, -1, 4}, 
    {4, -3, 4}
  };
  std::vector<cv::Point2f> projected_points, projected_shape;
  generate3DWorldPoints(point_set);
  for(;;) {
    *capdev >> frame; // get a new frame from the camera, treat as a stream
    if( frame.empty() ) {
      printf("frame is empty\n");
      break;
    }
    cv::imshow("Original Video", frame);
    cv::Size image_size(frame.cols, frame.rows);
    bool f = extractCorners(frame, new_frame, corner_set, false);
    if (f) {
      // Get camera's position
      cv::solvePnP(point_set, corner_set, camera_matrix, distortion_coeffs, rot, trans, false, cv::SOLVEPNP_ITERATIVE);
      /*
      std::cout<<"Rotation:"<<std::endl;
      std::cout<<rot<<std::endl;
      std::cout<<"Translation:"<<std::endl;
      std::cout<<trans<<std::endl;
      */
      // Project 3D points of the axes onto screen
      cv::projectPoints(axis, rot, trans, camera_matrix, distortion_coeffs, projected_points);
      cv::line(new_frame, projected_points[0], projected_points[1], cv::Scalar(0, 255, 255), 5);  
      cv::line(new_frame, projected_points[0], projected_points[2], cv::Scalar(255, 255, 0), 5);  
      cv::line(new_frame, projected_points[0], projected_points[3], cv::Scalar(255, 0, 255), 5);
      cv::projectPoints(shape_points, rot, trans, camera_matrix, distortion_coeffs, projected_shape);
      // Draw the barn shaped object on the screen
      drawShape(new_frame, projected_shape);
      cv::imshow("Virtual Objects", new_frame);
    }
    // see if there is a waiting keystroke
    char key = cv::waitKey(10);
    if (key == 'q') {
      cv::imwrite("../shape.png", new_frame);
      break;
    }
  }
  delete capdev;
  return(0);
}