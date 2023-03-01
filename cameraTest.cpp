#include<opencv2/opencv.hpp>
#include<cstdio>
#include<iostream>
#include<windows.h>
#include<opencv2/imgproc/types_c.h>

//using namespace cv;
using namespace std;

int main(int argc, char* argv[]) {
    // 打开摄像头
    cv::VideoCapture cam(0);
    if (!cam.isOpened()) {
        cout << "cam open failed!" << endl;
        getchar();
        return -1;
    }

    cout << "cam open success!" << endl;
    cv::namedWindow("cam");
    cv::Mat img;

    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(-1, -1));

    cv::Mat bsmMOG2;
    cv::Ptr<cv::BackgroundSubtractor> pMOG2 = cv::createBackgroundSubtractorMOG2();

    //cv::Mat bsmKNN;
    //cv::Ptr<cv::BackgroundSubtractor> pKNN = cv::createBackgroundSubtractorKNN();

    std::vector<std::vector<cv::Point>> contours;

    std::vector<cv::Vec4i> hierarchy;

    std::vector<std::vector<cv::Point>>::iterator itc = contours.begin();
    std::vector<cv::Vec4i>::iterator itc_hierarchy = hierarchy.begin();

    for (;;) {
        cam.read(img); // 读帧
        if (img.empty()) break;
        imshow("cam", img); // 显示每一帧

        pMOG2->apply(img, bsmMOG2);

        cv::morphologyEx(bsmMOG2, bsmMOG2, cv::MORPH_OPEN, kernel);

        cv::findContours(bsmMOG2, contours, CV_RETR_EXTERNAL,
            CV_CHAIN_APPROX_SIMPLE);


        if (contours.size() < 5) {
            //cv::morphologyEx(bsmMOG2, bsmMOG2, cv::MORPH_CLOSE, kernel);
            for (int i = 0; i < contours.size(); ++i)
            {
                //判断轮廓大小
                if (cv::contourArea(contours[i]) < 600 && cv::contourArea(contours[i]) > 50)
                {
                    //获取轮廓矩形
                    cv::Rect rect = cv::boundingRect(contours[i]);
                    //在矩形的左上角点输出文字
                    //cv::putText(bsmMOG2, "pest", cv::Point(rect.x, rect.y),
                    //cv::FONT_HERSHEY_PLAIN, 2, cv::Scalar(255, 255, 255));
                    itc = contours.begin();
                    while (itc != contours.end()) {

                        cv::Moments mom = cv::moments(cv::Mat(*itc++));
                        cv::Point pt = cv::Point(mom.m10 / mom.m00, mom.m01 / mom.m00);
                        cv::circle(bsmMOG2, pt, 2, cv::Scalar(0, 0, 255), 2);
                        string text_x = std::to_string(pt.x);
                        string text_y = std::to_string(pt.y);
                        string text = "(" + text_x + ", " + text_y + ")";
                        cv::putText(bsmMOG2, text, cv::Point(pt.x + 10, pt.y + 10), cv::FONT_HERSHEY_PLAIN, 1.5, cv::Scalar::all(255), 1, 8, 0);

                    }

                }
            }
        }
        //printf("number of contours:%d", contours.size());
        //Sleep(50);
        cv::imshow("MOG2", bsmMOG2);

        if (cv::waitKey(5) == 'q') break; // 键入q停止
    }

    return 0;
}
