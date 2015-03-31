// ExtraPointsGen.cpp: implementation of the CExtraPointsGen class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/

#include "stdafx.h"
#include "tiprep.h"
#include "ExtraPointsGen.h"
#include "TiPrepLogHelper.h"
#include <algorithm>
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

const CString EXCEPTION_EXTRAPOINTSGEN = L"Exception occurred in Extra Points Generation";   
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*************************************************************
* Method:    	CExtraPointsGen
* Description: 	This function is used 
* Returns:   	
*************************************************************/
CExtraPointsGen::CExtraPointsGen(int intervalpts = 5)
{
    m_diffpts = 0.0;
    m_TotalRoiNum = 0;
    m_Origpts = 0;
    m_primeFlag = 0;
    m_inside_pts = 0;
    m_Newpts = 0;
    m_BuffGenX = NULL;
    m_BuffGenY = NULL;
    m_BuffGenYrev = NULL;
    m_FailureFlag = 0;
    m_INTDVD = intervalpts;
}

CExtraPointsGen::CExtraPointsGen()
{
    
}

/*************************************************************
* Method:    	~CExtraPointsGen
* Description: 	This function is used 
* Returns:   	
*************************************************************/
CExtraPointsGen::~CExtraPointsGen()
{
    freeBuffGenX();
    freeBuffGenY();
    freeBuffGenYrev();
}

// //copy constructor
// CExtraPointsGen::CExtraPointsGen(CExtraPointsGen& f_ExtraPntGen)
// {
//     //TIPREP_TRACE(CExtraPointsGen::CExtraPointsGen);     
//     
// }

//assignment operator
CExtraPointsGen& CExtraPointsGen::operator = (CExtraPointsGen& f_ExtraPntGen)
{
//     //TIPREP_TRACE(CErrorDlg::operator =);
//     
    if(this == &f_ExtraPntGen) {
        return(*this);
    }  
	m_BuffGenX = f_ExtraPntGen.m_BuffGenX;
	m_BuffGenY = f_ExtraPntGen.m_BuffGenY;
	m_BuffGenYrev = f_ExtraPntGen.m_BuffGenYrev;
	m_diffpts = f_ExtraPntGen.m_diffpts;
	m_TotalRoiNum = f_ExtraPntGen.m_TotalRoiNum;
	m_Origpts = f_ExtraPntGen.m_Origpts;
	m_primeFlag = f_ExtraPntGen.m_primeFlag;
	m_inside_pts = f_ExtraPntGen.m_inside_pts;
	m_Newpts = f_ExtraPntGen.m_Newpts;
//     }   
//     
    return (*this);
}
///////////////////////////////////////////////////////////////////////////////
// Member Functions
///////////////////


/*************************************************************
* Method:    	AllocBuffGenX
* Description: 	This function is used 
* Returns:   	int
*************************************************************/
int CExtraPointsGen::AllocBuffGenX()
{
    try{
        if(NULL == m_BuffGenX) {
            if(m_Newpts <= 0) {
                m_FailureFlag = 1;
                return E_ERROR;
            }
            
            m_BuffGenX = new double[m_Newpts];
            
            if(NULL == m_BuffGenX) {
                CTiPrepLogHelper::WriteToErrorLog
                    (_T("AllocBuffGenX(): BuffGenX NULL error"), __LINE__, __FILE__);
                freeBuffGenX();
                m_FailureFlag = 1;
                return E_ERROR;
            }
            
            memset(m_BuffGenX, 0x0, (m_Newpts * sizeof(double)));
        }
        
        return E_NO_ERROR;
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_EXTRAPOINTSGEN, __LINE__, __FILE__);
        m_FailureFlag = 1;
        return E_ERROR;
    }
}

/*************************************************************
* Method:    	freeBuffGenX
* Description: 	This function is used 
* Returns:   	void
*************************************************************/
void CExtraPointsGen::freeBuffGenX()
{
    if(NULL != m_BuffGenX) {
        delete[] m_BuffGenX;
        m_BuffGenX = NULL;
    }

    return ;
}

