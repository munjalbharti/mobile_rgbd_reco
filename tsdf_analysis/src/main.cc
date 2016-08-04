// Copyright 2016 Robert Maier, Technical University Munich
#include <iostream>

#ifndef WIN64
    #define EIGEN_DONT_ALIGN_STATICALLY
#endif
#include <Eigen/Dense>

#include "tsdf_analysis.h"

#define STR1(x)  #x
#define STR(x)  STR1(x)

const int alpha_slider_max = 255;
int alpha_slider=0;
TsdfAnalysis tsdfAnalysis;
float slice ;
bool track_val_selected=false;

void on_trackbar( int, void* )
{
   slice = (float)alpha_slider;
   tsdfAnalysis.analyze(slice);
}


int main(int argc, char *argv[])
{
    // folders
    std::string sourceFolder = std::string(STR(APP_SOURCE_DIR));

    std::string inputFile = "/usr/wiss/maierr/Downloads/robert.tsdf";
    std::string outputFolder = "/usr/wiss/maierr/Downloads/";
    std::string meshFile = outputFolder + "mesh.ply";

    //command line usage: <app> -i "/path/to/file/filename.tsdf"
    const char *keys = {
        "{i|input| |input file}"
    };
    cv::CommandLineParser parser(argc, argv, keys);
    std::string inputFilename = parser.get<std::string>("input");

    if (inputFilename.empty())
    {
        std::cout << "No input file specified! using default!" << std::endl;
        inputFilename = inputFile;

    }

    // load tsdf volume

    if (!tsdfAnalysis.load(inputFilename))
    {
        std::cout << "Could not load tsdf volume from file!" << std::endl;
        return false;
    }

    char win_name[50];
    sprintf(win_name, "Select Slice");
    cv::namedWindow(win_name, cv::WINDOW_AUTOSIZE);

    char trackbar_name[50];
    sprintf(trackbar_name, "Height");

    cv::createTrackbar(trackbar_name,win_name,&alpha_slider,alpha_slider_max, on_trackbar );
    on_trackbar( alpha_slider, 0 );
    cv::waitKey(0);


    // save mesh
    if (!tsdfAnalysis.saveMesh(meshFile))
        std::cout << "Could not save mesh!" << std::endl;

    return 0;
}
