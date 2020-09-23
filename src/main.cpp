#include <iostream>
#include <pcl/io/ply_io.h>
#include <string>
#include <math.h>
#include <glob.h>
#include <vector>

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

int main(){
	std::string depthImgPath="/media/saumil/Extra_Linux/Dataset/Dataset/MultipleObjects/scene_035/frames/*depth.png";
	std::string rgbImgPath="/media/saumil/Extra_Linux/Dataset/Dataset/MultipleObjects/scene_035/frames/*rgb.png";
	auto depthImages = giveFiles(depthImgPath);
	auto rgbImages = giveFiles(rgbImgPath);
	// for(auto name: files) std::cout << name << std::endl;
	std::cout << depthImages.size() << std::endl;
	std::cout << rgbImages.size() << std::endl;
	return 0;
}