/*************************************************************
* Method:    	AllocBuffGenY
* Description: 	This function is used 
* Returns:   	int
*************************************************************/
int CExtraPointsGen::AllocBuffGenY()
{
    int l_nCurrentRoi = 0;

    try{
        if(NULL == m_BuffGenY) {
            if(m_TotalRoiNum <= 0 || m_Newpts <= 0) {
                m_FailureFlag = 1;
                return E_ERROR;
            }
            
            m_BuffGenY = new double*[m_TotalRoiNum];//c++
            
            if(NULL == m_BuffGenY) {
                return E_ERROR;
            }
            
            for(l_nCurrentRoi = 0; l_nCurrentRoi < m_TotalRoiNum; l_nCurrentRoi++) {
                m_BuffGenY[l_nCurrentRoi] = new double[m_Newpts];//c++
                
                if(NULL == m_BuffGenY[l_nCurrentRoi]) {
                    freeBuffGenY();
                    CTiPrepLogHelper::WriteToErrorLog(_T("AllocBuffGenY(): BuffGenY NULL error"), __LINE__, __FILE__);
                    m_FailureFlag = 1;
                    return E_ERROR;
                }
                
                memset(m_BuffGenY[l_nCurrentRoi], 0x0, (m_Newpts * sizeof(double)));//c
            }
        }
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_EXTRAPOINTSGEN, __LINE__, __FILE__);
        m_FailureFlag = 1;
        return E_ERROR;
    }
    
    return E_NO_ERROR;
}

/*************************************************************
* Method:    	freeBuffGenY
* Description: 	This function is used 
* Returns:   	void
*************************************************************/
void CExtraPointsGen::freeBuffGenY()
{
    int l_nCurrentRoi = 0;
    
    if(NULL != m_BuffGenY) {
        for(l_nCurrentRoi = 0; l_nCurrentRoi < m_TotalRoiNum; l_nCurrentRoi++) {
            if(NULL != m_BuffGenY[l_nCurrentRoi]) {
                delete [] m_BuffGenY[l_nCurrentRoi];
                m_BuffGenY[l_nCurrentRoi] = NULL;
            }
        }
    }

    if(NULL != m_BuffGenY) {
        delete [] m_BuffGenY;
        m_BuffGenY = NULL;
    }

    return;
}

/*************************************************************
* Method:    	AllocBuffGenYrev
* Description: 	This function is used 
* Returns:   	int
*************************************************************/
int CExtraPointsGen::AllocBuffGenYrev()
{
    int l_nCurrentRoi = 0;
    try{
        if(NULL == m_BuffGenYrev) {
            if(m_TotalRoiNum <= 0 || m_Newpts <= 0) {
                return E_ERROR;
            }
            
            m_BuffGenYrev = new double*[m_TotalRoiNum ];
            
            if(NULL == m_BuffGenYrev) {
                m_FailureFlag = 1;
                return E_ERROR;
            }
            
            for(l_nCurrentRoi = 0; l_nCurrentRoi < m_TotalRoiNum; l_nCurrentRoi++) {
                m_BuffGenYrev[l_nCurrentRoi] = new double[m_Newpts];
                
                if(NULL == m_BuffGenYrev[l_nCurrentRoi]) {
                    freeBuffGenYrev();
                    CTiPrepLogHelper::WriteToErrorLog(_T("AllocBuffGenYrev(): BuffGenYrev NULL error"), __LINE__, __FILE__);
                    m_FailureFlag = 1;
                    return E_ERROR;
                }
                
                memset(m_BuffGenYrev[l_nCurrentRoi], 0x0, (m_Newpts * sizeof(double)));//c
            }
        }
        
        return E_NO_ERROR;
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_EXTRAPOINTSGEN, __LINE__, __FILE__);
        m_FailureFlag = 1;
        return E_ERROR;
    }
}

