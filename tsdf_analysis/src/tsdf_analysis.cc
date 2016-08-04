// Copyright 2016 Robert Maier, Technical University Munich
#include "tsdf_analysis.h"

#include <iostream>
#include <fstream>


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "marching_cubes.h"


TsdfAnalysis::TsdfAnalysis() :
    tsdf_(0),
    weights_(0),
    hasColors_(false),
    colors_(0),
    colorWeights_(0),
    dim_(0, 0, 0),
    gridSize_(0),
    size_(0.0f, 0.0f, 0.0f),
    voxelSize_(0.0f, 0.0f, 0.0f)
{
}


TsdfAnalysis::~TsdfAnalysis()
{
    delete tsdf_;
    delete weights_;
    delete colors_;
    delete colorWeights_;
}
bool TsdfAnalysis::analyze(float slice)
{
	//interpolate slide to get z.. currently integer value is received

	int scale_fact=2;

	cv::Mat slice_img = get_area_image(slice,scale_fact);
	cv::Mat dist = get_dist_transform_image(slice,scale_fact);
	cv::Mat vol_img = get_volume_image(slice,scale_fact);


        char win[100];
	sprintf(win, "Slice Area");


	char dis_win[100];
	sprintf(dis_win, "Distance Transform");

	char vol_win[100];
	sprintf(vol_win, "Slice Volume");

	static bool window_displayed=false;

        if(!window_displayed){
	    cv::namedWindow(win, cv::WINDOW_AUTOSIZE);
	    cv::moveWindow(win, 500, 1000);

	    cv::namedWindow(vol_win, cv::WINDOW_AUTOSIZE);
	    cv::moveWindow(vol_win, 500 +slice_img.cols, 10);

	    cv::namedWindow(dis_win);
	    cv::moveWindow(dis_win, 500 + 2*slice_img.cols, 10);

	   window_displayed=true;
       }

	cv::imshow(win,slice_img);
	cv::imshow(dis_win,dist);
	cv::imshow(vol_win,vol_img);

	cv::waitKey(0);
	return true;

}


int TsdfAnalysis::get_foreground_pixels_count(cv::Mat src){
	int count_white = 0;
	for( int y = 0; y < src.rows; y++ ) {
	  for( int x = 0; x < src.cols; x++ ) {
	      if ( src.at<uchar>(y,x) == 255 ) {
	        count_white++;
	      }
	  }
	}
	return count_white;
}


cv::Mat TsdfAnalysis::get_dist_transform_image(float slice, int scale_fact){
	cv::Mat seg_img = get_segmented_image(slice, scale_fact);
	cv::Mat dist;
	cv::distanceTransform(seg_img, dist, CV_DIST_L1, 3);
	dist= get_hsv_image(dist);
	return dist;

}

cv::Mat TsdfAnalysis::get_segmented_image(float slice, int scale_fact){
	float z=slice;
	int w= dim_[0];
	int h= dim_[1];

	int no_of_pixels_x=w*scale_fact;
	int no_of_pixels_y=h*scale_fact;

	cv::Mat slice_img=cv::Mat(no_of_pixels_y,no_of_pixels_x,CV_8UC1, cvScalar(0));

	for (int y=0 ; y < no_of_pixels_y; y++){
		for (int x=0 ; x < no_of_pixels_x; x++){
			float x_center=x/scale_fact;
			float y_center=y/scale_fact;
			float z_center=z;

			float voxel_val= interpolate(x_center,y_center,z_center);

			if(voxel_val > 0){
		    	//for cv mat top left corner is 0,0
		    	slice_img.at<uchar>(no_of_pixels_y-y-1,x)=255;
			}else {
				slice_img.at<uchar>(no_of_pixels_y-y-1,x)=0;
				}
			}
		}

	return slice_img;

}

cv::Mat TsdfAnalysis::get_area_image(float slice,int scale_fact){

    	cv::Mat slice_img = get_segmented_image(slice,scale_fact);
	int fg_pixels = get_foreground_pixels_count(slice_img);
	float area_per_pixel=(voxelSize_[0]/scale_fact)*(voxelSize_[1]/scale_fact);
	float area=area_per_pixel*fg_pixels;

	char buff[100];
	snprintf(buff, sizeof(buff), "Area: %.4f",area);
	std::string area_str = buff;

	cv::putText(slice_img,area_str,cvPoint(50,20),cv:: FONT_HERSHEY_PLAIN,0.8,cv::Scalar(255,0,0,0),1);

	return slice_img;

}

cv::Mat TsdfAnalysis::get_hsv_image(cv::Mat img)
{
	cv::normalize(img, img, 0, 1., cv::NORM_MINMAX);
	cv::Mat hsv=cv::Mat(img.rows,img.cols,CV_8UC3, cvScalar(0,0,0));

	for (int i=0; i < img.rows;i++){
		for(int j=0;j< img.cols;j++){
			Vec3b pix;
			pix[0]=150*img.at<float>(i,j);
			pix[1]=255;
			pix[2]=255;
			hsv.at<Vec3b>(i,j)=pix;
		 }
		}

	cv::Mat bgr;
	cv::cvtColor(hsv, bgr,CV_HSV2BGR);

	return bgr ;

}

