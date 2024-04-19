#ifndef VIDEOWIDGET
#define VIDEOWIDGET

#include<QOpenGLWidget>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <QImage>
#include <QObject>

class VideoWidget:public QOpenGLWidget
{
    Q_OBJECT

public:
    VideoWidget(QWidget *p);
    void paintEvent(QPaintEvent *e);
    virtual ~VideoWidget();

public slots:
    void SetImage(cv::Mat mat);

protected:
    QImage img;
};



#endif // VIDEOWIDGET






