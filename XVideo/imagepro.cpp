#include "imagepro.h"

using namespace cv;

ImagePro::ImagePro()
{

}

//设置图像属性
void ImagePro::set(Mat mat1,Mat mat2)
{
    if(mat1.empty()) return;
    this->src1 = mat1;
    this->src2 = mat2;
    this->src1.copyTo(des);
}

//设置亮度和对比度
//亮度：0~100
//对比度：1.0~3.0
void ImagePro::Gain(double bright,double contrast)
{
    if(des.empty()) return;
    //矩阵变换
    /* void convertTo( OutputArray m, int rtype, double alpha=1, double beta=0 ) const;
     * alpha – 尺度变换因子（可选）。
     * beta  – 附加到尺度变换后的值上的偏移量（可选）。
     * m     – 目标矩阵。如果m在运算前没有合适的尺寸或类型，将被重新分配。
     * rtype – 目标矩阵的类型。因为目标矩阵的通道数与源矩阵一样，所以rtype也可以看做是目标矩阵的位深度。如果rtype为负值，目标矩阵和源矩阵将使用同样的类型。
     * */
    des.convertTo(des, -1, contrast, bright);
}

//旋转对应角度
void ImagePro::Rotate90()
{
    if(des.empty()) return;
    rotate(des,des,ROTATE_90_CLOCKWISE);
}

void ImagePro::Rotate180()
{
    if(des.empty()) return;
    rotate(des,des,ROTATE_180);
}

void ImagePro::Rotate270()
{
    if(des.empty()) return;
    rotate(des,des,ROTATE_90_COUNTERCLOCKWISE);
}

//左右镜像
void ImagePro::FlipX()
{
    if(des.empty()) return;
    flip(des,des,0);
}

//上下镜像
void ImagePro::FlipY()
{
    if(des.empty()) return;
    flip(des,des,1);
}

//上下左右镜像
void ImagePro::FlipXY()
{
    if(des.empty()) return;
    flip(des,des,-1);
}

//图像尺寸
void ImagePro::Resize(int width,int height)
{
    if(des.empty()) return;
    resize(des,des,Size(width,height));
}

//图像金字塔
void ImagePro::PyDown(int count)
{
    if(des.empty()) return;
    for(int i=0;i<count;i++)
    {
        pyrDown(des,des);
    }
}

void ImagePro::PyUp(int count)
{
    if(des.empty()) return;
    for(int i=0;i<count;i++)
    {
        pyrUp(des,des);
    }
}

//水印
void ImagePro::Mark(int x,int y,double a)
{
    if(des.empty()) return;
    //如果没有进行水印的图片
    if(src2.empty()) return;
    //水印区域
    Mat rol = des(Rect(x,y,src2.cols,src2.rows));
    addWeighted(src2,a,rol,1-a,0,rol);
}







