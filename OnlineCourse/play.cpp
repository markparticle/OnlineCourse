/*
 * @Author       : mark
 * @Date         : 2020-05-23
 * @copyleft GPL 2.0
 */

#include <Windows.h>
#include <cstdio>
#include <mmsystem.h>
#pragma comment (lib, "winmm.lib")

static char* pcmData = NULL;
static int offset = 0;
static int buffSize = 100000;

//回调函数
void CALLBACK waveOutProc(
    HWAVEOUT hwo,
    UINT uMsg,
    DWORD_PTR dwInstance,
    DWORD_PTR dwParam1,
    DWORD_PTR dwParam2)
{
    if (uMsg == WOM_OPEN) {
        printf("callback open\n");
    }
    else if (uMsg == WOM_CLOSE) {
        printf("callback close\n");
    }
    else {
        printf("callback %d(%d) \n", uMsg, WOM_DONE);
        LPWAVEHDR header = (LPWAVEHDR)dwParam1;
        header->lpData = pcmData + offset;
        offset += buffSize;
        header->dwBufferLength = buffSize;
        //waveOutPrepareHeader(hwo, header, sizeof(WAVEHDR));
        waveOutWrite(hwo, header, sizeof(WAVEHDR));
    }
}

//int main()
//{
//    int ret;
//    HWAVEOUT audioFd;
//    WAVEFORMATEX format;
//
//    //读文件
//    FILE* pcmFd;
//    fopen_s(&pcmFd, "F:\\pcm\\test.pcm", "rb");
//    const int size = 4000000;
//    pcmData = new char[size];
//    fread(pcmData, 1, size, pcmFd);
//
//    //init audio device
//    //格式类型
//    format.wFormatTag = WAVE_FORMAT_PCM;
//    //声道
//    format.nChannels = 1;
//    //采样率
//    format.nSamplesPerSec = 44000;
//    //每个声音样本的比特数
//    format.wBitsPerSample = 16;
//
//    format.nBlockAlign = format.nChannels * format.wBitsPerSample / 8;
//    //平均每秒的字节数（传输速度）
//    format.nAvgBytesPerSec = format.nChannels * format.nSamplesPerSec * format.wBitsPerSample / 8;
//
//    //打开声音设备
//    ret = waveOutOpen(
//        &audioFd,
//        WAVE_MAPPER,
//        &format,
//        (DWORD_PTR)waveOutProc, //回调函数
//        NULL,
//        CALLBACK_FUNCTION); //回调函数的标志
//
//    printf("waveOutOpen %d\n", ret);
//    //填数据
//    WAVEHDR header;
//    header.lpData = pcmData + offset;
//    offset += buffSize;
//    header.dwBufferLength = buffSize;
//    header.dwUser = NULL;
//    header.dwLoops = 0;
//    header.dwFlags = 0;
//    waveOutPrepareHeader(audioFd, &header, sizeof(header));
//    waveOutWrite(audioFd, &header, sizeof(header));
//
//    getchar();
//
//    return 0;
//}
