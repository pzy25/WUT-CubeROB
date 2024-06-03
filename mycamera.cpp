


#include "mycamera.h"
#include "ui_mycamera.h"

myCamera::myCamera(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::myCamera)
{
    ui->setupUi(this);

    initCamera(cap1, 2);
    initCamera(cap2, 1);
    initCamera(cap3, 0);
    initCamera(cap4, 3);
    mytimer = new QTimer(this);
    connect(mytimer,&QTimer::timeout,this,&myCamera::updateFrames);
    mytimer->start(33); // 每秒30帧率
}

myCamera::~myCamera()
{
    if (cap1.isOpened()) cap1.release();
    if (cap2.isOpened()) cap2.release();
    if (cap3.isOpened()) cap3.release();
    if (cap4.isOpened()) cap4.release();
    delete ui;
}

void myCamera::updateFrames()
{
    cv::Mat frame1, frame2, frame3, frame4;

    if (cap1.isOpened()) {
        cap1 >> frame1;
        if (!frame1.empty()) {
            showFrame(frame1, ui->labelCamera1);
        }
    }

    if (cap2.isOpened()) {
        cap2 >> frame2;
        if (!frame2.empty()) {
            showFrame(frame2, ui->labelCamera2);
        }
    }

    if (cap3.isOpened()) {
        cap3 >> frame3;
        if (!frame3.empty()) {
            showFrame(frame3, ui->labelCamera3);
        }
    }

    if (cap4.isOpened()) {
        cap4 >> frame4;
        if (!frame4.empty()) {
            showFrame(frame4, ui->labelCamera4);
        }
    }
}

void myCamera::showFrame(const Mat &frame, QLabel *label)
{
    cv::Mat resizedFrame;
    cv::resize(frame, resizedFrame, cv::Size(label->width(), label->height()));
    cv::cvtColor(resizedFrame, resizedFrame, cv::COLOR_BGR2RGB);
    QImage img((const unsigned char*)(resizedFrame.data), resizedFrame.cols, resizedFrame.rows, QImage::Format_RGB888);
    label->setPixmap(QPixmap::fromImage(img));
}

void myCamera::initCamera(VideoCapture &cap, int index)
{
    cap.open(index);
    if (cap.isOpened()) {
        cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
        cap.set(cv::CAP_PROP_FRAME_WIDTH, 320); // 设置分辨率
        cap.set(cv::CAP_PROP_FRAME_HEIGHT, 240);
        cap.set(cv::CAP_PROP_FPS, 15); // 设置帧率

        // 打印实际设置的参数
        std::cout << "Camera " << index << " settings:" << std::endl;
        std::cout << "Width: " << cap.get(cv::CAP_PROP_FRAME_WIDTH) << std::endl;
        std::cout << "Height: " << cap.get(cv::CAP_PROP_FRAME_HEIGHT) << std::endl;
        std::cout << "FPS: " << cap.get(cv::CAP_PROP_FPS) << std::endl;
    } else {
        qDebug() << "Failed to open camera" << index;
    }
}
