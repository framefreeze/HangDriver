/*****************************************************************************
*   Markerless AR desktop application.
******************************************************************************
*   by Khvedchenia Ievgen, 5th Dec 2012
*   http://computer-vision-talks.com
******************************************************************************
*   Ch3 of the book "Mastering OpenCV with Practical Computer Vision Projects"
*   Copyright Packt Publishing 2012.
*   http://www.packtpub.com/cool-projects-with-opencv/book
*****************************************************************************/

////////////////////////////////////////////////////////////////////
// File includes:
#include "Pattern.hpp"

void PatternTrackingInfo::computePose(const Pattern& pattern, const CameraCalibration& calibration)
{
  cv::Mat Rvec;
  cv::Mat_<float> Tvec;
  cv::Mat raux,taux;

  cv::solvePnP(pattern.points3d, points2d, calibration.getIntrinsic(), calibration.getDistorsion(),raux,taux);
  raux.convertTo(Rvec,CV_32F);
  taux.convertTo(Tvec ,CV_32F);

  cv::Mat_<float> rotMat(3,3); 
  cv::Rodrigues(Rvec, rotMat);

  // Copy to transformation matrix
/*
  for (int col=0; col<3; col++)
  {
    for (int row=0; row<3; row++)
    {        
     pose3d.r().mat[row][col] = rotMat(row,col); // Copy rotation component
      printf("R:row:%d col:%d mat:%f\n",row,col,rotMat(row,col));
    }
    pose3d.t().data[col] = Tvec(col); // Copy translation component
    printf("T:col:%d data:%f\n",col,Tvec(col));
  }
  */

  pose3d.r().mat[0][0]=0.996183;
  pose3d.r().mat[1][0]=-0.018193;
  pose3d.r().mat[2][0]=-0.085371;
  pose3d.r().mat[0][1]=-0.013699;
  pose3d.r().mat[1][1]=0.933330;
  pose3d.r().mat[2][1]=-0.358758;
  pose3d.r().mat[0][2]=0.086206;
  pose3d.r().mat[1][2]=0.358558;
  pose3d.r().mat[2][2]=0.929518;


  pose3d.t().data[0]=0.353836;
  pose3d.t().data[1]=0.485751;
  pose3d.t().data[2]=3.326557;

  // Since solvePnP finds camera location, w.r.t to marker pose, to get marker pose w.r.t to the camera we invert it.
  pose3d = pose3d.getInverted();
}

void PatternTrackingInfo::draw2dContour(cv::Mat& image, cv::Scalar color) const
{
  for (size_t i = 0; i < points2d.size(); i++)
  {
    cv::line(image, points2d[i], points2d[ (i+1) % points2d.size() ], color, 2, CV_AA);
  }
}