cv::Mat TsdfAnalysis::get_volume_image(float slice,int scale_fact)
{
	int w=dim_[0];
	int h=dim_[1];

	int no_of_pixels_x=w*scale_fact;
        int no_of_pixels_y=h*scale_fact;


	cv::Mat slice_img=cv::Mat(no_of_pixels_y,no_of_pixels_x,CV_32FC1, cvScalar(0));
	int total_voxels_inside=0;

	for (int y=0 ; y < no_of_pixels_y; y++){
		for (int x=0 ; x < no_of_pixels_x; x++){

			float vox_x = x/scale_fact;
			float vox_y = y/scale_fact;
			float vox_z = slice;
		    float voxel_val= interpolate(vox_x,vox_y,vox_z);

		    if(voxel_val < 0){
		    	slice_img.at<float>(no_of_pixels_y-y-1,x)=0;
		    }else {
		    	int voxels_above=1;
		    	for (int z=slice+1; z <= 255  ; z++ ){
		    	 	voxel_val= interpolate(vox_x,vox_y,z);
		    		if(voxel_val > 0){
		    			voxels_above = voxels_above + 1;
		    		}
		    	}
		      //maximum height can be 256
		      slice_img.at<float>(no_of_pixels_y-y-1,x) = voxels_above;
		      total_voxels_inside=total_voxels_inside + voxels_above;
		    }
		}
	}

	//the divided voxel along x and y and not z
        float vol_per_voxel=(voxelSize_[0]/scale_fact)*(voxelSize_[1]/scale_fact)*voxelSize_[2];
	float vol=vol_per_voxel * total_voxels_inside;

	slice_img= get_hsv_image(slice_img);

	char buff[100];
	snprintf(buff, sizeof(buff), "Vol: %.4f",vol);
	std::string vol_str = buff;

	cv::putText(slice_img,vol_str,cvPoint(50,20),cv:: FONT_HERSHEY_PLAIN,0.8,cv::Scalar(255,255,255,0),1);
        return slice_img;

}


bool TsdfAnalysis::load(const std::string &filename)
{
    std::ifstream inFile(filename.c_str(), std::ios::binary);
    if (!inFile.is_open())
        return false;

    // read volume dimensions and size
    inFile.read((char*)dim_.data(), sizeof(int) * 3);
    inFile.read((char*)size_.data(), sizeof(double) * 3);
    inFile.read((char*)&hasColors_, sizeof(bool));

    // check volume properties
    if (dim_[0] <= 0 || dim_[1] <= 0 || dim_[2] <= 0)
        return false;
    if (size_[0] <= 0.0f || size_[1] <= 0.0f || size_[2] <= 0.0f)
        return false;

    gridSize_ = dim_[0] * dim_[1] * dim_[2];
    voxelSize_ = size_.cwiseQuotient(dim_.cast<float>());
    std::cout << "volume dim: " << dim_.transpose() << std::endl;
    std::cout << "volume size: " << size_.transpose() << std::endl;
    std::cout << "voxel size: " << voxelSize_.transpose() << std::endl;

    try
    {
        // allocate tsdf volume
        tsdf_ = new float[gridSize_];
        std::fill_n(tsdf_, gridSize_, -1.0f);
        weights_ = new float[gridSize_];
        std::fill_n(weights_, gridSize_, 0.0f);

        if (hasColors_)
        {
            colors_ = new unsigned char[gridSize_ * 3];
            unsigned char defaultColor = 127; //0;
            std::fill_n(colors_, gridSize_ * 3, defaultColor);

            colorWeights_ = new float[gridSize_];
            std::fill_n(colorWeights_, gridSize_, 0.0f);
        }
    }
    catch (...)
    {
        std::cerr << "Could not allocate tsdf volume!" << std::endl;
        return false;
    }

    // read sdf values
    inFile.read((char*)tsdf_, sizeof(float) * gridSize_);
    // read weights
    inFile.read((char*)weights_, sizeof(float) * gridSize_);
    // read colors
    if (hasColors_)
    {
        inFile.read((char*)colors_, sizeof(unsigned char) * gridSize_ * 3);
        inFile.read((char*)colorWeights_, sizeof(float) * gridSize_);
    }

    inFile.close();

    return true;
}


bool TsdfAnalysis::saveMesh(const std::string &meshFile) const
{
    // extract mesh using marching cubes
    std::cout << "Marching cubes ..." << std::endl;
    MarchingCubes mc(dim_, size_);
    mc.computeIsoSurface(tsdf_, colors_);
    std::cout << "   # generated mesh faces " << mc.numFaces() << std::endl;
    std::cout << "   # generated mesh vertices " << mc.numVertices() << std::endl;

    // save mesh to ply file
    std::cout << "Saving mesh as ply file ..." << std::endl;
    //std::cout << "   mesh filename: " << meshFile << std::endl;
    if (!mc.savePly(meshFile))
    {
        std::cerr << "Mesh could not be saved!" << std::endl;
    }

    return true;
}


