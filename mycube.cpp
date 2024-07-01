#include "mycube.h"

MyCube::MyCube()
{
    RectInit();
    for(int i=0;i<4;i++)
    {
        colors[i].resize(18); // 每个摄像头最多 18 个颜色块
    }
}

void MyCube::RectInit()
{
    rects[0] = {
        Rect(115, 6, 10, 10),
        Rect(170, 6, 10, 10),
        Rect(226, 6, 10, 10),
        Rect(113, 40, 10, 10),
        Rect(170, 40, 10, 10),
        Rect(236, 40, 10, 10),
        Rect(110, 80, 10, 10),
        Rect(170, 80, 10, 10),
        Rect(245, 80, 10, 10),
        //9个
        Rect(110, 130, 10, 10),
        Rect(173, 130, 10, 10),
        Rect(245, 130, 10, 10),
        Rect(112, 170, 10, 10),
        Rect(173, 170, 10, 10),
        Rect(240, 170, 10, 10),
        Rect(120, 200, 10, 10),
        Rect(173, 200, 10, 10),
        Rect(236, 200, 10, 10)
    };

    rects[1] = {
        // 另外9个矩形框的位置
        Rect(60, 132, 10, 10),
        Rect(95, 95, 10, 10),
        Rect(130, 55, 10, 10),
        Rect(95, 170, 10, 10),
        Rect(130, 132, 10, 10),
        Rect(165, 95, 8, 8),
        Rect(135, 210, 10, 10),
        Rect(168, 160, 8, 8),
        Rect(205, 132, 10, 10)
    };

    rects[2] = {
        Rect(110, 6, 8, 8),
        Rect(170, 6, 10, 10),
        Rect(226, 6, 10, 10),
        Rect(100, 40, 10, 10),
        Rect(170, 40, 10, 10),
        Rect(236, 40, 10, 10),
        Rect(90, 80, 10, 10),
        Rect(170, 80, 10, 10),
        Rect(245, 80, 10, 10),
        //9个
        Rect(90, 135, 10, 10),
        Rect(165, 140, 10, 10),
        Rect(240, 140, 10, 10),
        Rect(90, 180, 10, 10),
        Rect(165, 185, 10, 10),
        Rect(230, 185, 10, 10),
        Rect(95, 220, 10, 10),
        Rect(165, 220, 10, 10),
        Rect(225, 220, 10, 10)
    };

    rects[3] = {
        Rect(95, 150, 10, 10),
        Rect(130, 128, 8, 8),
        Rect(165, 75, 10, 10),
        Rect(135, 170, 8, 8),
        Rect(165, 145, 10, 10),
        Rect(205, 105, 10, 10),
        Rect(170, 215, 10, 10),
        Rect(205, 175, 10, 10),
        Rect(235, 140, 10, 10)
    };
    qDebug()<<rects[0].size()<<Qt::endl;
}

Rect MyCube::shrinkRect(const Rect &original, int borderWidth)
{
    int shrinkBy = borderWidth;
    int newX = original.x + shrinkBy;
    int newY = original.y + shrinkBy;
    int newWidth = original.width - 2 * shrinkBy;
    int newHeight = original.height - 2 * shrinkBy;

    // 确保新的矩形仍在图像范围内
    if (newWidth > 0 && newHeight > 0) {
        return Rect(newX, newY, newWidth, newHeight);
    }
    return original; // 如果边框过宽导致新矩形不合理，则返回原矩形
}

void MyCube::drawRectangles(const Mat &image, const QVector<Rect> &rectangles)
{
    if (image.empty()) {
        qDebug() << "Empty image, cannot draw rectangles.";
        return;
    }
    for (int i = 0; i < rectangles.size(); ++i)
    {
        cv::rectangle(image, rectangles[i], Scalar(0, 0, 255), 1, LINE_8, 0);
    }
}

void MyCube::extractColors(const Mat &image, const QVector<Rect> &rectangles, int Idex)
{
    if (image.empty()) {
        qDebug() << "Empty image, cannot extract colors.";
        return;
    }
    colors[Idex].resize(rectangles.size());
    for (int i = 0; i < rectangles.size(); ++i)
    {
        Rect shrinkedRect = shrinkRect(rectangles[i],1);
        Mat roi = image(shrinkedRect); // 获取矩形区域
        //cout << roi <<endl;
        Scalar color = mean(roi); // 计算平均颜色
        //cout << "[" << Idex << "][" << i << "]: (" << color[0] << "," << color[1] << "," << color[2] << ")"<<endl;
        colors[Idex][i] = color; // 保存颜色
    }
}

void MyCube::copyColorsToCube()
{
    int index = 0;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < rects[i].size(); ++j) {
            if (index >= 54) break; // 防止数组越界
            //将bgr保留一位小数
            double b = round(colors[i][j][0] * 10) / 10.0;
            double g = round(colors[i][j][1] * 10) / 10.0;
            double r = round(colors[i][j][2] * 10) / 10.0;

            cube.scalarBGR[index] = cv::Scalar(b, g, r);

            QString colorStr = QString("(%1, %2, %3)")
                                   .arg(cube.scalarBGR[index][0])
                                   .arg(cube.scalarBGR[index][1])
                                   .arg(cube.scalarBGR[index][2]);
            qDebug()<< "[" << i << "][" << j << "]"<<colorStr;

            index++;
        }
    }
}

void MyCube::getCubeData()
{

}
