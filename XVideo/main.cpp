#include "xvideo.h"
#include "videothread.h"
#include "videowidget.h"
#include <QApplication>
using namespace cv;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    XVideo w;
    w.show();

    return a.exec();
}
