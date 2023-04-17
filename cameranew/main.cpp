#include <opencv2/opencv.hpp>
#include <QUdpSocket>
#include <QByteArray>


using namespace cv;

int main()
{
    // 打开摄像头
    VideoCapture cap(0);
    if (!cap.isOpened())
    {
        printf("Error opening camera.\n");
        return -1;
    }

    // 创建UDP套接字
    QUdpSocket udpSocket;
    QHostAddress host("192.168.137.1"); // 主机的IP地址
    quint16 port = 22222; // 主机端口

    // 持续捕获、编码和发送视频流
    while (true)
    {
        // 捕获一帧
        Mat frame;
        cap >> frame;

        // 将帧转换为JPEG格式
        std::vector<uchar> buffer;
        std::vector<int> params = { cv::IMWRITE_JPEG_QUALITY, 80 };
        cv::imencode(".jpg", frame, buffer, params);

        // 将JPEG数据发送到主机
        QByteArray datagram(reinterpret_cast<const char*>(buffer.data()), buffer.size());
        udpSocket.writeDatagram(datagram, host, port);


// cv::namedWindow("client", cv::WINDOW_AUTOSIZE);
// cv::imshow("client", frame);
// cv::waitKey(1);

        // 按下ESC键退出循环
        if (waitKey(1) == 27)
            break;
    }

    return 0;
}
