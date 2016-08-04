// Copyright 2016 Robert Maier, Technical University Munich
#ifndef TSDF_ANALYSIS_H
#define TSDF_ANALYSIS_H

#ifndef WIN64
    #define EIGEN_DONT_ALIGN_STATICALLY
#endif
#include <Eigen/Dense>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <opencv2/core/eigen.hpp>

class TsdfAnalysis
{
public:
    TsdfAnalysis();
    ~TsdfAnalysis();

    bool analyze(float slice);

    bool load(const std::string &filename);

    bool saveMesh(const std::string &meshFile) const;

private:
    float interpolate(float x, float y, float z) const;

    cv::Mat get_segmented_image(float slice, int scale_fact);

    cv::Mat get_volume_image(float slice,int scale_fact);
    cv::Mat get_area_image(float slice,int scale_fact);
    cv::Mat get_dist_transform_image(float slice, int scale_fact);

    int get_foreground_pixels_count(cv::Mat src);
    cv::Mat get_hsv_image(cv::Mat img);


    float* tsdf_;
    float* weights_;
    bool hasColors_;
    unsigned char* colors_;
    float* colorWeights_;

    Eigen::Vector3i dim_;
    size_t gridSize_;
    Eigen::Vector3f size_;
    Eigen::Vector3f voxelSize_;
};

#endif

