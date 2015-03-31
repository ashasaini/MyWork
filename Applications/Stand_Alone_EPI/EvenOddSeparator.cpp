// EvenOddSeparator.cpp: implementation of the CEvenOddSeparator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "epi.h"
#include "EvenOddSeparator.h"
#include "CommonDefinations.h"
#include "EPILogHelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEvenOddSeparator::CEvenOddSeparator()
{

}

CEvenOddSeparator::~CEvenOddSeparator()
{

}
int CEvenOddSeparator::separateDataEvenOdd(double *f_dDataBuffer, 
                                           double *f_dEvenDataBuffer, 
                                           double *f_dOddDataBuffer, 
                                           int f_nWidth,
                                           int f_nHeight)
{
    
    try
    {
        if(NULL == f_dDataBuffer
            || NULL == f_dEvenDataBuffer
            || NULL == f_dOddDataBuffer)
        {
            return EPI_ERROR_NULL_FUNCPARAMS;
        }
        long RowSize = 0;
        int templ_1 = 0;       
        
        double *templateBufIterator1 = NULL;
        double *templateBufIterator2 = NULL;
        double *ImgDataIterator = NULL;
        
        templ_1 = 0;
        
        RowSize = (f_nWidth * 2);
        
        templateBufIterator1 = f_dEvenDataBuffer;
        templateBufIterator2 = f_dOddDataBuffer;
        ImgDataIterator = f_dDataBuffer ;
        
        for (int i = 0; i <f_nHeight ;i+=2)
        {
            memcpy(&(templateBufIterator1 [templ_1*RowSize]),
                &(ImgDataIterator[i*RowSize]),
                RowSize*sizeof(double));
            memcpy(&(templateBufIterator2[templ_1*RowSize]),
                &(ImgDataIterator[(i+1)*RowSize]),
                RowSize*sizeof(double));
            templ_1++;
        }
        if((templ_1*2) != f_nHeight){
            throw 1;
        }
        
    }
    catch (...)
    {
        CEPILogHelper::WriteToErrorLog(_T("Exception occurred in separateDataEvenOdd."),__LINE__,__FILE__);
        return EPI_ERR_EXCEPTION;	
    }   
    return 0;
}
int CEvenOddSeparator::mergeDataEvenOdd(double *&f_dDataBuffer, 
                                        double *f_dEvenDataBuffer, 
                                        double *f_dOddDataBuffer, 
                                        int f_nWidth,
                                        int f_nHeight)
{
    
    try
    {
        if(NULL == f_dDataBuffer
            || NULL == f_dEvenDataBuffer
            || NULL == f_dOddDataBuffer)
        {
            return EPI_ERROR_NULL_FUNCPARAMS;
        }
        long RowSize = 0;
        int templ_1 = 0;       
        
        double *templateBufIterator1 = NULL;
        double *templateBufIterator2 = NULL;
        double *ImgDataIterator = NULL;
        
        templ_1 = 0;
        
        RowSize = (f_nWidth * 2);
        
        templateBufIterator1 = f_dEvenDataBuffer;
        templateBufIterator2 = f_dOddDataBuffer;
        ImgDataIterator = f_dDataBuffer ;
        
        for (int i = 0; i <f_nHeight ;i+=2)
        {
            memcpy(&(ImgDataIterator[i*RowSize]),
                &(templateBufIterator1 [templ_1*RowSize]),
                RowSize*sizeof(double));
            memcpy(&(ImgDataIterator[(i+1)*RowSize]),
                &(templateBufIterator2[templ_1*RowSize]),
                RowSize*sizeof(double));
            templ_1++;
        }
        if((templ_1*2) != f_nHeight){
            throw 1;
        }
        
    }
    catch (...)
    {
        CEPILogHelper::WriteToErrorLog(_T("Exception occurred in mergeDataEvenOdd."),__LINE__,__FILE__);
        return EPI_ERR_EXCEPTION;	
    }   
    return 0;
}
