#ifndef XVIDEO_H
#define XVIDEO_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QMessageBox>
#include <QTimerEvent>
#include <QObject>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

namespace Ui {
class XVideo;
}

class XVideo : public QWidget
{

    Q_OBJECT

public:
    explicit XVideo(QWidget *parent = 0);
    ~XVideo();
    void timerEvent(QTimerEvent *e);

public slots:
    //打开视频1按钮信号
    void Open();
    void SliderPress();
    void SliderRelease();
    //滑动条拖动
    void SetPos(int);
    //设置过滤器
    void Set();
    //导出视频
    void Export();

    void Play();
    void Pause();

    void Mark();
private:
    Ui::XVideo *ui;
};

#endif // XVIDEO_H
