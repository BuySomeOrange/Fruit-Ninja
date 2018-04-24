// MyEasyX.cpp: implementation of the CMyEasyX class.
//
//////////////////////////////////////////////////////////////////////
#include "easyx.h"
//#include "./cXimageLight/ximage.h"
#include "MyEasyX.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyEasyX::CMyEasyX()
{
	pngImage = NULL;
	angle = 0.0f;
	//hBmp = NULL;
}

CMyEasyX::~CMyEasyX()
{
	if(pngImage != NULL)
		delete pngImage;
	//if(hBmp)
	//	DeleteObject(hBmp);
}

int CMyEasyX::getWidth()
{
	return pngImage->GetWidth();
}

int CMyEasyX::getHeight()
{
	return pngImage->GetHeight();
}
