#include "cloud.h"


void createCloudFromImage(cv::Mat &image, cv::Mat &indices, cv::Mat &depth) {
	std::cout << " ----------------- \n" << std::endl;
	std::cout << indices.size().width << std::endl;
	std::cout << image.size() << std::endl;
	std::cout << depth.size() << std::endl;
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZRGB>);
	pcl::PointXYZRGB pt;
	int w = image.size().width;
	size_t length = indices.size().width;

	cv::Vec3b color;
	for(auto i=0; i<length; i++) {
		color = image.at<cv::Vec3b>(i/w, i%w); 
		pt.r = color[2]; pt.g = color[1]; pt.b = color[0];
		pt.x = indices.at<float>(0,i); pt.y = indices.at<float>(1,i);
		pt.z = depth.at<float>(0,i);
		cloud->push_back(pt);
	}
	std::cout << "Loaded "
			<< cloud->width * cloud->height
			<< " data points from test_pcd.pcd with the following fields: "
			<< std::endl;
	pcl::visualization::CloudViewer viewer("Simple Cloud Viewer");
	viewer.showCloud(cloud);
	while (!viewer.wasStopped())
	{
	}	

}
