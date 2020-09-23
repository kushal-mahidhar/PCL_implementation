#include <iostream>
#include <pcl/io/ply_io.h>
#include <string>
#include <math.h>
#include <glob.h>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

struct arguments {
	std::string path;
};

void printArgs() {
	std::cout << "--path -p Path to the frame/data directory\n";
	std::cout << "--help -h Help\n"; 
}

arguments argParse(int argc, char* argv[]) {
	if(argc==1) {
		printArgs();
		exit(0);
	}
	arguments args;
	
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



int main(int argc, char* argv[]) {
	auto args=argParse(argc, argv);
	// std::string depthImgPath="/media/saumil/Extra_Linux/Dataset/Dataset/MultipleObjects/scene_035/frames/*depth.png";
	// std::string depthImgPath="../../Dataset/MultipleObjects/scene_035/frames/*depth.png";
	std::string depthImgPath = args.path;
	depthImgPath += "/*depth.png"; 
	std::string rgbImgPath = args.path;
	rgbImgPath += "/*rgb.png";
	std::cout << depthImgPath << std::endl;
	auto depthImages = giveFiles(depthImgPath);
	auto rgbImages = giveFiles(rgbImgPath);
    cv::Mat img = cv::imread(rgbImages[0], cv::IMREAD_COLOR);
	if(img.empty()) {
        std::cout << "Could not read the image: " << rgbImages[0] << std::endl;
        return 1;
    }
    cv::imshow("Display window", img);
    int k = cv::waitKey(0); // Wait for a keystroke in the window
    if(k == 's')
    {
        cv::imwrite("starry_night.png", img);
    }
	return 0;
}