#include "read_bmp.h"
#include <string>
#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <cstdio>
#include <stdlib.h>
#include <cstring>
using namespace std;

/**
读取贴图文件，读入内存中的一个二维数组中。
name 贴图文件的绝对路径
texture 一个三维数组，读入的贴图文件保存在该块内存中。前两维表示坐标，后一维表示颜色
 */
int read_bmp(string name,BYTE texture[][64][4]) {
    BYTE* DDBdata = new BYTE[640 * 640 * 3];
    //    BYTE* DIBdata;
    WORD width = 128;
    WORD height = 128;
    int te, pe, cn;

    BITMAPINFOHEADER   bih = { 0 };//
    BITMAPFILEHEADER   bfh = { 0 };//

    FILE * fp;
    errno_t err;
    printf("reading %s\n\n",name.c_str());


//    system("dir");
    fp = fopen(name.c_str(), "rb");
    if (!fp){
        printf("error reading bmp\n");
        return 0;
    }
//
    fread(&bfh, 8, 1, fp);//
    fread(&bfh.bfReserved2, sizeof(bfh.bfReserved2), 1, fp);
    fread(&bfh.bfOffBits, sizeof(bfh.bfOffBits), 1, fp);
    fread(&bih, sizeof(BMPINFOHEADER_T), 1, fp);
    fseek(fp,0,SEEK_SET);

    fread(DDBdata, bfh.bfOffBits, 1, fp);
    fread(DDBdata, bih.biWidth*bih.biHeight * 4, 1, fp);
//    printf("read3\n");
    for (te = 0; te<64; te++)
    {
        for (pe = 0; pe<64 * 4; pe++)
        {

            if(pe%4==3){//透明度设置
                texture[te][pe/4][3]=0;
                for(int i=0;i<3;i++){
                    texture[te][pe/4][3]+=texture[te][pe/4][i]/3;
                }
                //保证颜色足够 todo:这东西可能还得改才好看。。
                if(texture[te][pe/4][3]*5<=255){
                        texture[te][pe/4][3]*=5;
                }else{
                    texture[te][pe/4][3]=255;
                }
            }else{
                texture[te][pe/4][pe%4]=DDBdata[te * 64 * 4 + pe];
            }
        }
    }
    fclose(fp);
    delete[] DDBdata;
    return 0;

}
