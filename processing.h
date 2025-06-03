/*
Dhriti Anjaria
Date: February 15, 2025
Include file for functions to extract corners and manage calibration parameters.
*/
#ifndef PROC_H
#define PROC_H
int extractCorners(cv::Mat &src, cv::Mat &dst, std::vector<cv::Point2f>& corner_set, bool draw);
int generate3DWorldPoints(std::vector<cv::Vec3f> &point_set);
int writeParamsToFile(cv::Mat &camera_matrix, cv::Mat &distortion_coeffs);
int getParamsFromFile(cv::Mat &camera_matrix, cv::Mat &distortion_coeffs);
#endif