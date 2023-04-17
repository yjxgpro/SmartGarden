#include "ui_picamera.h"
#include <QtWidgets/QWidget>
#include <QWidget>
#include <QUdpSocket>
#include <QTimer>
#include <QImage>
#include <QPushButton>

class camerashow1 : public QWidget
{
    Q_OBJECT;

public:
    camerashow1(QWidget* parent = nullptr);
    ~camerashow1();

private slots:
    void readPendingDatagrams();
    void updateImage();
    void openDisplay();
    void closeDisplay();

private:
    QUdpSocket* udpSocket;
    QTimer* timer;
    QImage image;
    QPushButton* openButton;
    QPushButton* closeButton;
    bool displayFlag;
};
