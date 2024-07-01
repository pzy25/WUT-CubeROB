#ifndef MYCUBE_H
#define MYCUBE_H
#include <QVector>
#include <QDebug>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

struct Cube
{
    Scalar scalarBGR[54];
};


class MyCube
{
public:
    MyCube();
    Cube cube;
    QVector<Rect> rects[4];
    Rect rect[54];
    char str[6] = { 'F', 'R', 'U', 'B', 'L', 'D' };
    QVector<Scalar> colors[4]; // 颜色数组

    double B1 = 35;
    double B2 = 132;

    void RectInit();
    Rect shrinkRect(const Rect& original,int borderWidth);
    void drawRectangles(const Mat &image, const QVector<Rect> &rectangles);
    void extractColors(const Mat &image,const QVector<Rect> &rectangles,int Idex);
    void copyColorsToCube();
    void getCubeData();
};

#endif // MYCUBE_H