/*************************************************************
* Method:    	freeBuffGenYrev
* Description: 	This function is used 
* Returns:   	void
*************************************************************/
void CExtraPointsGen::freeBuffGenYrev()
{
    int l_nCurrentRoi = 0;
    try{
        if(NULL != m_BuffGenYrev) {
            for(l_nCurrentRoi = 0; l_nCurrentRoi < m_TotalRoiNum; l_nCurrentRoi++) {
                if(NULL != m_BuffGenYrev[l_nCurrentRoi]) {
                    delete [] m_BuffGenYrev[l_nCurrentRoi];
                    m_BuffGenYrev[l_nCurrentRoi] = NULL;
                }
            }
        }
        
        if(NULL != m_BuffGenYrev) {
            delete [] m_BuffGenYrev;
            m_BuffGenYrev = NULL;
        }
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_EXTRAPOINTSGEN, __LINE__, __FILE__);
        m_FailureFlag = 1;
    }
    return;
}

/*************************************************************
* Method:    	ModeSetting
* Description: 	This function is used 
* Parameter: 	int size :
* Returns:   	int
*************************************************************/
int CExtraPointsGen::ModeSetting(int size)
{
    freeBuffGenX();
    freeBuffGenY();
    freeBuffGenYrev();

    if(size <= 0) {
        m_FailureFlag = 1;
        return E_ERROR;
    }

    m_TotalRoiNum = size > 0 ? size  : 1;
    return E_NO_ERROR;
}

