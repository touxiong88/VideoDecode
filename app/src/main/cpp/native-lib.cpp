//
// Created by Administrator on 2017/11/22.
//
#include "com_shz_videodecode_Player.h"
#include <jni.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <android/log.h>
#include <android/native_window_jni.h>
#include <android/native_window.h>


#define LOGI(FORMAT,...) __android_log_print(ANDROID_LOG_INFO,"shz",FORMAT,##__VA_ARGS__);
#define LOGE(FORMAT,...) __android_log_print(ANDROID_LOG_ERROR,"shz",FORMAT,##__VA_ARGS__);

#if 0

/*解码绘制*/
JNIEXPORT void JNICALL Java_com_shz_videodecode_Player_play
(JNIEnv *env, jclass jcls, jstring path_jstr, jobject surface){

    const char* path_str = (*env)->GetStringUTFChars(env,path_jstr,NULL);
/*注册组件*/
    av_register_all();
    AVFormatContext *fmt_ctx = avformat_alloc_context();//封装格式的上下文
    av_format_open_input(fmt_ctx,path_cstr,NULL,NULL);
    avfomat_find_stream_info(fmt_ctx);//获取信息

    /*获取视频流的索引位置*/
    int i,video_stream_idx= -1;
    for(;fmt_ctx->nb_streams;i++){
        if(fmt_ctx->stream[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO){
            video_stream_idx = i;
        }
    }
    /*huoqu shipinliu jiemaqi jiema*/
    AVCodecContext *codec_ctx = fmt_ctx->stream[video_stream_idx]->codec;
    AVCodec *codec = avcodec_find_decoder(codec_ctx->codec_id);
    //dakai jiemaqi
    avcodec_open2(codec_ctx,codec,NULL);

    //
    AVPacket *packet = (AVPacket*) av_malloc(sizeof(AVPacket));//yasuo shuju (bianma shuju)
    AVFrame *yuv_frame = av_frame_alloc()//jiema shuju

    ANativeWindow* nativeWindow = ANativeWindow_fromSurface(env,surface);
    //绘制时的缓冲区
    ANativeWindow_Buffer outBuffer;
    int got_frame;
    int frame_count = 0;
    while(av_read_frame(fmt_ctx,packet) >= 0){
        if(packet->stream_index == video_stream_idx){
            //视频解码
            avcode_decode_video2(codec_ctx,yuv_frame,&got_frame,packet);
            if(got_frame){// jiema chenggong
                //lock
                //设置缓冲区 的属性 (width height pixel format)
                ANativeWindow_setBuffersGeometry(nativeWindow,codec_ctx->width,codec_ctx->height,WINDOW_FORMAT_RGBA_8888);
                ANativeWindow_lock(nativeWindow,&outBuffer,Null);

                //设置rgb_frame缓冲区 的属性 (width height pixel format)
                //rgb_frame 缓冲区与outBuffer.bits是同一块内存
                avpicture_fill((AVPicture *)rgb_frame,outBuffer.bits,AV_PIX_FMT_RGBA,codec_ctx->width,codec_ctx->height);
                //绘制
                //yuv->RGBA_8888
                I420ToARGB( yuv_frame->data[0],yuv_frame->linesize[0],
                            yuv_frame->data[2],yuv_frame->linesize[2],
                            yuv_frame->data[1],yuv_frame->linesize[1],
                            yuv_frame->data[0],yuv_frame->linesize[0],
                            codec_ctx->width,codec_ctx->height);


                ANativeWindow_unlockAndPost(naticeWindow);
                usleep(16*1000);
                LOGI("frame: %d",frame_count++);
            }
        }

    }


    (*env)->ReleaseStringUTFChars(env,path_jstr,path_cstr);

}



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "avutil.h"
#include "frame.h"


#define INBUF_SIZE 4096

static void pgm_save(unsigned char *buf, int wrap, int xsize, int ysize,
                     char *filename)
{
    FILE *f;
    int i;

    f = fopen(filename,"w");
    fprintf(f, "P5\n%d %d\n%d\n", xsize, ysize, 255);
    for (i = 0; i < ysize; i++)
        fwrite(buf + i * wrap, 1, xsize, f);
    fclose(f);
}

static void decode(AVCodecContext *dec_ctx, AVFrame *frame, AVPacket *pkt,
                   const char *filename)
{
    char buf[1024];
    int ret;

    ret = avcodec_send_packet(dec_ctx, pkt);
    if (ret < 0) {
        fprintf(stderr, "Error sending a packet for decoding\n");
        exit(1);
    }

    while (ret >= 0) {
        ret = avcodec_receive_frame(dec_ctx, frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return;
        else if (ret < 0) {
            fprintf(stderr, "Error during decoding\n");
            exit(1);
        }

        printf("saving frame %3d\n", dec_ctx->frame_number);
        fflush(stdout);

        /* the picture is allocated by the decoder. no need to
           free it */
        snprintf(buf, sizeof(buf), "%s-%d", filename, dec_ctx->frame_number);
        pgm_save(frame->data[0], frame->linesize[0],
                 frame->width, frame->height, buf);
    }
}

int main(int argc, char **argv)
{
    const char *filename, *outfilename;
    const AVCodec *codec;
    AVCodecParserContext *parser;
    AVCodecContext *c= NULL;
    FILE *f;
    AVFrame *frame;
    uint8_t inbuf[INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE];
    uint8_t *data;
    size_t   data_size;
    int ret;
    AVPacket *pkt;

    if (argc <= 2) {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        exit(0);
    }
    filename    = argv[1];
    outfilename = argv[2];

    avcodec_register_all();

    pkt = av_packet_alloc();
    if (!pkt)
        exit(1);

    /* set end of buffer to 0 (this ensures that no overreading happens for damaged MPEG streams) */
    memset(inbuf + INBUF_SIZE, 0, AV_INPUT_BUFFER_PADDING_SIZE);

    /* find the MPEG-1 video decoder */
    codec = avcodec_find_decoder(AV_CODEC_ID_MPEG1VIDEO);
    if (!codec) {
        fprintf(stderr, "Codec not found\n");
        exit(1);
    }

    parser = av_parser_init(codec->id);
    if (!parser) {
        fprintf(stderr, "parser not found\n");
        exit(1);
    }

    c = avcodec_alloc_context3(codec);
    if (!c) {
        fprintf(stderr, "Could not allocate video codec context\n");
        exit(1);
    }

    /* For some codecs, such as msmpeg4 and mpeg4, width and height
       MUST be initialized there because this information is not
       available in the bitstream. */

    /* open it */
    if (avcodec_open2(c, codec, NULL) < 0) {
        fprintf(stderr, "Could not open codec\n");
        exit(1);
    }

    f = fopen(filename, "rb");
    if (!f) {
        fprintf(stderr, "Could not open %s\n", filename);
        exit(1);
    }

    frame = av_frame_alloc();
    if (!frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }

    while (!feof(f)) {
        /* read raw data from the input file */
        data_size = fread(inbuf, 1, INBUF_SIZE, f);
        if (!data_size)
            break;

        /* use the parser to split the data into frames */
        data = inbuf;
        while (data_size > 0) {
            ret = av_parser_parse2(parser, c, &pkt->data, &pkt->size,
                                   data, data_size, AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
            if (ret < 0) {
                fprintf(stderr, "Error while parsing\n");
                exit(1);
            }
            data      += ret;
            data_size -= ret;

            if (pkt->size)
                decode(c, frame, pkt, outfilename);
        }
    }

    /* flush the decoder */
    decode(c, frame, NULL, outfilename);

    fclose(f);

    av_parser_close(parser);
    avcodec_free_context(&c);
    av_frame_free(&frame);
    av_packet_free(&pkt);

    return 0;
}





#endif