#include <jni.h>

#include "ProcessImage.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <Eigen/Eigen>
#include <thread>
#include <pthread.h>
#include <sys/syscall.h>
#include <android/log.h>
#include <opencv2/xfeatures2d.hpp>

#include <stdio.h>

using namespace std;
using namespace cv;
using namespace Eigen;


int toGray(Mat img, Mat& gray);
void find_features(Mat img, Mat& dst);
void cornerHarris_demo( Mat src_gray, Mat dst );


void *thread_function_one(void *)
{
    pthread_t tid = pthread_self();
    int id=gettid();
    int pid=getpid();
    int ppid= getppid();
    // int sid = syscall(SYS_gettid);
    __android_log_print(ANDROID_LOG_INFO, "MYTAG", "PID %d",pid);
    __android_log_print(ANDROID_LOG_INFO, "MYTAG", "TID %d",id);

    int i;
    for(i=0 ; i < 1000; i++) {
        __android_log_write(ANDROID_LOG_INFO, "MYTAG", "thread function 1\n");
        // sleep(10);
    }
//   sleep(30);
}

void *thread_function_two(void *)
{
    int j;
    for(j=0; j < 1000; j++) {
        __android_log_write(ANDROID_LOG_INFO, "MYTAG", "thread function 2\n");
        // sleep(3);
    }
    // sleep(10);
}


JNIEXPORT jstring JNICALL Java_exp_com_tum_opencvjniexperimental_MainActivity_helloWorld(JNIEnv* env, jobject obj)
{
    return env-> NewStringUTF("1323got Image pointer11111!!!");
}

JNIEXPORT jint Java_exp_com_tum_opencvjniexperimental_MainActivity_convertNativeGray(JNIEnv* env, jobject obj, jlong addrRgba, jlong addrDetectedPointsImage) {

    Mat& mRgb = *(Mat*)addrRgba;
    Mat& detectedPointsImage = *(Mat*)addrDetectedPointsImage;

    int conv;
    jint retVal;



    Mat mGray = Mat::zeros( mRgb.size(), CV_32FC1 );;
    conv = toGray(mRgb, mGray);

    if (CV_MAJOR_VERSION < 3) {
        cout << "less than 3" << endl;
    } else {
        cout << "MORE than 3" << endl;
    }




//    Ptr<ORB> orbPointer = ORB::create();
//    if (orbPointer == NULL) {
//        cout << "detector not found " << endl;
//
//    }
    vector<KeyPoint> keyPoints1;
    // orbPointer->detect(mGray, keyPoints1);


    cv::Ptr<Feature2D> f2d = xfeatures2d::SURF::create();
    f2d->detect( mGray, keyPoints1 );

    drawKeypoints(mGray, keyPoints1, detectedPointsImage);

    //cornerHarris_demo(mGray,detectedPointsImage);



    retVal = (jint)conv;

    MatrixXd m(2,2);
    m(0,0) = 3;
    m(1,0) = 2.5;
    m(0,1) = -1;
    m(1,1) = m(1,0) + m(0,1);

    // std::cout << "Matrix Value " <<  m << endl;
    int i=1;
    //  LOGE("Number = %d", i);
    //LOGD(m);

    return retVal;
//
}