/*************************************************************
* Method:    	GenerateBuffgenY
* Description: 	This function is used 
* Parameter: 	double a :
* Parameter: 	double b :
* Parameter: 	double m :
* Parameter: 	double K :
* Parameter: 	double Xr :
* Parameter: 	int roi :
* Parameter: 	int EquationFlag :
* Parameter: 	int MinrevFlag :
* Returns:   	int
*************************************************************/
int CExtraPointsGen::GenerateBuffgenY(
    double a, double b, double m, double K,
    double Xr ,
    int roi,
    int EquationFlag,
    int MinrevFlag
)
{
//    int lim = 0;
    //int status = 0;
    int i = 0;
//    int j = 0;
//    int k = 0;
//    int breakflag = 0;
    int index = 0;
    double**TempBuf = NULL;
    
    if(NULL == m_BuffGenX || NULL == m_BuffGenY || NULL == m_BuffGenYrev || roi < 0) {
        m_FailureFlag = 1;
        return E_ERROR;
    }
    try{
        index = roi ;
        
        if(MinrevFlag == MINREVERSEGRAPH/*1*/) {
            TempBuf = m_BuffGenYrev;
        } else {
            TempBuf = m_BuffGenY;
        }
        
        if(TempBuf == NULL) {
            m_FailureFlag = 1;
            return E_ERROR;
        }
        
        for(i = 0; i < m_Newpts; i++) {
            if(TempBuf != NULL && TempBuf[index] != NULL) { //NULL Check Added -KJ 2013 July
                if(0 == EquationFlag) {
                    TempBuf[index][i] = (a) * (1 - 2 * exp(b * 0.001 * m_BuffGenX[i])); //1 milliseconds = seconds 0.001
                } else if(1 == EquationFlag) {
                    TempBuf[index][i] = (a) * (1 - 2 * exp(b * 0.001 * m_BuffGenX[i]) + exp(b * Xr));
                } else if(2 == EquationFlag) {
                    TempBuf[index][i] = (a) * (1 - 2 * exp(b * 0.001 * m_BuffGenX[i])) + m * (exp(b * Xr));
                } else if(3 == EquationFlag) {
                    TempBuf[index][i] = (a) * (1 - exp(b * 0.001 * m_BuffGenX[i]));
                } else if(4 == EquationFlag) {
                    TempBuf[index][i] = (a) * (1 - K * exp(b * 0.001 * m_BuffGenX[i])); //capital K not small
                } else {
                    CTiPrepLogHelper::WriteToErrorLog(_T("GenerateBuffgenY(): unknown equation"), __LINE__, __FILE__);
                    m_FailureFlag = 1;
                    return E_ERROR;
                }
            }
        }
        
        for(i = 0; i < m_Newpts; i++) {
            if(NULL != TempBuf || NULL != TempBuf[index]) {
                TempBuf[index][i] = TempBuf[index][i] * 10000;
            }
        }
        
        TempBuf = NULL;//dont free this
        return E_NO_ERROR;
    } catch(...) {
        m_FailureFlag = 1;
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_EXTRAPOINTSGEN, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:    	GenerateBuffgenX
* Description: 	This function is used 
* Parameter: 	int * x :
* Returns:   	int
*************************************************************/
int CExtraPointsGen::GenerateBuffgenX(int *x)
{
    int lim = 0;
    int i = 0;
    int j = 0;
    int k = 0;
    
    try{
        if(x == NULL || NULL == m_BuffGenX) {
            m_FailureFlag = 1;
            return E_ERROR;
        }
        
        for(i = 0, k = 0; i < m_Newpts; i++, k++) {
            if(m_Origpts == k) {
                break;
            }
            
            m_BuffGenX[i] = (double)x[k];//filling as it is milliseconds
            
            if(m_primeFlag) {
                lim = m_inside_pts;
            } else {
                lim = m_INTDVD - 1;
            }
            
            for(j = 0; j < lim; j++) {
                if(i == (m_Newpts - 1)) {
                    break;
                }
                
                i++;
                m_BuffGenX[i] = (double)((double)x[k] + m_diffpts * (j + 1));
            }
        }
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_EXTRAPOINTSGEN, __LINE__, __FILE__);
        m_FailureFlag = 1;
        return E_ERROR;
    }
    return E_NO_ERROR;
}

/*************************************************************
* Method:    	DivideInterval
* Description: 	This function is used 
* Parameter: 	double first :
* Parameter: 	double second :
* Parameter: 	int prevpts :
* Parameter: 	int & newpts :
* Returns:   	int
*************************************************************/
int CExtraPointsGen:: DivideInterval(double first, double second, int prevpts, int & newpts)
{
    int diff = 0;
    int fst = 0, sec = 0;
    int mod10 = 0;
    
    try{
        fst = (int)((1000.0 * first) + 0.5);
        sec = (int)((1000.0 * second) + 0.5);
        mod10 = fst % 10;
        
        if(mod10 > 5) {
            fst = fst + (10 - mod10);
        }
        
        mod10 = sec % 10;
        
        if(mod10 > 5) {
            sec = sec + (10 - mod10);
        }
        
        diff = (int)fabs(float(sec - fst));
        
        if(diff > 10) {
            if(diff % m_INTDVD == 0) { //creating 4 pts in between
                
                m_diffpts = (diff / m_INTDVD);
                newpts = prevpts * m_INTDVD - (m_INTDVD - 1);
            } else if(diff % 2 == 0) {

                m_diffpts = diff / 2 ;
                newpts = prevpts * 2 - 1;
            } else {
                m_primeFlag = 1;
                m_inside_pts = diff - 1;
                m_diffpts = 1;
                newpts = (diff - 1) * (prevpts - 1) + prevpts;
            }
        }
        
        m_Origpts = prevpts;
        return E_NO_ERROR;
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_EXTRAPOINTSGEN, __LINE__, __FILE__);
        m_FailureFlag = 1;
        return E_ERROR;
    }
}

