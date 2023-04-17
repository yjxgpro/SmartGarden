/******************************************************************************
Author: Yucheng Xiao, 2740379x@student.gla.ac.uk
This file prototypes the Camera and ServoCallback class, implemented in pestDetect.cpp. 
pest detecting is a part function of Project "SmartGarden",
please click https://github.com/yjxgpro/SmartGarden.git see more.
******************************************************************************/

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

class ServoCallback {
public:
    virtual void hasSample(int n1, int n2) = 0;
};


class Camera {
public:
    int pos_x;
    int pos_y;
    void detect();
    ServoCallback* sCallback;
    void registerCallback(ServoCallback* cb);
};