int toGray(Mat img, Mat& gray)
{
    //cv::imshow("test",img);
    cv::cvtColor(img, gray, CV_RGBA2GRAY); // Assuming RGBA input

    // if (gray.rows == img.rows && gray.cols == img.cols)
    // {
    //     return (1);
    //  }

    // std::thread t1(&thread_function_one);   // t starts running
    //   std::thread t2(&thread_function_two);
//    pthread_t tid = pthread_self();
//    int id=gettid();
//    int pid=getpid();
//    int ppid= getppid();
//    // int sid = syscall(SYS_gettid);
//    __android_log_print(ANDROID_LOG_INFO, "MYTAG", "PID",pid);
//    __android_log_print(ANDROID_LOG_INFO, "MYTAG", "TID %d",id);
//
//
//    pthread_t t1;
//    pthread_t t2;
//
//    __android_log_write(ANDROID_LOG_INFO, "MYTAG", "main threadnoo\n");
//    __android_log_write(ANDROID_LOG_INFO, "MYTAG", "main thread ok ok\n");
//    pthread_create(&t1, NULL, thread_function_one,NULL);
//    pthread_create(&t2, NULL, thread_function_two,NULL);
//    //t1.join();   // main thread waits for the thread t to finish
//    __android_log_write(ANDROID_LOG_INFO, "MYTAG", "now1\n");
//    __android_log_write(ANDROID_LOG_INFO, "MYTAG", "now2\n");
//
//    int j ;
//    for(j=0; j < 1000; j++) {
//        __android_log_write(ANDROID_LOG_INFO, "MYTAG", "main function 2\n");
//        //sleep(3);
//    }
//
//    __android_log_write(ANDROID_LOG_INFO, "MYTAG", "t1 finished\n");
//
//   // t2.join();
//    pthread_join(t1, NULL);
//    pthread_join(t2, NULL);
//    __android_log_write(ANDROID_LOG_INFO, "MYTAG", "main thread\n");

    return(0);
}



void find_features(Mat image, Mat output){

    if (CV_MAJOR_VERSION < 3) {
        cout << "less than 3" << endl;
    } else {
        cout << "MORE than 3" << endl;
    }


    //cv::FeatureDetector * detector = new cv::FastFeatureDetector();
    //cv::DescriptorExtractor * extractor = new cv::OrbDescriptorExtractor();
    cv::Ptr<Feature2D> f2d = xfeatures2d::SIFT::create();
    //cv::Ptr<Feature2D> f2d = xfeatures2d::SURF::create();
    //cv::Ptr<Feature2D> f2d = ORB::create();
    // you get the picture, i hope..

    //-- Step 1: Detect the keypoints:
    std::vector<KeyPoint> keyPoints1;
    f2d->detect( image, keyPoints1 );


//   Ptr<ORB> orbPointer = ORB::create();
//    if (orbPointer == NULL) {
//        cout << "detector not found " << endl;
//        return ;
//    }
//
//    vector<KeyPoint> keyPoints1;
//
//    orbPointer->detect(image, keyPoints1);

    drawKeypoints(image, keyPoints1, output);


}
//
void cornerHarris_demo( Mat src_gray, Mat dst )
{


    //  Mat  dst_norm, dst_norm_scaled;
//
//
    /// Detector parameters
    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.04;
//
//    /// Detecting corners
    cornerHarris( src_gray, dst, blockSize, apertureSize, k, BORDER_DEFAULT );;

//
//    /// Normalizing
    // normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
    // convertScaleAbs( dst_norm, dst_norm_scaled );
//
//    /// Drawing a circle around corners
    //  for( int j = 0; j < dst.rows ; j++ )
    // { for( int i = 0; i < dst.cols; i++ )
    //   {
    //     if( (int) dst.at<float>(j,i) > thresh )
    //   {
    //     circle( dst_norm_scaled, Point( i, j ), 5,  Scalar(0), 2, 8, 0 );
    //  }
    //  }
    // }
//    /// Showing the result
//    namedWindow( corners_window, CV_WINDOW_AUTOSIZE );
//    imshow( corners_window, dst );
}



