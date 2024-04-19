#ifndef IMAGEPRO_H
#define IMAGEPRO_H
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <QThread>
#include <QObject>


class ImagePro:public QObject
{
    Q_OBJECT

public:
    ImagePro();

    //设置原图，会清理之前处理结果
    void set(cv::Mat mat1,cv::Mat mat2);

    //获取目标图
    cv::Mat Get() { return des; }

    //设置亮度和对比度
    void Gain(double bright,double contrast);

    //旋转对应角度
    void Rotate90();
    void Rotate180();
    void Rotate270();

    //进行镜像
    void FlipX();
    void FlipY();
    void FlipXY();

    //图像尺寸
    void Resize(int width,int height);

    //图像金字塔
    void PyDown(int count);
    void PyUp(int count);

    //水印
    void Mark(int x,int y,double a);

private:
    //原图
    cv::Mat src1, src2;

    //目标图
    cv::Mat des;
};

#endif // IMAGEPRO_H
