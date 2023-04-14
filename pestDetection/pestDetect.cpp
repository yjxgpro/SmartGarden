#include "camera.h"


void Camera::registerCallback(ServoCallback* cb) {
    sCallback = cb;
}

void Camera::detect() {
    int pos[2];
    VideoCapture cam(0);
    if (!cam.isOpened()) {
        cout << "cam open failed!" << endl;
        getchar();
        exit;
    }

    cout << "cam open success!" << endl;
    namedWindow("cam");
    Mat img;

    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));

    Mat bsmMOG2;
    Ptr<BackgroundSubtractor> pMOG2 = createBackgroundSubtractorMOG2(500, 25, false);

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    vector<vector<Point>>::iterator itc = contours.begin();
    vector<Vec4i>::iterator itc_hierarchy = hierarchy.begin();

    for (;;) {
        cam.read(img);
        if (img.empty()) break;

        imshow("cam", img);

        pMOG2->apply(img, bsmMOG2, 0.005);

        morphologyEx(bsmMOG2, bsmMOG2, MORPH_OPEN, kernel);
        erode(bsmMOG2, bsmMOG2, kernel);

        findContours(bsmMOG2, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        if (contours.size() < 5) {
            for (int i = 0; i < contours.size(); ++i) {
                if (contourArea(contours[i]) < 600 && contourArea(contours[i]) > 10) {
                    Rect rect = boundingRect(contours[i]);
                    itc = contours.begin();
                    while (itc != contours.end()) {
                        Moments mom = moments(Mat(*itc++));
                        Point pt = Point(mom.m10 / mom.m00, mom.m01 / mom.m00);
                        circle(bsmMOG2, pt, 2, Scalar(0, 0, 255), 2);
                        pos_x = pt.x;
                        pos_y = pt.y;
                        sCallback->hasSample(pos_x, pos_y);
                        pos[0] = pos_x;
                        pos[1] = pos_y;
                        string text_x = std::to_string(pos_x);
                        string text_y = std::to_string(pos_y);
                        string text = "(" + text_x + ", " + text_y + ")";
                        putText(bsmMOG2, text, Point(pt.x + 10, pt.y + 10), FONT_HERSHEY_PLAIN, 1.5, Scalar::all(255), 1, 8, 0);
                    }
                }
            }
        }
        
        imshow("MOG2", bsmMOG2);

        if (waitKey(5) == 'q') break;
    }

        
};