float TsdfAnalysis::interpolate(float x, float y, float z) const
{
    float valCur = std::numeric_limits<float>::quiet_NaN();

    int w = dim_[0];
    int h = dim_[1];
    int s = dim_[2];

#if 0
    // direct lookup, no interpolation
    int x0 = static_cast<int>(std::floor(x + 0.5f));
    int y0 = static_cast<int>(std::floor(y + 0.5f));
    int z0 = static_cast<int>(std::floor(z + 0.5f));
    if (x0 >= 0 && x0 < w && y0 >= 0 && y0 < h && z0 >= 0 && z0 < s)
        valCur = tsdf_[z0*w*h + y0*w + x0];
#else
    // trilinear interpolation
    int x0 = static_cast<int>(std::floor(x));
    int y0 = static_cast<int>(std::floor(y));
    int z0 = static_cast<int>(std::floor(z));
    int x1 = x0 + 1;
    int y1 = y0 + 1;
    int z1 = z0 + 1;

    float x1_weight = x - static_cast<float>(x0);
    float y1_weight = y - static_cast<float>(y0);
    float z1_weight = z - static_cast<float>(z0);
    float x0_weight = 1.0f - x1_weight;
    float y0_weight = 1.0f - y1_weight;
    float z0_weight = 1.0f - z1_weight;

    if (x0 < 0 || x0 >= w)
        x0_weight = 0.0f;
    if (x1 < 0 || x1 >= w)
        x1_weight = 0.0f;
    if (y0 < 0 || y0 >= h)
        y0_weight = 0.0f;
    if (y1 < 0 || y1 >= h)
        y1_weight = 0.0f;
    if (z0 < 0 || z0 >= s)
        z0_weight = 0.0f;
    if (z1 < 0 || z1 >= s)
        z1_weight = 0.0f;
    float w000 = x0_weight * y0_weight * z0_weight;
    float w100 = x1_weight * y0_weight * z0_weight;
    float w010 = x0_weight * y1_weight * z0_weight;
    float w110 = x1_weight * y1_weight * z0_weight;
    float w001 = x0_weight * y0_weight * z1_weight;
    float w101 = x1_weight * y0_weight * z1_weight;
    float w011 = x0_weight * y1_weight * z1_weight;
    float w111 = x1_weight * y1_weight * z1_weight;

    size_t offZ0 = z0*w*h;
    size_t offZ1 = z1*w*h;
    size_t offY0 = y0*w;
    size_t offY1 = y1*w;

    if (w000 > 0.0f && weights_[offZ0 + offY0 + x0] <= 0.0f)
        w000 = 0.0f;
    if (w010 > 0.0f && weights_[offZ0 + offY1 + x0] <= 0.0f)
        w010 = 0.0f;
    if (w100 > 0.0f && weights_[offZ0 + offY0 + x1] <= 0.0f)
        w100 = 0.0f;
    if (w110 > 0.0f && weights_[offZ0 + offY1 + x1] <= 0.0f)
        w110 = 0.0f;
    if (w001 > 0.0f && weights_[offZ1 + offY0 + x0] <= 0.0f)
        w001 = 0.0f;
    if (w011 > 0.0f && weights_[offZ1 + offY1 + x0] <= 0.0f)
        w011 = 0.0f;
    if (w101 > 0.0f && weights_[offZ1 + offY0 + x1] <= 0.0f)
        w101 = 0.0f;
    if (w111 > 0.0f && weights_[offZ1 + offY1 + x1] <= 0.0f)
        w111 = 0.0f;

    float sumWeights = w000 + w100 + w010 + w110 + w001 + w101 + w011 + w111;
    float sum = 0.0f;
    if (w000 > 0.0f)
        sum += tsdf_[offZ0 + offY0 + x0] * w000;
    if (w010 > 0.0f)
        sum += tsdf_[offZ0 + offY1 + x0] * w010;
    if (w100 > 0.0f)
        sum += tsdf_[offZ0 + offY0 + x1] * w100;
    if (w110 > 0.0f)
        sum += tsdf_[offZ0 + offY1 + x1] * w110;
    if (w001 > 0.0f)
        sum += tsdf_[offZ1 + offY0 + x0] * w001;
    if (w011 > 0.0f)
        sum += tsdf_[offZ1 + offY1 + x0] * w011;
    if (w101 > 0.0f)
        sum += tsdf_[offZ1 + offY0 + x1] * w101;
    if (w111 > 0.0f)
        sum += tsdf_[offZ1 + offY1 + x1] * w111;

    if (sumWeights > 0.0f)
        valCur = sum / sumWeights;
#endif

    return valCur;
}
