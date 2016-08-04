#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include "my_dvo.h"
#include <jni.h>

#ifndef WIN64
    #define EIGEN_DONT_ALIGN_STATICALLY
#endif
#include <Eigen/Dense>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


#include "dvo.hpp"
#include "tum_benchmark.hpp"

#define STR1(x)  #x
#define STR(x)  STR1(x)


JNIEXPORT jint Java_exp_com_tum_opencvjniexperimental_MainActivity_dvoFunc(JNIEnv* env)
{

    std::string dataFolder="/storage/emulated/0/Download/rgbd_dataset_freiburg1_desk/";
    std::string assocFile = dataFolder + "rgbd_assoc.txt";

    std::ifstream assocIn;
    assocIn.open(assocFile.c_str());
    if (!assocIn.is_open())
        return 0;


    return 10 ;

}

JNIEXPORT jint Java_exp_com_tum_opencvjniexperimental_MainActivity_myDVOFunc(JNIEnv* env)
{
    std::string dataFolder = std::string(STR(DVO_SOURCE_DIR)) + "/data/";

    Eigen::Matrix3f K;
#if 1
    // initialize intrinsic matrix: fr1
    K <<    517.3, 0.0, 318.6,
            0.0, 516.5, 255.3,
            0.0, 0.0, 1.0;
//    dataFolder = "/work/maierr/rgbd_data/rgbd_dataset_freiburg1_xyz/";
    //dataFolder="/home/munjalbharti/Downloads/TUM_RGBD/rgbd_dataset_freiburg1_desk/";
    dataFolder="/storage/emulated/0/Download/rgbd_dataset_freiburg1_desk/";
//dataFolder = "/work/maierr/rgbd_data/rgbd_dataset_freiburg1_desk2/";
#else
    dataFolder = "/work/maierr/rgbd_data/rgbd_dataset_freiburg3_long_office_household/";
    // initialize intrinsic matrix: fr3
    K <<    535.4, 0.0, 320.1,
            0.0, 539.2, 247.6,
            0.0, 0.0, 1.0;
#endif




    //std::cout << "Camera matrix: " << K << std::endl;

    // load file names
    std::string assocFile = dataFolder + "rgbd_assoc.txt";
    std::vector<std::string> filesColor;
    std::vector<std::string> filesDepth;
    std::vector<double> timestampsDepth;
    std::vector<double> timestampsColor;
    if (!loadAssoc(assocFile, filesDepth, filesColor, timestampsDepth, timestampsColor))
    {
        std::cout << "Assoc file could not be loaded!" << std::endl;
        return 1;
    }
    int numFrames = filesDepth.size();

    int maxFrames = -1;
    maxFrames = 100;

    Eigen::Matrix4f absPose = Eigen::Matrix4f::Identity();
    std::vector<Eigen::Matrix4f> poses;
    std::vector<double> timestamps;
    poses.push_back(absPose);
    timestamps.push_back(timestampsDepth[0]);

    cv::Mat grayRef = loadGray(dataFolder + filesColor[0]);
    cv::Mat depthRef = loadDepth(dataFolder + filesDepth[0]);
    int w = depthRef.cols;
    int h = depthRef.rows;

    DVO dvo;
    dvo.init(w, h, K);

    std::vector<cv::Mat> grayRefPyramid;
    std::vector<cv::Mat> depthRefPyramid;
    dvo.buildPyramid(depthRef, grayRef, depthRefPyramid, grayRefPyramid);

    // process frames
    double runtimeAvg = 0.0;
    int framesProcessed = 0;
    for (size_t i = 1; i < numFrames && (maxFrames < 0 || i < maxFrames); ++i)
    {
        std::cout << "aligning frames " << (i-1) << " and " << i  << std::endl;

        LOGE("Alligning frames %d and %d", i-1,i);
        // load input frame
        std::string fileColor1 = filesColor[i];
        std::string fileDepth1 = filesDepth[i];
        double timeDepth1 = timestampsDepth[i];
        //std::cout << "File " << i << ": " << fileColor1 << ", " << fileDepth1 << std::endl;
        cv::Mat grayCur = loadGray(dataFolder + fileColor1);
        cv::Mat depthCur = loadDepth(dataFolder + fileDepth1);
        // build pyramid
        std::vector<cv::Mat> grayCurPyramid;
        std::vector<cv::Mat> depthCurPyramid;
        dvo.buildPyramid(depthCur, grayCur, depthCurPyramid, grayCurPyramid);

        // frame alignment
        double tmr = (double)cv::getTickCount();

        Eigen::Matrix4f relPose = Eigen::Matrix4f::Identity();
        dvo.align(depthRefPyramid, grayRefPyramid, depthCurPyramid, grayCurPyramid, relPose);

        tmr = ((double)cv::getTickCount() - tmr)/cv::getTickFrequency();
        runtimeAvg += tmr;

        // concatenate poses
        absPose = absPose * relPose.inverse();
        poses.push_back(absPose);
        timestamps.push_back(timeDepth1);

        depthRefPyramid = depthCurPyramid;
        grayRefPyramid = grayCurPyramid;
        ++framesProcessed;
    }

    double avgRuntime=(runtimeAvg / framesProcessed) * 1000.0;
    LOGE("average runtime: %f ms",avgRuntime);
    std::cout << "average runtime: " << (runtimeAvg / framesProcessed) * 1000.0 << " ms" << std::endl;

    // save poses
    savePoses(dataFolder + "traj.txt", poses, timestamps);

    // clean up
  //  cv::destroyAllWindows();
    LOGE("Direct Image Alignment finished");
    std::cout << "Direct Image Alignment finished." << std::endl;
    return(0);
}
