#include <iostream>
// #include <pcl/io/ply_io.h>
#include <string>
#include <math.h>
#include <glob.h>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
// #include <Eigen/Dense>
#include <algorithm>

#define mat_data CV_32F
float kk_ir[3][3]       = { {572.12194657,  0.0,            319.92431451}, 
                            {0.0,           572.23379558,   250.6101592}, 
                            {0.0,           0.0,            1.0}};

float kk[3][3]          = { {535.65426184,  0.0,            319.72728693},
                            {0.0,           535.57826374,   238.83344673},
                            {0.0,           0.0,            1.0}};

float r[3][3]           = { {9.99968601e-01, -7.86208726e-03, -9.92368165e-04},
                            {7.86717595e-03,  9.99955363e-01,  5.23253678e-03},
                            {9.51185208e-04, -5.24017962e-03,  9.99985818e-01}};
                            
float t[3][1]           = { {-26.59783789},
                            {0.31928171},
                            {2.0905246}};

const cv::Mat KK_ir     = cv::Mat(3, 3, mat_data, kk_ir);
const cv::Mat KK_ir_inv = KK_ir.inv();
const cv::Mat KK        = cv::Mat(3, 3, mat_data, kk);
const cv::Mat R         = cv::Mat(3, 3, mat_data, r);
const cv::Mat T         = cv::Mat(3, 1, mat_data, t);


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


cv::Mat meshCV(int rows, int cols){
    cv::Mat grid(3, rows*cols, mat_data);
    for(int i =0; i < rows*cols; ++i){
        grid.at<float> (0,i) = (i % cols);
        grid.at<float> (1,i) = i / cols;
        grid.at<float> (2,i) = 1;
    }
    return grid;
}


cv::Mat calculate(cv::Mat &imgDepth, cv::Mat img){
    imgDepth.convertTo(imgDepth, mat_data);
    cv::Mat allDepth = imgDepth.reshape(0, 1); // 0 channels and 1 row
    allDepth.push_back(allDepth.row(0));
    cv::vconcat(allDepth, allDepth.row(0), allDepth);

    // generate homogenous (u,v,1) meshgrid of pixel positions as 3 x n
    auto alIndex  = meshCV(img.rows, img.cols);

    // project pixels to world coordinate system
    cv::Mat worldCord = (KK_ir_inv * alIndex).mul(allDepth);
    worldCord.push_back(cv::Mat::ones(1, worldCord.cols, mat_data)); // homogenous corrdinates
    
    // camera pixels x = K [RT] X
    // generate RT matrix from individual R and T matrix
    cv::Mat RT;
    cv::hconcat(R, T, RT);
    
    cv::Mat newImg  = KK * RT * worldCord;
    newImg.row(0)  /= newImg.row(2);
    newImg.row(1)  /= newImg.row(2);
    newImg.row(2)  /= newImg.row(2);

    cv:: Mat map_x  = cv::Mat(1, newImg.cols, mat_data);
    newImg.row(0).copyTo(map_x.row(0));
    map_x           = map_x.reshape(0, img.rows);

    cv:: Mat map_y  = newImg.row(1);
    map_y           = map_y.reshape(0, img.rows);

    cv::Mat dst;
    cv::remap( img, dst, map_x, map_y, CV_INTER_LINEAR);
    return dst;
}


int main(int argc, char* argv[]) {
    auto args=argParse(argc, argv);
    // std::string depthImgPath="/media/saumil/Extra_Linux/Dataset/Dataset/MultipleObjects/scene_035/frames/*depth.png";
    // std::string depthImgPath="../../Dataset/MultipleObjects/scene_035/frames/*depth.png";
    std::string depthImgPath    = args.path;
    depthImgPath               += "/*depth.png"; 
    std::string rgbImgPath      = args.path;
    rgbImgPath                 += "/*rgb.png";

    auto depthImageNames        = giveFiles(depthImgPath);
    auto rgbImageNames          = giveFiles(rgbImgPath);

    // sort the data in an increasin lexicographical order
    std::sort(rgbImageNames.begin(), rgbImageNames.end());
    std::sort(depthImageNames.begin(), depthImageNames.end());
    
    cv::Mat img                 = cv::imread(rgbImageNames[0], -1);
    cv::Mat imgDepth            = cv::imread(depthImageNames[0], -1);
    
    if(img.empty()){
        std::cout << "Could not read the image: " << rgbImageNames[0] << std::endl;
        return 1;
    }

    cv::Mat result              = calculate(imgDepth, img);
    cv::imshow("Display window", img);
    cv::imshow("Remapped window", result);
    
    int k = cv::waitKey(0); // Wait for a keystroke in the window
    if(k == 's')
    {
        cv::imwrite("starry_night.png", img);
    }
    return 0;
}