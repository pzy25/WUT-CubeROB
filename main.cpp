#include "mycamera.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    myCamera w;
    w.show();
    return a.exec();
}
