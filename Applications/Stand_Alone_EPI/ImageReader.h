// ImageReader.h: interface for the CImageReader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEREADER_H__548040A1_2938_4E81_BF81_FEE3BD57D76D__INCLUDED_)
#define AFX_IMAGEREADER_H__548040A1_2938_4E81_BF81_FEE3BD57D76D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#ifndef NORMAL
#define NORMAL          0
#endif

#ifndef ERROR
#define ERROR           -1
#endif

#include "ImgFileHandler.h"

class CImageReader : public CImgFileHandler
{
public:
    CImageReader();
    virtual ~CImageReader();

};

#endif // !defined(AFX_IMAGEREADER_H__548040A1_2938_4E81_BF81_FEE3BD57D76D__INCLUDED_)
