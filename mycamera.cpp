#include "mycamera.h"
#include "ui_mycamera.h"

myCamera::myCamera(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::myCamera)
{
    ui->setupUi(this);

    ui->pushButton_stop->setEnabled(false);
    initHSV();

    connect(ui->pushButton_save,&QPushButton::clicked,this,&myCamera::saveImages);
    connect(ui->pushButton_load,&QPushButton::clicked,this,&myCamera::loadImages);

    connect(ui->pushButton_start,&QPushButton::clicked,this,&myCamera::startVideo);
    connect(ui->pushButton_stop,&QPushButton::clicked,this,&myCamera::stopVideo);
    mytimer = new QTimer(this);
    connect(mytimer,&QTimer::timeout,this,&myCamera::updateFrames);
    connect(ui->pushButton_export,&QPushButton::clicked,this,&myCamera::exportDataToCSV);
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
    framesProcessed = 0;
    processAndShowFrame(cap1, loadedImages[0], myCube.rects[0], ui->labelCamera1,0);
    processAndShowFrame(cap2, loadedImages[1], myCube.rects[1], ui->labelCamera2,1);
    processAndShowFrame(cap3, loadedImages[2], myCube.rects[2], ui->labelCamera3,2);
    processAndShowFrame(cap4, loadedImages[3], myCube.rects[3], ui->labelCamera4,3);
}

void myCamera::processAndShowFrame(VideoCapture &cap, Mat &loadedImage, const QVector<Rect> &rectangles,
    QLabel *label,int Idx)
{
    cv::Mat frame;
    if (cap.isOpened()) {
        cap >> frame;
        if (frame.empty()) {
            qDebug() << "Captured empty frame.";
            return;
        }
    } else if (!loadedImage.empty()) {
        frame = loadedImage;
    }
    cv::Mat processedFrame = processHSVImage(frame);

    myCube.extractColors(processedFrame,rectangles,Idx);
    myCube.drawRectangles(processedFrame, rectangles);

    framesProcessed++; // 增加已处理的摄像头数量
    if(framesProcessed ==4)
    {
        myCube.copyColorsToCube();
    }
    showFrameToLabel(processedFrame, label);
}


void myCamera::showFrameToLabel(const Mat &frame, QLabel *label)
{
    cv::Mat resizedFrame;
    cv::resize(frame, resizedFrame, cv::Size(label->width(), label->height()));
    cv::cvtColor(resizedFrame, resizedFrame, cv::COLOR_BGR2RGB);
    QImage img((const unsigned char*)(resizedFrame.data), resizedFrame.cols, resizedFrame.rows, QImage::Format_RGB888);
    label->setPixmap(QPixmap::fromImage(img));
}

//******Camera********//
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

void myCamera::startVideo()
{
    initCamera(cap1, 1);
    initCamera(cap2, 3);
    initCamera(cap3, 0);
    initCamera(cap4, 2);

    mytimer->start(33); // 启动定时器，每秒30帧率
    ui->pushButton_start->setEnabled(false);
    ui->pushButton_stop->setEnabled(true);
}

void myCamera::stopVideo()
{
    mytimer->stop(); // 停止定时器

    if (cap1.isOpened()) cap1.release();
    if (cap2.isOpened()) cap2.release();
    if (cap3.isOpened()) cap3.release();
    if (cap4.isOpened()) cap4.release();

    ui->pushButton_start->setEnabled(true);
    ui->pushButton_stop->setEnabled(false);
}


// *********HSV********//
void myCamera::initHSV()
{
    hMin=0, hMax=180;
    sMin=0, sMax=255;
    vMin=0, vMax=255;
    ui->hMinSlider->setRange(0, 180);
    ui->hMaxSlider->setRange(0, 180);
    ui->sMinSlider->setRange(0, 255);
    ui->sMaxSlider->setRange(0, 255);
    ui->vMinSlider->setRange(0, 255);
    ui->vMaxSlider->setRange(0, 255);

    ui->hMinSlider->setValue(hMin);
    ui->hMaxSlider->setValue(hMax);
    ui->sMinSlider->setValue(sMin);
    ui->sMaxSlider->setValue(sMax);
    ui->vMinSlider->setValue(vMin);
    ui->vMaxSlider->setValue(vMax);

    connect(ui->hMinSlider, &QSlider::valueChanged, this, &myCamera::updateHSVRange);
    connect(ui->hMaxSlider, &QSlider::valueChanged, this, &myCamera::updateHSVRange);
    connect(ui->sMinSlider, &QSlider::valueChanged, this, &myCamera::updateHSVRange);
    connect(ui->sMaxSlider, &QSlider::valueChanged, this, &myCamera::updateHSVRange);
    connect(ui->vMinSlider, &QSlider::valueChanged, this, &myCamera::updateHSVRange);
    connect(ui->vMaxSlider, &QSlider::valueChanged, this, &myCamera::updateHSVRange);
}