JNIEXPORT jobject Java_exp_com_tum_opencvjniexperimental_MainActivity_getMesh(JNIEnv* env){


    jclass vecClass = env->FindClass("exp/com/tum/opencvjniexperimental/Vec3f");
    jmethodID vecCons = env->GetMethodID(vecClass, "<init>","(FFF)V");

    //Set Vertices
    jobject vecObj1 = env->NewObject(vecClass, vecCons,0.0,0.0,0.0);
    jobject vecObj2 = env->NewObject(vecClass, vecCons,5.0,0.0,0.0);
    jobject vecObj3 = env->NewObject(vecClass, vecCons,2.5,5.0,0.0);

    jobjectArray verticesArray= env->NewObjectArray(3, env->FindClass("exp/com/tum/opencvjniexperimental/Vec3f"),0);
    env->SetObjectArrayElement( verticesArray,0,vecObj1 );
    env->SetObjectArrayElement( verticesArray,1,vecObj2 );
    env->SetObjectArrayElement( verticesArray,2,vecObj3 );


    //Set Normals
    // jobject normalObj1 = env->NewObject(vecClass, vecCons,3.0,4.0,5.0);
    //jobject normalObj2 = env->NewObject(vecClass, vecCons,4.0,5.0,6.0);
    //jobjectArray normalsArray= env->NewObjectArray(2, env->FindClass("exp/com/tum/opencvjniexperimental/Vec3f"),0);
    // env->SetObjectArrayElement( normalsArray,0,normalObj1 );
    // env->SetObjectArrayElement( normalsArray,1,normalObj2 );

    //Set Textures
    // jobject textObj1 = env->NewObject(vecClass, vecCons,5.0,6.0,7.0);
    // jobject textObj2 = env->NewObject(vecClass, vecCons,6.0,7.0,8.0);
    //jobjectArray textArray= env->NewObjectArray(2, env->FindClass("exp/com/tum/opencvjniexperimental/Vec3f"),0);
    //env->SetObjectArrayElement( textArray,0,textObj1 );
    //env->SetObjectArrayElement( textArray,1,textObj2 );

    //Set Faces
    jclass faceVertClass = env->FindClass("exp/com/tum/opencvjniexperimental/FaceVert");
    jmethodID faceVertCons = env->GetMethodID(faceVertClass, "<init>","(III)V");



    jobject faceVertObj1_1 = env->NewObject(faceVertClass, faceVertCons,1,NULL,NULL);
    jobject faceVertObj1_2 = env->NewObject(faceVertClass, faceVertCons,2,NULL,NULL);
    jobject faceVertObj1_3 = env->NewObject(faceVertClass, faceVertCons,3,NULL,NULL);


    jobjectArray faceVertArray1= env->NewObjectArray(3, env->FindClass("exp/com/tum/opencvjniexperimental/FaceVert"),0);
    env->SetObjectArrayElement( faceVertArray1,0,faceVertObj1_1 );
    env->SetObjectArrayElement( faceVertArray1,1,faceVertObj1_2 );
    env->SetObjectArrayElement( faceVertArray1,2,faceVertObj1_3 );

    //jobject faceVertObj2_1 = env->NewObject(faceVertClass, faceVertCons,5,5,5);
    //jobject faceVertObj2_2 = env->NewObject(faceVertClass, faceVertCons,8,8,8);

    //jobjectArray faceVertArray2= env->NewObjectArray(2, env->FindClass("exp/com/tum/opencvjniexperimental/FaceVert"),0);
    // env->SetObjectArrayElement( faceVertArray2,0,faceVertObj2_1 );
    // env->SetObjectArrayElement( faceVertArray2,1,faceVertObj2_2 );

    jclass faceVertArrayClass = env->GetObjectClass(faceVertArray1);
    // jclass faceVertArrayClass = env->FindClass("[exp/com/tum/opencvjniexperimental/FaceVert");
    jobjectArray faceVertArrayObject = env->NewObjectArray(1, faceVertArrayClass, NULL);
    env->SetObjectArrayElement( faceVertArrayObject,0,faceVertArray1 );
    //env->SetObjectArrayElement( faceVertArrayObject,1,faceVertArray2 );


    jclass cls = env->FindClass("exp/com/tum/opencvjniexperimental/Mesh");
    // jmethodID constructor = env->GetMethodID(cls, "<init>","([Lexp/com/tum/opencvjniexperimental/Vec3f;[Lexp/com/tum/opencvjniexperimental/Vec3f;[Lexp/com/tum/opencvjniexperimental/Vec3f;)V");
    jmethodID constructor = env->GetMethodID(cls, "<init>","([Lexp/com/tum/opencvjniexperimental/Vec3f;[Lexp/com/tum/opencvjniexperimental/Vec3f;[Lexp/com/tum/opencvjniexperimental/Vec3f;[[Lexp/com/tum/opencvjniexperimental/FaceVert;)V");

//    jobject obj = env->NewObject(cls, constructor,verticesArray,textArray,normalsArray,faceVertArrayObject);

    jobject obj = env->NewObject(cls, constructor,verticesArray,NULL,NULL,faceVertArrayObject);

    return obj ;

}