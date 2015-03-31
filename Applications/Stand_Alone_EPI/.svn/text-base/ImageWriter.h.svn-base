// ImageWriter.h: interface for the CImageWriter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEWRITER_H__42AFE18B_6E28_4138_9E8F_56174EC81B8B__INCLUDED_)
#define AFX_IMAGEWRITER_H__42AFE18B_6E28_4138_9E8F_56174EC81B8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImgFileHandler.h"

class CImageWriter : public CImgFileHandler
{
public:
    CImageWriter();
    virtual ~CImageWriter();
    int createFolders(int l_nProtocolNo,
                      int l_nSubProtNo,
                      int l_nSliceGrpNo,
                      int l_nSliceNo,
                      int l_nImageNo, CString &f_csFilePath);
    template <class T>
    int WriteToFile(CString &f_csFileName, T *f_dImgBuf, unsigned long f_nTotalImgPix) {
        int fd = 0;
        T *tembuf = NULL;

        try {
            if(f_nTotalImgPix <= 0) {
                CString cs(_T("Incorrect total number of image pixels recieved"));
                CEPILogHelper::WriteToErrorLog(cs, __LINE__, __FILE__);
                return(E_ERROR);
            }

            if(NULL == f_dImgBuf) {
                CString cs(_T("Image buffer found NULL"));
                CEPILogHelper::WriteToErrorLog(cs, __LINE__, __FILE__);
                return(E_ERROR);
            }

            if(f_csFileName.IsEmpty() == TRUE) {
                CString cs(_T("Filename found empty"));
                CEPILogHelper::WriteToErrorLog(cs, __LINE__, __FILE__);
                return(E_ERROR);
            }

            char charptr[256] = {'\0'};
            sprintf(charptr , "%s", f_csFileName);
            fd = _open(charptr, (O_WRONLY | O_BINARY | O_TRUNC | O_CREAT));

            if(fd < 0) {
                CString cs(_T("Failed to open file for writing"));
                CEPILogHelper::WriteToErrorLog(cs, __LINE__, __FILE__);
                return(E_ERROR);
            }

            unsigned long l_nSizeWritten = 0;
            int  count = (signed)f_nTotalImgPix;
            unsigned long l_nMallocSize = f_nTotalImgPix * sizeof(T);
            tembuf = (T*)malloc(l_nMallocSize);

            if(NULL == tembuf) {
                CString cs(_T("Memory allocation failed"));
                CEPILogHelper::WriteToErrorLog(cs, __LINE__, __FILE__);

                if(fd) {
                    _close(fd);
                }

                return(E_ERROR);
            }

            memset(tembuf , 0x00, f_nTotalImgPix);

            if(NULL != tembuf) {
                for(int i = 0; i < count; i++) {
                    memcpy((tembuf + i),
                           (f_dImgBuf + ( i )), sizeof(T)); 
                }
            }

            l_nSizeWritten = _write(fd, (void*)tembuf, l_nMallocSize);

            if(l_nSizeWritten != l_nMallocSize) {
                CString cs(_T("Filesize Mismatch"));
                CEPILogHelper::WriteToErrorLog(cs, __LINE__, __FILE__);
            }

            _close(fd);

            if(NULL != tembuf){
                free(tembuf);
				tembuf = NULL;
            }            
        } catch(...) {
            if(fd) {
                _close(fd);
            }

            if(tembuf) {
                free(tembuf);
            }

            CString cs(_T("Exception occurred in WriteToFile"));
            CEPILogHelper::WriteToErrorLog(cs, __LINE__, __FILE__);
            return(E_ERROR);
        }

        return(E_NO_ERROR);
    }
};

#endif // !defined(AFX_IMAGEWRITER_H__42AFE18B_6E28_4138_9E8F_56174EC81B8B__INCLUDED_)
