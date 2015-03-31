// FileReader.cpp: implementation of the CFileReader class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.

    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#include "stdafx.h"
#include "FileReader.h"
#include <fcntl.h>
#include <IO.h>
#include "EPILogHelper.h"
#include "CommonDefinations.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileReader::CFileReader()
{
}

CFileReader::~CFileReader()
{
    //
}


/*************************************************************
* Method:       ReadImage
* Description:  This function is used
* Parameter:    CString & f_csImgFilePath :
* Parameter:    short * f_dImgBuf :
* Parameter:    unsigned long l_ulNumOfElements :
* Returns:      bool
*************************************************************/
int CFileReader::ReadImage(CString &f_csImgFilePath, unsigned long l_ulNumOfElements, double* &f_dImageBuff)
{
    int fd = 0;
    long *tmp_int32buff = NULL;
    short *tmp_int16buff = NULL;
    //flt32_t *tmp_flt32buff = NULL;
    unsigned long i = 0;

    try {
        char path[256] = {'\0'};
        sprintf(path, "%s", f_csImgFilePath);
        fd = _open(path, (O_RDONLY | O_BINARY));
	
        if(fd > 0) {
            f_dImageBuff = new double[l_ulNumOfElements];

            if(f_dImageBuff) { //-----------------------------------
                //double type
                int size_flt64 = _read(fd, f_dImageBuff, l_ulNumOfElements * sizeof(double));
                int l_nSizebuff = (l_ulNumOfElements * sizeof(double));

                if(size_flt64 != l_nSizebuff) {
                    if(fd) {
                        _close(fd);
                        fd = 0;
                    }

                    fd = _open(path, (O_RDONLY | O_BINARY));
                    tmp_int32buff = new long[l_ulNumOfElements];

                    if(tmp_int32buff) { //-------------------------------//int32 type
                        int size_int32 = _read(fd, tmp_int32buff, l_ulNumOfElements * sizeof(long));
                        int l_nSizebuff = (l_ulNumOfElements * sizeof(long));

                        if(size_int32 != l_nSizebuff) {
                            if(fd) {
                                _close(fd);
                                fd = 0;
                            }

                            fd = _open(path, (O_RDONLY | O_BINARY));
                            tmp_int16buff = new short[l_ulNumOfElements];

                            if(tmp_int16buff) { //-----------------------------//int16 type
                                int size_int16 = _read(fd, tmp_int16buff, l_ulNumOfElements * sizeof(short));
                                int l_nSizebuff = (l_ulNumOfElements * sizeof(short));

                                if(size_int16 != l_nSizebuff) {
                                    if(tmp_int32buff) {
                                        delete [] tmp_int32buff;
                                        tmp_int32buff = NULL;
                                    }

                                    if(tmp_int16buff) {
                                        delete [] tmp_int16buff;
                                        tmp_int16buff = NULL;
                                    }

                                    if(fd) {
                                        _close(fd);
                                        fd = 0;
                                    }

                                    return 1;
                                } else { //then data is int16
                                    memset(f_dImageBuff, 0x0, l_ulNumOfElements * sizeof(double));

                                    for(i = 0; i < l_ulNumOfElements; i++) {
                                        f_dImageBuff[i] = tmp_int16buff[i];
                                    }

                                    if(tmp_int16buff) {
                                        delete [] tmp_int16buff;
                                        tmp_int16buff = NULL;
                                    }

                                    if(tmp_int32buff) {
                                        delete [] tmp_int32buff;
                                        tmp_int32buff = NULL;
                                    }
                                }
                            } else {
                                if(tmp_int32buff) {
                                    delete [] tmp_int32buff;
                                    tmp_int32buff = NULL;
                                }

                                if(fd) {
                                    _close(fd);
                                    fd = 0;
                                }

                                return 1;
                            }
                        } else { //then data is int32
                            memset(f_dImageBuff, 0x0, l_ulNumOfElements * sizeof(double));

                            for(i = 0; i < l_ulNumOfElements; i++) {
                                f_dImageBuff[i] = tmp_int32buff[i];
                            }

                            if(tmp_int32buff) {
                                delete [] tmp_int32buff;
                                tmp_int32buff = NULL;
                            }
                        }
                    } else {
                        if(fd) {
                            _close(fd);
                            fd = 0;
                        }

                        return 1;
                    }
                }
            } else {
                if(fd) {
                    _close(fd);
                    fd = 0;
                }

                return 1;
            }

            if(fd) {
                _close(fd);
                fd = 0;
            }
        }else {
			return 1;
		}

        return(0);
    } catch(...) {
        CEPILogHelper::WriteExceptionToLog(__LINE__, __FILE__);

        if(fd) {
            _close(fd);
            fd = 0;
        }

        if(tmp_int32buff) {
            delete [] tmp_int32buff;
            tmp_int32buff = NULL;
        }

        if(tmp_int16buff) {
            delete [] tmp_int16buff;
            tmp_int16buff = NULL;
        }

        return EPI_ERR_EXCEPTION;
    }
}



///////////////////////////////////////////////////////////////////////////////
// End of file
//////////////