QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mycamera.cpp \
    mycube.cpp

HEADERS += \
    mycamera.h \
    mycube.h

FORMS += \
    mycamera.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += C:\Qt-OpenCv\OpenCV-Build\install\include
LIBS += C:\Qt-OpenCv\OpenCV-Build\lib\libopencv_*.a

INCLUDEPATH +=C:\eigen-3.4.0\eigen-3.4.0
