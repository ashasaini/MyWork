// VFCWriter.h: interface for the CVFCWriter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VFCWRITER_H__6C5353CD_5BE7_43F5_A0C0_97545B856BE6__INCLUDED_)
#define AFX_VFCWRITER_H__6C5353CD_5BE7_43F5_A0C0_97545B856BE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#ifdef V35M
extern "C" {
#include <tami/libvfToUi/vfToUiprotoVf.h>
}
#endif

#include "VFChandler.h"

class CVFCWriter : public CVFChandler  
{
public:
	CVFCWriter();
	virtual ~CVFCWriter();
    int OpenDBFile();
    int CloseDBFile();
    int createNewProtocols(int srcIndx, int &dstP);
    int PutData(void *DataBuffer, 
                VfPathElem_t pathP[], 
                int img_size, int data_type, int channel);
};

#endif // !defined(AFX_VFCWRITER_H__6C5353CD_5BE7_43F5_A0C0_97545B856BE6__INCLUDED_)
