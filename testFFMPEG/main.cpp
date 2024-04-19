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
 * @LastEditTime : 2024-04-19 15:50:09
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
#include "libavutil/imgutils.h"
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
static void SaveFrame(AVFrame *pFrame, int width, int height, int iFrame)
{
    FILE *pFile = NULL;
    char szFilename[32] = {0};
    int y;

    //打开文件
    snprintf(szFilename, sizeof(szFilename), "frame%d.ppm", iFrame);
    pFile = fopen(szFilename, "wb");

    if (pFile == NULL) {
        printf("szFile open failed!");
        return;
    }

    // 写入文件
    fprintf(pFile, "P6 %d %d 255", width, height);

    // 写入数据
    for (y = 0l; y < height; y++)
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
    std::string file_name = "/Users/mcd/Desktop/My_Github/QT_Projects/testFFMPEG/test.mp4";

    AVFormatContext *pFormatCtx = NULL;
    AVCodecContext *pCodecCtx = NULL;
    AVCodecParameters *pCodecPar = NULL;
    AVCodec *pCodec = NULL;
    AVFrame *pFrame = NULL, *pFrameRGB = NULL;
    AVPacket *packet = NULL;
    uint8_t *out_buffer = NULL;

    static struct SwsContext *img_convert_ctx = NULL;

    int videoStream = -1, i = 0, numBytes = 0;
    int ret = 0;

    unsigned version = avcodec_version();
    cout << "version is:" << version << endl;

#if 0
    void *ofmt_opaque = NULL;
    const AVOutputFormat *ofmt = NULL;

    while ((ofmt = av_muxer_iterate(&ofmt_opaque))) {
        printf("short name:%-30s  long name:%-50s\n", ofmt->name, ofmt->long_name);
    }

    // av_register_all();  // 初始化FFMPEG, 这个函数在4.0以后就被弃用了
#endif
    cout << "input stream name: " << file_name.c_str() << endl;

    pFormatCtx = avformat_alloc_context();
    if (avformat_open_input(&pFormatCtx, file_name.c_str(), NULL, NULL) != 0) {
        printf("Couldn't open input stream.\n");
        return -1;
    }

    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        printf("Couldn't find stream information.\n");
        return -1;
    }

    // 循环查找视频中包含的流信息，直到找到视频类型的流
    // 将其记录下来，保存到videoStream变量中
    // 这里现在只处理视频流，音频流先不管
    for (i = 0; i < pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStream = i;
            break;
        }
    }

    // 如果videoStream为-1，说明没找到视频流
    if (videoStream == -1) {
        printf("Didn't find a video stream.\n");
        return -1;
    }

    cout << "videoStream is:" << videoStream << endl;

    // 查找编码器
    pCodecPar = pFormatCtx->streams[videoStream]->codecpar;
    pCodec = (AVCodec *)avcodec_find_decoder(pCodecPar->codec_id);
    if (pCodec == NULL) {
        printf("Couldn't find decoder.\n");
        return -1;
    }

    pCodecCtx = avcodec_alloc_context3((const AVCodec *)pCodec);
    if (pCodecCtx == NULL) {
        printf("Couldn't allocate decoder context.\n");
        return -1;
    }

    if (avcodec_parameters_to_context(pCodecCtx, pCodecPar) >= 0) {
        printf("copy decoder parameters success\n");
    }
    else {
        printf("copy decoder parameters failed\n");
        return -1;
    }

    // 打开编码器
    if (avcodec_open2(pCodecCtx, (const AVCodec *)pCodec, NULL) < 0) {
        printf("Couldn't open decoder.\n");
        return -1;
    }

    pFrame = av_frame_alloc();
    pFrameRGB = av_frame_alloc();

    if (pFrame == NULL || pFrameRGB == NULL) {
        printf("Couldn't allocate video frame.\n");
        return -1;
    }

    img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);
    numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height, 1);

    out_buffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));
    if (out_buffer == NULL) {
        printf("Couldn't allocate output buffer.\n");
        return -1;
    }

    av_image_fill_arrays(pFrameRGB->data, pFrameRGB->linesize, out_buffer, AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height, 1);
    int y_size = pCodecCtx->width * pCodecCtx->height;

    packet = (AVPacket *)av_malloc(sizeof(AVPacket));  // 分配一个packet
    if (packet == NULL) {
        printf("Couldn't allocate packet.\n");
        return -1;
    }
    av_new_packet(packet, y_size);                        // 分配packet的数据
    av_dump_format(pFormatCtx, 0, file_name.c_str(), 0);  // 打印输出视频信息

    int index = 0;
    
    // printf("do save frame\n");
    while (1) {
        if (av_read_frame(pFormatCtx, packet) < 0) {
            printf("read frame error\n");
            goto end;  // 读取完毕
        }

        // printf("stream index = %d\n", packet->stream_index);
        if (packet->stream_index == videoStream) {
            // ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
            // printf("start decode file\n");
            ret = avcodec_send_packet(pCodecCtx, packet);
            if (ret < 0) {
                fprintf(stderr, "Error submitting the packet to the decoder\n");
                goto end;
            }
            while(ret >= 0) {
                ret = avcodec_receive_frame(pCodecCtx, pFrame);
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                    // printf("avcode receice frame failed ret = %d\n", ret);
                    break;
                }
                else if (ret < 0) {
                    fprintf(stderr, "Error during decoding\n");
                    goto end;;
                }

                sws_scale(img_convert_ctx,
                        (const uint8_t *const *)pFrame->data,
                        pFrame->linesize, 0, pCodecCtx->height,
                        pFrameRGB->data, pFrameRGB->linesize);
                SaveFrame(pFrameRGB, pCodecCtx->width, pCodecCtx->height, index++);
                // printf("save frame index = %d\n", index);
                if (index > 50) {
                    printf("save frame success\n");
                    goto end;
                }
            }
        }
    }
end:
    av_packet_unref(packet);
    sws_freeContext(img_convert_ctx);
    av_free(out_buffer);
    av_free(packet);
    av_frame_free(&pFrameRGB);
    av_frame_free(&pFrame);
    avcodec_free_context(&pCodecCtx);
    avformat_close_input(&pFormatCtx);

    return 0;
}
