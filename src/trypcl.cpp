#include <iostream>
#include <pcl/io/ply_io.h>
#include <pcl/point_types.h>
#include <pcl/visualization/cloud_viewer.h>
#include <string>
#include <math.h>
#include <vector>


int main(int argc, char* argv[]) {
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZRGB>);
	// pcl::io::loadPLYFile<>
	// if (pcl::io::loadPLYFile<pcl::PointXYZ>("test_pcd.pcd", *cloud) == -1) //* load the file
	// {
	// 	PCL_ERROR("Couldn't read file test_pcd.pcd \n");
	// 	return (-1);
	// }
	pcl::PointXYZRGB pt;
	pt.r = 0;
	pt.g = 0;
	pt.b = 255;
	for(int i=0; i<=100; i++) {
		pt.x = 1.0;
		pt.y = 1.0;
		pt.z = float(i);
		pt.rgb = i;
		cloud->push_back(pt);
	}	
	std::cout << "Loaded "
				<< cloud->width * cloud->height
				<< " data points from test_pcd.pcd with the following fields: "
				<< std::endl;
	for (size_t i = 0; i < cloud->points.size(); ++i)
		std::cout << "    " << cloud->points[i].x
				<< " " << cloud->points[i].y
				<< " " << cloud->points[i].z << std::endl;

	pcl::visualization::CloudViewer viewer("Simple Cloud Viewer");
	viewer.showCloud(cloud);
	while (!viewer.wasStopped())
	{
	}
	return 0;
}