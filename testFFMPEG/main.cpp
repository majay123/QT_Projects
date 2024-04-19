/*
 * 
 * 　　┏┓　　　┏┓+ +
 * 　┏┛┻━━━┛┻┓ + +
 * 　┃　　　　　　　┃ 　
 * 　┃　　　━　　　┃ ++ + + +
 *  ████━████ ┃+
 * 　┃　　　　　　　┃ +
 * 　┃　　　┻　　　┃
 * 　┃　　　　　　　┃ + +
 * 　┗━┓　　　┏━┛
 * 　　　┃　　　┃　　　　　　　　　　　
 * 　　　┃　　　┃ + + + +
 * 　　　┃　　　┃
 * 　　　┃　　　┃ +  神兽保佑
 * 　　　┃　　　┃    代码无bug　　
 * 　　　┃　　　┃　　+　　　　　　　　　
 * 　　　┃　 　　┗━━━┓ + +
 * 　　　┃ 　　　　　　　┣┓
 * 　　　┃ 　　　　　　　┏┛
 * 　　　┗┓┓┏━┳┓┏┛ + + + +
 * 　　　　┃┫┫　┃┫┫
 * 　　　　┗┻┛　┗┻┛+ + + +
 * 
 * 
 * 
 * ************Copyright 2024 MCD************
 * 
 * @version      : 
 * @Company      : HOPE
 * @Author       : MCD
 * @Date         : 2024-04-18 17:37:03
 * @LastEditors  : MCD
 * @LastEditTime : 2024-04-19 10:21:38
 * @FilePath     : /testFFMPEG/main.cpp
 * @Description  : 
 * 
 * ******************************************
 */

#include <iostream>

using namespace std;

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavdevice/avdevice.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
}

#include <stdio.h>

/**
 * @brief 现在我们需要做的是让SaveFrame函数把RGB信息定稿到一个PPM格式的文件中。
 * 我们将生产一个简单的PPM格式的文件。
 * @param pFrame 一个指向存储RGB信息的指针
 * @param width 图像的宽度
 * @param height 图像的高度
 * @param iFrame 图像的帧序号
 */
static void Save_frame(AVFrame *pFrame, int width, int height, int iFrame)
{
    FILE *pFile;
    char szFilename[32];
    int y;

    //打开文件
    snprintf(szFilename, sizeof(szFilename), "frame%d.ppm", iFrame);
    pFile = fopen(szFilename, "wb");

    if(pFile == NULL) {
        printf("szFile open failed!");
        return;
    }

    // 写入文件
    fprintf(pFile, "P6 %d %d 255", width, height);
    
    // 写入数据
    for(y = 0l; y < height; y++)
        fwrite(pFrame->data[0] + y * pFrame->linesize[0], 1, width * 3, pFile);
    
    // 关闭文件
    fclose(pFile);
    return;
}


/**
 * @brief 使用ffmpeg解码视频并保存文件
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char *argv[])
{
    
    // cout << "Hello World!" << endl;


    unsigned version = avcodec_version();
    cout << "version is:" << version << endl;
    return 0;
}
