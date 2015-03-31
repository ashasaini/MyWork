// kspaceSeparator.cpp: implementation of the CkspaceSeparator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "epi.h"
#include "kspaceSeparator.h"
#include "CommonDefinations.h"
#include "EPILogHelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CkspaceSeparator::CkspaceSeparator()
{
}

CkspaceSeparator::~CkspaceSeparator()
{
}

int CkspaceSeparator::separateDataTemplateAndKspace( void *f_cEntireData,
         void *f_cRawDataBuffer,
         void *f_cTemplateDataBuffer1,
         void *f_cTemplateDataBuffer2,
        int &f_nWidth, int &f_nMultiplier,
        int &f_nHeight, int &flag,
        int &f_nNumrepetition,int f_nDataTypeSize)
{
    //UNREFERENCED_PARAMETER(f_nImage_size);
    UNREFERENCED_PARAMETER(flag);
    int newWidth = 0;
    int raw = 0;
     char *templateBufIterator1 = NULL;
     char *templateBufIterator2 = NULL;
     char *rawBufIterator = NULL;
     char *ImgDataIterator = NULL;
    int numRepeats = 0;
    int y = 0, templ_1 = 0, templ_2 = 0;

    try {
        if(NULL == f_cEntireData
                || NULL == f_cRawDataBuffer
                || NULL == f_cTemplateDataBuffer1
                || NULL == f_cTemplateDataBuffer2) {
            return EPI_ERROR_NULL_FUNCPARAMS;
        }

        raw = 0;
        templ_1 = 0;
        templ_2 = 0;
        newWidth = f_nWidth * f_nMultiplier;
        int RowSize = newWidth*f_nDataTypeSize;
        /*
        If height is not multiple of 3 crash occurs in code.
        To avoid crash and to avoid access of not allocated memory following adjustment is done.
        */
        int correctHt = f_nHeight / 3;
        correctHt = correctHt * 3;

        for(numRepeats = 0; numRepeats < f_nNumrepetition; numRepeats++) {
            templateBufIterator1 = (char*)f_cTemplateDataBuffer1 ;
            templateBufIterator2 = (char*)f_cTemplateDataBuffer2 ;
            rawBufIterator = (char*)f_cRawDataBuffer ;
            ImgDataIterator = (char*)f_cEntireData ;
            y = 0;
//-------------------------------------------------------------------------------------Row by Row Copy Of data
            
            while(y < correctHt) {
                memcpy((templateBufIterator1 + (templ_1 * RowSize )),//-----0+n
                       (ImgDataIterator + (y * RowSize)),
                       RowSize);
                templ_1++;
                y++;
                memcpy((templateBufIterator2 + (templ_2 * RowSize)),//-----1+n
                       (ImgDataIterator + (y * RowSize)),
                       RowSize);
                templ_2++;
                y++;
                memcpy((rawBufIterator + (raw * RowSize)),//-----2+n
                       (ImgDataIterator + (y * RowSize)),
                       RowSize);
                raw++;
                y++;
            }
            if((raw + templ_1 + templ_2) > y){
                CString cs(_T("Seperation Problem!"));
//                 AfxMessageBox(cs);
            }
        }
    } catch(...) {
        CEPILogHelper::WriteToErrorLog(_T("Exception occurred in separateDataTemplateAndKspace."), __LINE__, __FILE__);
        return 1;
    }

    return 0;
}

int CkspaceSeparator::mergeDataTemplateAndKspace(double *f_cFinalBuffer, 
                                                 double *f_cRawDataBuffer, 
                                                 double *f_cTemplateDataBuffer1, 
                                                 double *f_cTemplateDataBuffer2, 
                                                 int f_nWidth,
                                                 int f_nHeight)
{

    try
    {
        if(NULL == f_cFinalBuffer
            || NULL == f_cRawDataBuffer
            || NULL == f_cTemplateDataBuffer1
            || NULL == f_cTemplateDataBuffer2
            || (f_nHeight % 3))
        {
            return EPI_ERROR_NULL_FUNCPARAMS;
        }
        //int newWidth = 0;
        int raw = 0;
        //int numRepeats = 0;
        long RowSize = 0;
        int templ_1 = 0, templ_2 = 0;       
        
        double *templateBufIterator1 = NULL;
        double *templateBufIterator2 = NULL;
        double *rawBufIterator = NULL;
        double *ImgDataIterator = NULL;

        raw = 0;
        templ_1 = 0;
        templ_2 = 0;

        RowSize = (f_nWidth * 2);

        templateBufIterator1 = f_cTemplateDataBuffer1;
        templateBufIterator2 = f_cTemplateDataBuffer2;
        rawBufIterator = f_cRawDataBuffer ;
        ImgDataIterator = f_cFinalBuffer ;
        
        for (int i = 0; i <f_nHeight ;i+=3)
        {
            memcpy(&(ImgDataIterator[i*RowSize]),
                &(templateBufIterator1 [templ_1*RowSize]),
                RowSize*sizeof(double));
            memcpy(&(ImgDataIterator[(i+1)*RowSize]),
                &(templateBufIterator2[templ_1*RowSize]),
                RowSize*sizeof(double));
            memcpy(&(ImgDataIterator[(i+2)*RowSize]),
                &(rawBufIterator[templ_1*RowSize]),
                RowSize*sizeof(double));
            templ_1++;
        }
        if((templ_1*3) != f_nHeight){
            throw 1;
        }
        
    }
    catch (...)
    {
        CEPILogHelper::WriteToErrorLog(_T("Exception occurred in mergeDataTemplateAndKspace."),__LINE__,__FILE__);
        return EPI_ERR_EXCEPTION;	
    }   
    return 0;
}
