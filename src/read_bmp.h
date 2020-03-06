#ifndef READ_BMP_H
#define READ_BMP_H
#include <string>
#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <cstdio>
#include <stdlib.h>
using namespace std;
/*
用来读取bmp的，网上找的代码
 */
int read_bmp(string name,BYTE texture[][64][4]);

typedef long LONG;
typedef unsigned long DWORD;
typedef unsigned short WORD;

typedef struct {
    WORD    bfType;
    DWORD   bfSize;
    WORD    bfReserved1;
    WORD    bfReserved2;
    DWORD   bfOffBits;
} BMPFILEHEADER_T;

typedef struct {
    DWORD      biSize;
    LONG       biWidth;
    LONG       biHeight;
    WORD       biPlanes;
    WORD       biBitCount;
    DWORD      biCompression;
    DWORD      biSizeImage;
    LONG       biXPelsPerMeter;
    LONG       biYPelsPerMeter;
    DWORD      biClrUsed;
    DWORD      biClrImportant;
} BMPINFOHEADER_T;



#endif // READ_BMP_H
