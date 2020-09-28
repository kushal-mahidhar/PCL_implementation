#include <iostream>
#include <opencv2/core.hpp>
#include <pcl/io/ply_io.h>
#include <pcl/point_types.h>
#include <pcl/visualization/cloud_viewer.h>
#include <string>
#include <math.h>
#include <vector>


void createCloudFromImage(cv::Mat &image, cv::Mat &indices, cv::Mat &depth);
