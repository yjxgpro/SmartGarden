#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <QLabel>
#include <QTimer>
#include <iostream>
#include <QHBoxLayout>

using namespace cv;
using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    QLabel* label1;
    QLabel* label2;
    QFrame* frame1;
    QFrame* frame2;
    QWidget* centralWidget;
    QHBoxLayout* layout;
    cv::VideoCapture cap;
    QTimer* timer1;
    int pos_x;
    int pos_y;
    Ptr<BackgroundSubtractor> pMOG2 = createBackgroundSubtractorMOG2(500, 25, false);
    
    

private slots:
    void updateFrame();
};

#endif // MAINWINDOW_H