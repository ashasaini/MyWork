// VFCWriter.cpp: implementation of the CVFCWriter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VFCWriter.h"
#include "CommonDefinations.h"
#include "EPILogHelper.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVFCWriter::CVFCWriter()
{

}

CVFCWriter::~CVFCWriter()
{
    CloseDBFile();
}
int CVFCWriter::createNewProtocols(int srcIndx, int &dstP )
{
    UNREFERENCED_PARAMETER(srcIndx);
    UNREFERENCED_PARAMETER(dstP);
    try 
    {
        
        int retVal;
        int to_prot;
        //  Duplicate the protocol for odd one
        to_prot = -1;
        retVal = VuiCopyProtocol(m_vfd, srcIndx, -1, 0, &to_prot); // Image Group zero.
        
        if(to_prot != -1) {
            VuiAddToAcqOrder(m_vfd, to_prot);
        } else {
            return -1;
        }    
        dstP = to_prot;
    }catch(...){
        CString l_csErrMsg = _T("Exception occured while createNewProtocols vfDB");
        return EPI_ERR_EXCEPTION;
    }
    return E_NO_ERROR;
    
}

int CVFCWriter::OpenDBFile()
{
    try{
        VfDBName_t l_name;
        int l_nstatus = 0;
        char* l_cpStudyfilename = NULL;
        CString l_csErrMsg ;
        l_cpStudyfilename = new char[128];
        
        if(NULL == l_cpStudyfilename) {
            CString l_csErrMsg = _T("Memory can not be allocated because of error");
            return(EPI_MEM_ALLOC);
        }    
        sprintf(l_cpStudyfilename, "%s", m_csStudyFileName);
        l_name.filename = l_cpStudyfilename;
        l_nstatus = ::VfOpen(VFFT_FILE, l_name, O_RDWR , &m_vfd);
        
        if(E_NO_ERROR != l_nstatus) {
            l_csErrMsg.Format(_T("vfopen error"));
            if(NULL != l_cpStudyfilename) {
                delete []l_cpStudyfilename;
                l_cpStudyfilename = NULL;            
            }
            if (E_FILE_NOT_FOUND == l_nstatus)
            {
                return EPI_VF_FILE_NOT_FOUND;
            }
            return(EPI_VF_OPEN);
        }
        if(NULL != l_cpStudyfilename) {
            delete []l_cpStudyfilename;
            l_cpStudyfilename = NULL;            
        }
    }catch(...){
        return E_ERROR;
    }
    return E_NO_ERROR;
}

int CVFCWriter::CloseDBFile()
{
    try {
        if(m_vfd){
            ::VfClose(m_vfd);
            m_vfd = NULL;
        }
    } catch(...) {
        CString l_csErrMsg = _T("Exception occured while closing vfDB");
        return EPI_ERR_EXCEPTION;
    }    
    return E_NO_ERROR;
}

int CVFCWriter::PutData(void *DataBuffer, 
                        VfPathElem_t pathP[], 
                        int img_size, 
                        int data_type, int channel)
{
    try{
        VfFieldSpec_t   req;
        int             num_put = 0;
        int             status = E_NO_ERROR;
        time_t          timestamp;
        if (NULL == DataBuffer)
        {
            CEPILogHelper::WriteToErrorLog("\n Data buffer is NULL",__LINE__,__FILE__);
            return EPI_ERROR_NULL_FUNCPARAMS;
        }
        if(channel == 0) {
            VFF_SET_ARGS(req, SVN_GEN_DATA, data_type, img_size, DataBuffer,
                VFO_REPLACE);
        } else {
            VFF_SET_ARGS(req, (SVN_GEN_DATA + (19 + channel)), data_type, img_size, DataBuffer,
                VFO_REPLACE);
        }
        
        status = VfPutFields(m_vfd, pathP, SVD_ST_IMAGE,
            &req, 1, &num_put, &timestamp);
        
        if(num_put != 1 || status != E_NO_ERROR) {
            CEPILogHelper::WriteToErrorLog("\n error in put data",__LINE__,__FILE__);
            return EPI_VFC_FAILED;
        }
    }catch(...){
        CString l_csErrMsg = _T("Exception occured while closing vfDB");
        CEPILogHelper::WriteToErrorLog(l_csErrMsg,__LINE__,__FILE__);
        return EPI_ERR_EXCEPTION;
        
    }    
    return E_NO_ERROR;
}
