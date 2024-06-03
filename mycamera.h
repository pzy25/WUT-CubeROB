#ifndef MYCAMERA_H
#define MYCAMERA_H

#include <QWidget>
#include <QDebug>
#include <QTimer>
#include <QLabel>
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;

QT_BEGIN_NAMESPACE
namespace Ui {
class myCamera;
}
QT_END_NAMESPACE

class myCamera : public QWidget
{
    Q_OBJECT

public:
    myCamera(QWidget *parent = nullptr);
    ~myCamera();

private slots:
    void updateFrames(); // 更新所有摄像头帧

private:
    Ui::myCamera *ui;
    VideoCapture cap1, cap2, cap3, cap4;
    QTimer *mytimer;
    void showFrame(const cv::Mat &frame, QLabel *label);
    void initCamera(VideoCapture &cap, int index);


};
#endif // MYCAMERA_H
