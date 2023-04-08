#include "mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent)
{
    resize(1600, 960);

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    layout = new QHBoxLayout(centralWidget);

    frame1 = new QFrame();
    frame1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    frame1->setFixedSize(800, 600);
    layout->addWidget(frame1);

    label1 = new QLabel(frame1);
    label1->setGeometry(0, 0, frame1->width(), frame1->height());

    frame2 = new QFrame();
    frame2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    frame2->setFixedSize(800, 600);
    layout->addWidget(frame2);

    label2 = new QLabel(frame2);
    label2->setGeometry(0, 0, frame2->width(), frame2->height());


    // Open the default camera
    cap.open(0);

    // Check if camera opened successfully
    if (!cap.isOpened())
    {
        qDebug() << "Error opening camera";
        exit(EXIT_FAILURE);
    }

    timer1 = new QTimer(this);
    connect(timer1, SIGNAL(timeout()), this, SLOT(updateFrame()));
    timer1->start(33);

}

MainWindow::~MainWindow()
{
}

void MainWindow::updateFrame()
{
    Mat frame;
    Mat bsmMOG2;
    Mat kernel;

    cap.read(frame);
    kernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
    
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    vector<vector<Point>>::iterator itc = contours.begin();
    vector<Vec4i>::iterator itc_hierarchy = hierarchy.begin();
    
    pMOG2->apply(frame, bsmMOG2, 0.005);
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
                    string text_x = std::to_string(pos_x);
                    string text_y = std::to_string(pos_y);
                    string text = "(" + text_x + ", " + text_y + ")";
                    putText(bsmMOG2, text, Point(pt.x + 10, pt.y + 10), FONT_HERSHEY_PLAIN, 1.5, Scalar::all(255), 1, 8, 0);
                }
            }
        }
    }
    QImage qframe(frame.data, frame.cols, frame.rows, QImage::Format_RGB888);
    qframe = qframe.rgbSwapped();

    QVector<QRgb> colorTable;
    for (int i = 0; i < 256; ++i)
        colorTable.push_back(qRgb(i, i, i));
    QImage qframe1(bsmMOG2.data, bsmMOG2.cols, bsmMOG2.rows, bsmMOG2.step, QImage::Format_Grayscale8);
    qframe1.setColorTable(colorTable);


    label1->setPixmap(QPixmap::fromImage(qframe));
    label1->setScaledContents(true);
    
    // Set the QImage to the QLabel in the Qt interface
    label2->setPixmap(QPixmap::fromImage(qframe1));
    label2->setScaledContents(true);
}