void myCamera::updateHSVRange()
{
    hMin = ui->hMinSlider->value();
    hMax = ui->hMaxSlider->value();
    sMin = ui->sMinSlider->value();
    sMax = ui->sMaxSlider->value();
    vMin = ui->vMinSlider->value();
    vMax = ui->vMaxSlider->value();
    qDebug() << "hMin: " << hMin << " hMax: " << hMax;
    qDebug() << "sMin: " << sMin << " sMax: " << sMax;
    qDebug() << "vMin: " << vMin << " vMax: " << vMax;
    ui->Hmin->setText(QString::number(hMin));
    ui->Hmax->setText(QString::number(hMax));
    ui->Smin->setText(QString::number(sMin));
    ui->Smax->setText(QString::number(sMax));
    ui->Vmin->setText(QString::number(vMin));
    ui->Vmax->setText(QString::number(vMax));
}

void myCamera::exportDataToCSV()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save CSV", "C:/Users/庞子杨/Desktop/桌面/product/PyhtonVscode"
    , "CSV files (*.csv);;All files (*)");
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Could not open file for writing: " << fileName;
        return;
    }

    QTextStream out(&file);
    out << "Index,B,G,R\n"; // CSV header

    int index = 0;
    for (int i = 0; i < 4; ++i) {
        for (const Scalar &color : myCube.colors[i]) {
            out << index << "," << color[0] << "," << color[1] << "," << color[2] << "\n";
            ++index;
        }
    }

    file.close();
    qDebug() << "Data exported to: " << fileName;
}

Mat myCamera::processHSVImage(const Mat &image)
{
    Mat hsvImage, mask;
    // Mat blurredImage;
    // GaussianBlur(image, blurredImage, Size(19, 19), 0); // 添加高斯模糊
    cvtColor(image, hsvImage, COLOR_BGR2HSV); // 转换为HSV色彩空间
    inRange(hsvImage, Scalar(hMin, sMin, vMin), Scalar(hMax, sMax, vMax), mask);

    Mat result;
    bitwise_and(image, image, result, mask);
    return result;
}

void myCamera::saveImages()
{
    QString dir = "C:/QT6 code/Cube solver test/SolverImageTest0"; // 指定保存目录
    if (dir.isEmpty()) return;

    Mat frame;
    if (cap1.isOpened()) {
        cap1 >> frame;
        if (!frame.empty()) {
            imwrite((dir + "/camera1.jpg").toStdString(), frame);
        }
    }

    if (cap2.isOpened()) {
        cap2 >> frame;
        if (!frame.empty()) {
            imwrite((dir + "/camera2.jpg").toStdString(), frame);
        }
    }

    if (cap3.isOpened()) {
        cap3 >> frame;
        if (!frame.empty()) {
            imwrite((dir + "/camera3.jpg").toStdString(), frame);
        }
    }

    if (cap4.isOpened()) {
        cap4 >> frame;
        if (!frame.empty()) {
            imwrite((dir + "/camera4.jpg").toStdString(), frame);
        }
    }


}

void myCamera::loadImages()
{
    QStringList files = QFileDialog::getOpenFileNames(this, "Select Images to Load", "", "Images (*.png *.xpm *.jpg)");
    if (files.size() != 4) {
        qDebug() << "Please select exactly 4 images.";
        return;
    }

    for (int i = 0; i < files.size(); ++i) {
        Mat image = imread(files[i].toStdString());
        if (image.empty()) {
            qDebug() << "Failed to load image:" << files[i];
            continue;
        }
        loadedImages[i] = image; // 存储加载的图像
        switch (i) {
        case 0:
            showFrameToLabel(image, ui->labelCamera1);
            break;
        case 1:
            showFrameToLabel(image, ui->labelCamera2);
            break;
        case 2:
            showFrameToLabel(image, ui->labelCamera3);
            break;
        case 3:
            showFrameToLabel(image, ui->labelCamera4);
            break;
        }
    }
}
