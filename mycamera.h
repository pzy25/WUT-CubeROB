#ifndef MYCAMERA_H
#define MYCAMERA_H

#include <QWidget>
#include <QDebug>
#include <QTimer>
#include <QLabel>
#include <QSlider>
#include <QString>
#include <QFileDialog>
#include <iostream>
#include "mycube.h"
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

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
    void initHSV();
    void showFrameToLabel(const cv::Mat &frame, QLabel *label);
    void initCamera(VideoCapture &cap, int index);
    Mat processHSVImage(const Mat& image);
    void processAndShowFrame(cv::VideoCapture &cap, cv::Mat &loadedImage, const QVector<Rect> &rectangles,
        QLabel *label,int Idex);

private slots:
    void updateFrames(); // 更新所有摄像头帧
    void updateHSVRange(); //更新HSV范围
    void exportDataToCSV();
    void saveImages();
    void loadImages();
    void startVideo();
    void stopVideo();

private:
    Ui::myCamera *ui;
    VideoCapture cap1, cap2, cap3, cap4;
    int hMin, hMax, sMin, sMax, vMin, vMax;
    Mat loadedImages[4]; // 用于存储加载的图像
    QTimer *mytimer;
    MyCube myCube;
    int framesProcessed=0; // 已处理的摄像头数量


};
#endif // MYCAMERA_H
