// TiPrepChildThread.cpp: implementation of the CTiPrepChildThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tiprep.h"
#include "TiPrepChildThread.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTiPrepChildThread::CTiPrepChildThread() :
m_dval_Ti(0),
m_nroiNo(0),
m_dval_T1(0),
m_dinputData(0),
m_doutputData(0),
m_dfitErr(0),
m_nselectImg(0),
m_ntime_data(0),
m_dRepetationTim(0),
m_nSelectedEqn(0),
m_CExtraPoint(0),
m_nManualInitialGuessChk(0),
m_numpts(0),
m_linefitobj(NULL),
m_drevval_Ti(NULL),
m_drevval_T1(NULL),
m_drevinputData(NULL),
m_drevoutputData(NULL),
m_drevfitErr(NULL)
{
	
}

CTiPrepChildThread::~CTiPrepChildThread()
{
	
}
int CTiPrepChildThread::StartThread(double *f_dval_Ti,
									double*f_dval_T1,
									int f_nroiNo,
									double **f_dRoi_inputData,
									double **f_dRoi_outputData,
									double *f_dfitErr,
									int f_nselectImg,
									int *f_ntime_data,
									double f_dRepetationTime,
									int f_nSelectedEqn,
									CExtraPointsGen*f_CExtraPoint,
									int f_nManualInitialGuessChk, int pts, HANDLE &hndl,CLineFitStrategy * linefit,
									double *f_drev_val_Ti,
									double*f_drevval_T1,
									double **f_drevRoi_inputData,
									double **f_drevRoi_outputData,
									double *f_drevfitErr,HWND f_wndhndl)
{
	try {
		this->m_dval_Ti = f_dval_Ti;
		this->m_dval_T1 = f_dval_T1;
		this->m_nroiNo = f_nroiNo;
		this->m_dinputData= f_dRoi_inputData; //img x rois
		this->m_doutputData=f_dRoi_outputData; // img x rois
		this->m_dfitErr=f_dfitErr; // rois
		this->m_nselectImg=f_nselectImg; //numimgs
		this->m_ntime_data=f_ntime_data; //same for all rois
		this->m_dRepetationTim = f_dRepetationTime; //same
		this->m_nSelectedEqn=f_nSelectedEqn; //same
		this->m_CExtraPoint = f_CExtraPoint; //same
		//	this->f_nMinrevFlag=f_nMinrevFlag; //same
		this->m_nManualInitialGuessChk = f_nManualInitialGuessChk; //same
		this->m_numpts = pts;
		this->m_linefitobj = linefit;
		
		this->m_drevval_Ti = f_drev_val_Ti;
		this->m_drevval_T1 = f_drevval_T1;
		
		this->m_drevinputData= f_drevRoi_inputData; //img x rois
		this->m_drevoutputData=f_drevRoi_outputData; // img x rois
		this->m_drevfitErr=f_drevfitErr; // rois
		this->wndhndl = f_wndhndl;
		DWORD dwthreadid;
		hndl = CreateThread(NULL, 0, TiPrepThreadProc, this, 0, &dwthreadid);
				} catch (...) {
		CTiPrepLogHelper::WriteToErrorLog(_T("Exception occurred in StartThread"), __LINE__, __FILE__);
		return E_ERROR;
				}
				return 0;
}
DWORD WINAPI  CTiPrepChildThread::TiPrepThreadProc(LPVOID lpParameter)
{
	if (NULL == lpParameter) return false;
	
	try {
	//	BOOL isthreadcompleted = false;
		CTiPrepChildThread* self = NULL;
		self = (CTiPrepChildThread*)lpParameter;
		if (self == NULL) return 0;
		int status = 0;
        if(NULL == self->m_linefitobj){
            return 0;
        }
		//   self->m_CriticalSection.Lock(); //AN/Revw/Critical scetion is notrequired here, this object is different for each thread
		
		self->m_linefitobj->AllocTimeStamp(self->m_ntime_data,self->m_nselectImg);
		self->m_linefitobj->AllocTmpBuffers(self->m_nselectImg,self->m_nSelectedEqn);
		for (int i = self->m_nroiNo; i < (self->m_nroiNo + self->m_numpts); i++)
		{
			status = self->m_linefitobj->CalculateTi_NonLinear(
				self->m_dval_Ti, 
				self->m_dval_T1,
				i,
				self->m_dinputData[i],
				self->m_doutputData[i],
				&self->m_dfitErr[i],
				self->m_nselectImg,
				self->m_ntime_data,
				self->m_dRepetationTim,
				self->m_nSelectedEqn,
				self->m_CExtraPoint,
				0, self->m_nManualInitialGuessChk);
			
			status = self->m_linefitobj->CalculateTi_NonLinear(self->m_drevval_Ti, 
				self->m_drevval_T1,
				i,
				self->m_drevinputData[i],
				self->m_drevoutputData[i],
				&self->m_drevfitErr[i],
				self->m_nselectImg,
				self->m_ntime_data,
				self->m_dRepetationTim,
				self->m_nSelectedEqn,
				self->m_CExtraPoint,
				1, self->m_nManualInitialGuessChk);
			// if (i == (self->m_nroiNo + self->m_numpts - 1)){
			//   isthreadcompleted = TRUE;
            //}
		//	if ((i == (self->m_nroiNo + self->m_numpts - 1)) || (i%100 == 0))
			::PostMessage(self->wndhndl,WM_UPDATE_PROGRESS,FALSE,0);
		}
		::PostMessage(self->wndhndl,WM_UPDATE_PROGRESS,TRUE,0);
		self->m_linefitobj->DeAllocTimeStampData();
		self->m_linefitobj->DeAllocTmpBuffers(self->m_nSelectedEqn);
		//  self->m_CriticalSection.Unlock();
	} catch(...)
	{
		CTiPrepLogHelper::WriteToErrorLog(_T("Exception occurred in TiPrepThreadProc"), __LINE__, __FILE__);
        return E_ERROR;
	}
	return 0;
}