/*
Dhriti Anjaria
Date: March 2, 2025
Processing: This file has functions to extract corners and read and write calibration parameters to file.
*/
#include <cstdio>
#include <cstring>
#include <opencv2/opencv.hpp>
#include <fstream>

/*
Extracts corners from a chessboard in the frame and returns true if corners were found.
*/
bool extractCorners(cv::Mat &src, cv::Mat &dst, std::vector<cv::Point2f>& corner_set, bool draw) {
  src.copyTo(dst);
  cv::Mat temp;
  cv::cvtColor(src, temp, cv::COLOR_RGB2GRAY);
  cv::Size patternsize(9,6); 
  cv::findChessboardCorners(src, patternsize, corner_set);
  // If the corners were found draw them
  if (corner_set.size() > 0) {
    // Draw the corners only if true was passes as a param
    if (draw) {
      cv::cornerSubPix(temp, corner_set, cv::Size(11, 11), cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.1));
      cv::drawChessboardCorners(dst, patternsize, cv::Mat(corner_set), 1);
    }
    return true;
  }
  return false;
}


/*
Creates a vector of 3D world points for all the corners found on chessboard.
*/
int generate3DWorldPoints(std::vector<cv::Vec3f> &point_set) {
  point_set.clear();
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 9; j++) {
      point_set.push_back(cv::Vec3f(j, -i, 0));
    }
  }
  return 0;
}

/*
Writes the calibration matrix and distortion coefficients to a file
*/
int writeParamsToFile(cv::Mat &camera_matrix, cv::Mat &distortion_coeffs) {
  std::cout << "Camera Matrix:\n" << camera_matrix << std::endl;
  std::cout << "Distortion Coefficients:\n" << distortion_coeffs << std::endl;
  std::ofstream file("calibration_results.txt", std::ios::trunc);
  file<<"Camera Matrix:\n";
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      file<<camera_matrix.at<double>(i, j)<<" ";
    }
  }
  file<<"\nDistortion Coefficients:\n";
  for (int i = 0; i < 5; i++) {
    file<<distortion_coeffs.at<double>(0, i)<<" ";
  }  
  file.close();
  std::cout<<"Calibration parameters saved to calibration_results.txt"<<std::endl;
  return 0;
}

/*
Loads the camera matrix and distortion coefficients from the file.
*/
int getParamsFromFile(cv::Mat &camera_matrix, cv::Mat &distortion_coeffs) {
  std::ifstream file("calibration_results.txt");
  std::string str;
  file>>str;  
  file>>str; 
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      file>>camera_matrix.at<double>(i, j);
    }
  }
  file>>str; 
  file>>str;  
  for (int i = 0; i < 5; i++) {
    file>>distortion_coeffs.at<double>(0, i);
  }
  file.close();
return 0;
}