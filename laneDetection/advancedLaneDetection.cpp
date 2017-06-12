#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "stdlib.h"
#include "stdio.h"
#include <iostream>

using namespace cv ;

Mat filter_color (Mat img)
{
    Mat yellow_mask,white_mask,combined;
    inRange(img,Scalar(15,100,120),Scalar(255,255,255),yellow_mask);
    inRange(img,Scalar(0,0,150),Scalar(255,30,255),white_mask);
    combined = white_mask | yellow_mask ;
    return combined;

}

Mat dir_threshold(Mat sobelx, Mat sobely)
{
    Mat magnitude,angles,absx,absy,outDir;
    absx= abs(sobelx);
    absy= abs(sobely);
    cartToPolar( absx, absy, magnitude, angles);
    inRange(angles,0.7,1.2,outDir);
    return outDir;
}

Mat magnitude_threshold(Mat sobelx , Mat sobely)
{
    Mat mag,scaledMag,outMag;
    double maxValue,minValue;
    magnitude( sobelx, sobely, mag);
    minMaxLoc(mag, &minValue, &maxValue);
    divide(maxValue, mag, scaledMag);
    inRange(scaledMag,0.5,4,outMag);
    minMaxLoc(outMag, &minValue, &maxValue);
    return outMag;


}

int main()
{
    Mat img,outColor,grad_x,grad_y,outDir,outMag ;
    img = imread("undistorted.jpg");
    outColor = filter_color(img);
    Sobel( outColor, grad_x, CV_64F, 1, 0, 3, 1, 0, BORDER_DEFAULT );
    Sobel( outColor, grad_y, CV_64F, 0, 1, 3, 1, 0, BORDER_DEFAULT );
    outDir = dir_threshold(grad_x,grad_y);
    outMag = magnitude_threshold(grad_x,grad_y);
    namedWindow( "test", CV_WINDOW_AUTOSIZE );
    namedWindow( "original", CV_WINDOW_AUTOSIZE );
    imshow ("test",outMag);
    imshow("original",img);
    waitKey(0);
    return 0;
}
