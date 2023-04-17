#include "picamera.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QPushButton>

camerashow1::camerashow1(QWidget* parent)
    : QWidget(parent), udpSocket(nullptr), timer(nullptr), displayFlag(false)
{
    // Create UDP sockets
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(22222);

    // Timer to update images at regular intervals
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &camerashow1::updateImage);
    timer->start(30);

    openButton = new QPushButton(tr("open"), this);
    closeButton = new QPushButton(tr("close"), this);
    connect(openButton, &QPushButton::clicked, this, &camerashow1::openDisplay);
    connect(closeButton, &QPushButton::clicked, this, &camerashow1::closeDisplay);
    openButton->setEnabled(true);
    closeButton->setEnabled(false);

    QVBoxLayout* layout = new QVBoxLayout;
    QLabel* label = new QLabel(this);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
    layout->addWidget(openButton);
    layout->addWidget(closeButton);
    setLayout(layout);
}

camerashow1::~camerashow1()
{
    if (udpSocket) {
        udpSocket->deleteLater();
    }
    if (timer) {
        timer->stop();
        timer->deleteLater();
    }
}

void camerashow1::readPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());

        // Convert to image and display
        image.loadFromData(reinterpret_cast<const uchar*>(datagram.constData()), datagram.size());
        if (image.isNull()) {
            qWarning() << "Failed to decode datagram.";
        }
        else {
            if (displayFlag) {
                // Display the image in the label
                static_cast<QLabel*>(layout()->itemAt(0)->widget())->setPixmap(QPixmap::fromImage(image).scaled(640, 480, Qt::KeepAspectRatio));
            }
        }
    }
}

void camerashow1::updateImage()
{
    readPendingDatagrams();
}

void camerashow1::openDisplay()
{
    openButton->setEnabled(false);
    closeButton->setEnabled(true);
    displayFlag = true;
}

void camerashow1::closeDisplay()
{
    openButton->setEnabled(true);
    closeButton->setEnabled(false);
    displayFlag = false;
}
