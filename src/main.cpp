#include <iostream>
// #include <pcl/io/ply_io.h>
#include <string>
#include <math.h>
#include <glob.h>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc.hpp"
#include "cloud.h"

struct arguments {
	std::string path;
};

void printArgs() {
	std::cout << "--path -p Path to the frame/data directory default : /media/saumil/Extra_Linux/Dataset/Dataset/MultipleObjects/scene_035/frames\n";
	std::cout << "--help -h Help\n"; 
}

arguments argParse(int argc, char* argv[]) {
	// if(argc==1) {
	// 	printArgs();
	// 	exit(0);
	// }
	arguments args;
	args.path = "/media/saumil/Extra_Linux/Dataset/Dataset/MultipleObjects/scene_035/frames";
	for(int i=1; i<argc; i++) {
		if(std::strcmp(argv[i], "--help")==0 || std::strcmp(argv[i], "-h")==0) {
			printArgs();
			exit(0);
		}
		else if(std::strcmp(argv[i], "--path")==0 || std::strcmp(argv[i], "-p")==0) {
			args.path = argv[i+1];
		}
	}
	
	return args;
}

std::vector<std::string> giveFiles(std::string pattern) {
	glob_t glob_result;
    glob(pattern.c_str(),GLOB_TILDE,NULL,&glob_result);
    std::vector<std::string> files;
    for(unsigned int i=0; i<glob_result.gl_pathc; ++i){
        files.push_back(std::string(glob_result.gl_pathv[i]));
    }
    globfree(&glob_result);
	return files;
}

float kk_ir[3][3] = {{572.12194657, 0.0, 319.92431451}, 
					  {0.0, 572.23379558, 250.6101592}, 
					  {0.0, 0.0, 1.0}};
float kk[3][3] = {{535.65426184, 0.0, 319.72728693},
				   {0.0, 535.57826374, 238.83344673},
				   {0.0, 0.0, 1.0}};
float r[3][3] = {{9.99968601e-01, -7.86208726e-03, -9.92368165e-04},
				  {7.86717595e-03,  9.99955363e-01,  5.23253678e-03},
				  {9.51185208e-04, -5.24017962e-03,  9.99985818e-01}};
float t[3] = {-26.59783789,0.31928171,2.0905246};

cv::Mat KK_ir = (cv::Mat_<float>(3,3) << 572.12194657, 0.0, 319.92431451, 0.0, 572.23379558, 250.6101592, 0.0, 0.0, 1.0);
cv::Mat KK_ir_inv = KK_ir.inv();
cv::Mat KK = cv::Mat(3, 3, CV_32F, kk);
cv::Mat R = cv::Mat(3, 3, CV_32F, r);
// cv::Mat T = cv::Mat(3, 1, CV_32F, t);

cv::Mat calculate(int h, int w, cv::Mat depth, cv::Mat img) {
	int cols = h*w;
	depth.convertTo(depth, CV_32F);
	std::cout << h << " " << w << std::endl;
	cv::Mat all_ind = cv::Mat::ones(3, cols, CV_32F); 
	cv::Mat Translate = cv::Mat::ones(3, cols, CV_32F);
	cv::Mat allDepth = cv::Mat::ones(3, cols, CV_32F);
	cv::Mat map_x = cv::Mat::ones(h, w, CV_32F);
	cv::Mat map_y = cv::Mat::ones(h, w, CV_32F);
	auto newDepth = depth.reshape(0, 1);
	for(int i=0; i<cols; i++) {
		all_ind.at<float>(0,i) = i%w;
		all_ind.at<float>(1,i) = i/w;
		Translate.at<float>(0,i) = t[0];
		Translate.at<float>(1,i) = t[1];
		Translate.at<float>(2,i) = t[2];
		allDepth.at<float>(0,i) = newDepth.at<float>(0,i);
		allDepth.at<float>(1,i) = newDepth.at<float>(0,i);
		allDepth.at<float>(2,i) = newDepth.at<float>(0,i);
	}
	cv::Mat indices = (KK_ir_inv*all_ind).mul(allDepth);
	cv::Mat inb = KK*(R*indices + Translate);
	createCloudFromImage(img, indices, depth);
	for(int i=0; i<h; i++) {
		for(int j=0; j<w; j++) {
			map_x.at<float>(i,j) = inb.at<float>(0, i*w+j) / inb.at<float>(2, i*w+j);
			map_y.at<float>(i,j) = inb.at<float>(1, i*w+j) / inb.at<float>(2, i*w+j);
		}
	}
	cv::Mat dst;
	cv::remap( img, dst, map_x, map_y, CV_INTER_LINEAR);
	// auto warped_ind =  + Translate;
	return dst;
}

int main(int argc, char* argv[]) {
	auto args=argParse(argc, argv);
	// std::string depthImgPath="/media/saumil/Extra_Linux/Dataset/Dataset/MultipleObjects/scene_035/frames/*depth.png";
	// std::string depthImgPath="../../Dataset/MultipleObjects/scene_035/frames/*depth.png";
	std::string depthImgPath = args.path;
	depthImgPath += "/*depth.png"; 
	std::string rgbImgPath = args.path;
	rgbImgPath += "/*rgb.png";
	
	auto depthImages = giveFiles(depthImgPath);
	auto rgbImages = giveFiles(rgbImgPath);
    cv::Mat img = cv::imread(rgbImages[0], cv::IMREAD_COLOR);
	cv::Mat depth = cv::imread(depthImages[0], cv::IMREAD_ANYCOLOR | cv::IMREAD_ANYDEPTH);
	
	// std::cout << KK_ir.at<double>(0,0) << std::endl;
	// std::cout << img.size().height << std::endl;
	auto formed = calculate(depth.size().height, depth.size().width, depth, img);
	// std::cout << depth << std::endl;
	if(img.empty()) {
        std::cout << "Could not read the image: " << rgbImages[0] << std::endl;
        return 1;
    }
    cv::imshow("Display window", img);
	cv::imshow("Depth", depth);
	cv::imshow("Transformed", formed);
    int k = cv::waitKey(0); // Wait for a keystroke in the window
    if(k == 's')
    {
        cv::imwrite("starry_night.png", img);
    }
	return 0;
}