/*************************************************************
* Method:    	DivideIntervalAndAllocateNewBuffers
* Description: 	This function is used 
* Parameter: 	int & pts :
* Parameter: 	int * x :
* Returns:   	int
*************************************************************/
int CExtraPointsGen::DivideIntervalAndAllocateNewBuffers(int &pts , int *x)
{
    int status = E_NO_ERROR;

    try {
        if(NULL == x) {
            pts = 0;
            m_FailureFlag = 1;
            return (E_ERROR);
        }

        status = DivideInterval((double)x[0] / 1000, (double)x[1] / 1000, (pts),m_Newpts); //in milliseconds to seconds
        if(status != E_NO_ERROR){
            (pts) = 0;
            return E_ERROR;
        }
        (pts) = m_Newpts;

        if(m_Newpts == 0) {
            (pts) = 0;
            return E_ERROR;
        }

        status = AllocBuffGenX();

        if(status != E_NO_ERROR) {
            (pts) = 0;
            m_FailureFlag = 1;
            return E_ERROR;
        }

        status = AllocBuffGenY();

        if(status != E_NO_ERROR) {
            freeBuffGenX();
            (pts) = 0;
            m_FailureFlag = 1;
            return E_ERROR;
        }

        status = AllocBuffGenYrev();

        if(status != E_NO_ERROR) {
            freeBuffGenY();
            freeBuffGenX();
            (pts) = 0;
            m_FailureFlag = 1;
            return E_ERROR;
        }

        return E_NO_ERROR;

    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_EXTRAPOINTSGEN, __LINE__, __FILE__);
        freeBuffGenYrev();
        freeBuffGenY();
        freeBuffGenX();
        m_FailureFlag = 1;
        return E_ERROR;
    }
}

/*************************************************************
* Method:    	GetCopyOfNewBuffers
* Description: 	This function is used 
* Parameter: 	int * xptr :
* Parameter: 	double * * yptr :
* Parameter: 	double * * revyptr :
* Returns:   	int
*************************************************************/
int CExtraPointsGen::GetCopyOfNewBuffers(int*xptr, double**yptr, double**revyptr)
{
    if(NULL == xptr || NULL == yptr || NULL == revyptr) {
        freeBuffGenY();
        freeBuffGenX();
        freeBuffGenYrev();
        m_FailureFlag = 1;
        return E_ERROR;
    }

    CopyBufferX(xptr);
    CopyBufferY(yptr);
    CopyBufferYrev(revyptr);
    return E_NO_ERROR;
}

/*************************************************************
* Method:    	CopyBufferX
* Description: 	This function is used 
* Parameter: 	int * xptr :
* Returns:   	void
*************************************************************/
void CExtraPointsGen::CopyBufferX(int*xptr)
{
    for(int index = 0; index < m_TotalRoiNum; index++) {
        if(xptr != NULL && m_BuffGenX != NULL) {
            for(int i = 0; i < m_Newpts; i++) {
                xptr[i] = (int)m_BuffGenX[i];
            }
        }
    }
}

/*************************************************************
* Method:    	CopyBufferY
* Description: 	This function is used 
* Parameter: 	double * * yptr :
* Returns:   	void
*************************************************************/
void CExtraPointsGen::CopyBufferY(double**yptr)
{
    try{
        for(int i = 0; i < m_TotalRoiNum; i++) {
            if(yptr[i] != NULL && m_BuffGenY[i] != NULL && m_Newpts > 0) {
                memcpy(yptr[i], m_BuffGenY[i], sizeof(double)*m_Newpts);
            }
        }
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_EXTRAPOINTSGEN, __LINE__, __FILE__);
        m_FailureFlag = 1;
    }
}

/*************************************************************
* Method:    	CopyBufferYrev
* Description: 	This function is used 
* Parameter: 	double * * yptrrev :
* Returns:   	void
*************************************************************/
void CExtraPointsGen::CopyBufferYrev(double**yptrrev)
{
    try{
        for(int i = 0; i < m_TotalRoiNum; i++) {
            if(yptrrev[i] != NULL && m_BuffGenYrev[i] != NULL && m_Newpts > 0) {
                memcpy(yptrrev[i], m_BuffGenYrev[i], sizeof(double)*m_Newpts);
            }
        }
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_EXTRAPOINTSGEN, __LINE__, __FILE__);
    }
}

///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////

