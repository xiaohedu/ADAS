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

Mat warp_image(Mat img, Point2f* src_vertices , Point2f* dst_vertices)
{
    Mat M,warped;
    M = getPerspectiveTransform(src_vertices,dst_vertices);
    warpPerspective(img,warped, M,warped.size());
    return warped;

}

void searchForLanes(Mat img)
{
    Mat outImg,halfImg,hist ,midHist;
    double maxVal,minVal;
    Point leftPoint,rightPoint,minInd;
    cvtColor(img,outImg,CV_GRAY2RGB);
    halfImg = img (Range(img.rows/2,img.rows),Range(0,img.cols));
    reduce(halfImg,hist,0,CV_REDUCE_SUM, CV_32SC1);
    midHist =hist(Range(0,1),Range(0,hist.cols/2));
    minMaxLoc(midHist,&minVal,&maxVal,&minInd,&leftPoint);
    std::cout<<leftPoint<<" "<<img.rows<< " "<<img.cols;
    //std::cout<<maxInd<<"\n";
    midHist = hist(Range(0,1),Range(hist.cols/2,hist.cols));
    minMaxLoc(midHist,&minVal,&maxVal,&minInd,&rightPoint);
    //std::cout<<midHist<<"\n";
    //std::cout<<hist.rows<<" "<<hist.cols<<"\n";
    std::cout<<rightPoint<<" "<<img.rows<< " "<<img.cols<<"\n";
    std::cout<<midHist;
    int numOfWindows = 9;
    int windowsHeight = img.rows / numOfWindows ;
    Mat nonZeroCoordinates,nonZeroX,nonZeroY;
    findNonZero(img, nonZeroCoordinates);
    //nonZeroX = nonZeroCoordinates[1];
    //nonZeroY = nonZeroCoordinates[0];
    int margin = 100 , minPix = 50 ;



}

int main()
{
    Mat img,outColor,grad_x,grad_y,outDir,outMag,combined,warped;
    img = imread("undistorted.jpg");
    outColor = filter_color(img);
    Sobel( outColor, grad_x, CV_64F, 1, 0, 3, 1, 0, BORDER_DEFAULT );
    Sobel( outColor, grad_y, CV_64F, 0, 1, 3, 1, 0, BORDER_DEFAULT );
    outDir = dir_threshold(grad_x,grad_y);
    outMag = magnitude_threshold(grad_x,grad_y);
    combined = (outColor & (outMag | outDir));
    Point2f src[4],dst[4];
    src[0]=Point2f( 290, 230 );
    src[1]=Point2f( 350, 230 );
    src[2]=Point2f( 520, 340 );
    src[3]=Point2f( 130, 340 );
    dst[0]= Point2f( 130, 0 );
    dst[1]= Point2f( 520, 0 );
    dst[2]= Point2f( 520, 310);
    dst[3]= Point2f( 130, 310 );
    warped = warp_image(combined,src,dst);
    imshow("warp",warped);
    searchForLanes(combined);
    namedWindow( "test", CV_WINDOW_AUTOSIZE );
    namedWindow( "original", CV_WINDOW_AUTOSIZE );
    imshow ("test",combined);
    imshow("original",img);
    waitKey(0);
    return 0;
}
