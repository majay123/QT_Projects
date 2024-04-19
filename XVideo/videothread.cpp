#include "videothread.h"
#include "filter.h"
#include <QDebug>
using namespace cv;

//一号视频源
static VideoCapture cap1;

static bool isexit = false;

//保存视频
static VideoWriter vw;

VideoThread::VideoThread()
{
    start();
}

VideoThread::~VideoThread()
{
    mutex.lock();
    isexit = true;
    mutex.unlock();
}

bool VideoThread::Open(const std::string file)
{
    //线程上锁
    mutex.lock();
    bool re=cap1.open(file);
    mutex.unlock();
    if (!re)
            return re;
    fps = cap1.get(CAP_PROP_FPS);
    width = cap1.get(CAP_PROP_FRAME_WIDTH);
    height = cap1.get(CAP_PROP_FRAME_HEIGHT);
    if (fps <= 0) fps = 25;
    return true;
}

void VideoThread::run()
{
   Mat mat1;
   while(1)
   {
       mutex.lock();
       if (isexit)
       {
           mutex.unlock();
           break;
       }

       //视频打开失败
       if(!cap1.isOpened())
       {
           qDebug()<<"打开失败";
           mutex.unlock();
           msleep(5);
           continue;
       }

       //暂停处理
       if(!isPlay)
       {
           mutex.unlock();
           msleep(5);
           continue;
       }

       //没读到视频或者读完视频，读取帧，解码并颜色转换
       if(!cap1.read(mat1)||mat1.empty())    /*************读帧*************/
       {
           mutex.unlock();
           msleep(5);
           continue;
       }

       //信号：显示图像1
       ViewImage1(mat1);

       /*******************执行任务列表******************/
       Mat mat2 = mark;
       Mat des = Filter::Get()->filter(mat1,mat2);

       //信号：显示图像2
       ViewDes(des);

       //如果开始写视频
       if(isWrite)
       {
           //写入帧
           vw.write(des);
       }

       int s = 0;
       s = 1000 / fps;
       msleep(s);

       mutex.unlock();

   }
}

//返回当前播放位置
double VideoThread::GetPos()
{
    double pos=0;
    mutex.lock();
    if(!cap1.isOpened())
    {
        mutex.unlock();
        return pos;
    }

    double count = cap1.get(CAP_PROP_FRAME_COUNT);
    double cur = cap1.get(CAP_PROP_POS_FRAMES);

    if(count>0.001)
        pos = cur/count;

    mutex.unlock();
    return pos;
}

//根据时间跳转
bool VideoThread::Seek(int frame)
{
    mutex.lock();
    if(!cap1.isOpened())
    {
        mutex.unlock();
        return false;
    }
    //跳转视频中指定帧
    int re=cap1.set(CAP_PROP_POS_FRAMES,frame);
    mutex.unlock();
    return re;
}

bool VideoThread::Seek(double pos)
{
    //总帧数
    double count = cap1.get(CAP_PROP_FRAME_COUNT);
    int frame = pos*count;
    return Seek(frame);
}

//开始保存视频
bool VideoThread::StartSave(const string filename, int width, int height)
{
    mutex.lock();

    if(!cap1.isOpened())
    {
        mutex.unlock();
        return false;
    }

    if(width <= 0)
        width = cap1.get(CAP_PROP_FRAME_WIDTH);
    if(height <= 0)
        height = cap1.get(CAP_PROP_FRAME_HEIGHT);

    vw.open(filename,
            VideoWriter::fourcc('X','2','6','4'),
            this->fps,
            Size(width,height)
            );

    if(!vw.isOpened())
    {
        qDebug()<< "start save failed!";
        mutex.unlock();
        return false;
    }

    this->isWrite=true;
    mutex.unlock();
    return true;
}

//停止保存视频，写入视频帧的索引
void VideoThread::StopSave()
{
    qDebug()<<"停止导出";
    vw.release();
}











