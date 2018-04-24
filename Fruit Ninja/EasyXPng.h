#ifndef MYEASYX_H
#define MYEASYX_H

#include "easyx.h"
#include "MyEasyX.h"

void initgraphEx();
void closegraphEx();
void loadimage(CMyEasyX * image, LPCTSTR pImgFile, int nWidth = 0, int nHeight = 0);
void putimage(int dstX, int dstY, CMyEasyX *image);
void rotateimage(CMyEasyX *dstimg, CMyEasyX *srcimg, float angle);

#endif