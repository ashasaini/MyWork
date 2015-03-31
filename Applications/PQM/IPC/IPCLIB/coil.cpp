//Redmine-783
//Redmine-780
//Redmine-782
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: coil.cpp
 *  Overview: Implementation of CCoil class.
 *  Last Modified: 2007/8/10 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2007/6/15 12:00:00     PQM5 Baselined code
 *   2.0          Patni            2007/8/10 12:00:00     Implemented PQM6 requirements & Formatted
 *
 *****************************************************************************/
#include "StdAfx.h"
#include "coil.h"
#include <tami/libAcq/AcqCoilSignal.h>
#include <PQM/PQMLogMgr.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
IMPLEMENT_SERIAL(CCoil, CObject, 1)

//***************************Method Header*************************************

//Method Name    : CCoil()

//Author         : PATNI/ASG

//Purpose        : Construction

//*****************************************************************************
CCoil::CCoil():
    m_coil_key(0),
    m_coil_v(NULL)
{

}

//***************************Method Header*************************************
//Method Name    : ~CCoil()
//Author         : PATNI/ASG
//Purpose        : Destruction
//*****************************************************************************
CCoil::~CCoil(
)
{
}


CCoil::CCoil(
    const CCoil& obj
): m_coil_key(obj.m_coil_key), m_coil_v(obj.m_coil_v)             // V9.25 Coil Support@AMT
    //- Patni-PJS/2009Apr25/Modify/CPP Test tool execution
{
}

//***************************Method Header*************************************
//Method Name    : operator = ()
//Author         : PATNI/HAR
//Purpose        : Overloading the = operator
//*****************************************************************************
CCoil& CCoil::operator = (
    const CCoil& f_opr
)
{
    if (this == &f_opr) {
        return *this;
    }

    //    m_coilid        = f_opr.m_coilid;
    //    m_coiltype      = f_opr.m_coiltype;
    //    m_insidelimit   = f_opr.m_insidelimit;
    //    m_outsidelimit  = f_opr.m_outsidelimit;
    //    m_pacpattern    = f_opr.m_pacpattern;
    //
    //    m_recievewbenable = f_opr.m_recievewbenable;
    //
    //    m_rfl_limit     = f_opr.m_rfl_limit;
    //    m_speederenable = f_opr.m_speederenable;
    //    m_tunelimit     = f_opr.m_tunelimit;
    //    m_wbcoil        = f_opr.m_wbcoil;
    //    m_enable        = f_opr.m_enable;
    //    m_label         = f_opr.m_label;
    //
    //    m_coil_description = f_opr.m_coil_description;
    //
    //    m_channel       = f_opr.m_channel;
    //    m_port          = f_opr.m_port;
    //    m_coil_key      = f_opr.m_coil_key;
    m_coil_v        = f_opr.m_coil_v;    // V9.25 Coil support@AMT
    m_coil_key	    = f_opr.m_coil_key;
    return *this;
}

//***************************Method Header*************************************
//Method Name    : IsSectionDB()
//Author         : PATNI/AMT
//Purpose        : Returns true if section database is present for this coil
//                  else false
//*****************************************************************************
int CCoil::IsSectionDB() const
{
    LPCTSTR FUNC_NAME = _T("CCoil::IsSectionDB");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_coil_v) {
        return m_coil_v->sectionDB ;

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_coil_v pointer is NULL"));
        return 0;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//***************************Method Header*************************************
//Method Name    : GetOutsideLimit()
//Author         : PATNI/ASG
//Purpose        : Getting the outside limit
//*****************************************************************************
long CCoil::GetOutsideLimit(
) const
{
    LPCTSTR FUNC_NAME = _T("CCoil::GetOutsideLimit");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_coil_v) {
        CString str_msg(_T(""));
        str_msg.Format(_T("OutSideLimit --> %d"), m_coil_v->outSideLimit);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

        // return m_outsidelimit;
        return m_coil_v->outSideLimit;      // V9.25 coil support@AMT

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_coil_v pointer is NULL"));
        return 0l;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//***************************Method Header*************************************
//Method Name    : GetInsideLimit()
//Author         : PATNI/ASG
//Purpose        : Getting the inside limit
//*****************************************************************************
long CCoil::GetInsideLimit(
) const
{
    LPCTSTR FUNC_NAME = _T("CCoil::GetInsideLimit");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_coil_v) {
        CString str_msg(_T(""));
        str_msg.Format(_T("InsideLimit --> %d"), m_coil_v->inSideLimit);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

        // return m_insidelimit;
        return m_coil_v->inSideLimit;              // V9.25 coil support@AMT

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_coil_v pointer is NULL"));
        return 0l;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//***************************Method Header*************************************
//Method Name    : GetCoilKey()
//Author         : PATNI/HAR
//Purpose        : Get the Coil Key
//*****************************************************************************
unsigned long CCoil::GetCoilKey(
) const
{
    LPCTSTR FUNC_NAME = _T("CCoil::GetCoilKey");
    CString str_msg(_T(""));
    str_msg.Format(_T("CoilKey --> %d"), m_coil_key);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    return m_coil_key;
}

//***************************Method Header*************************************
//Method Name    : GetRFLimit()
//Author         : PATNI/ASG
//Purpose        : Getting the RF limit
//*****************************************************************************
long CCoil::GetRFLLimit(
) const
{
    LPCTSTR FUNC_NAME = _T("CCoil::GetRFLLimit");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_coil_v) {
        CString str_msg(_T(""));
        str_msg.Format(_T("RFLLimit --> %d"), m_coil_v->rflLimit);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

        // return m_rfl_limit;
        return m_coil_v->rflLimit;      // V9.25 coil support@AMT

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_coil_v pointer is NULL"));
        return 0l;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//***************************Method Header*************************************
//Method Name    : GetTuneLimit()
//Author         : PATNI/ASG
//Purpose        : Getting the tune limit
//*****************************************************************************
int CCoil::GetTuneLimit(
) const
{
    LPCTSTR FUNC_NAME = _T("CCoil::GetTuneLimit");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_coil_v) {
        CString str_msg(_T(""));
        str_msg.Format(_T("TuneLimit --> %d"), m_coil_v->tuneLimit);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

        // return m_tunelimit;
        return m_coil_v->tuneLimit;     // V9.25 coil support@AMT

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_coil_v pointer is NULL"));
        return 0;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//***************************Method Header*************************************
//Method Name    : GetEnable()
//Author         : PATNI/ASG
//Purpose        : Return the m_enable variable
//*****************************************************************************
BOOL CCoil::GetEnable(
) const
{
    LPCTSTR FUNC_NAME = _T("CCoil::GetEnable");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_coil_v) {
        CString str_msg = _T("");
        str_msg.Format(_T("%d"), m_coil_v->enable);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

        // return m_enable;
        return m_coil_v->enable;    // V9.25 coil support@AMT

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_coil_v pointer is NULL"));
        return FALSE;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//***************************Method Header*************************************
//Method Name    : GetLabel()
//Author         : PATNI/HAR
//Purpose        : Get the value of Label
//*****************************************************************************
CString CCoil::GetLabel(
) const
{
    LPCTSTR FUNC_NAME = _T("CCoil::GetLabel");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_coil_v) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Label --> ") + CString(m_coil_v->label));

        // return m_label;
        return m_coil_v->label;     // V9.25 coil support@AMT

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_coil_v pointer is NULL"));
        return _T("");
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//***************************Method Header*************************************
//Method Name    : GetCoilId()
//Author         : PATNI/ASG
//Purpose        : Getting the value of Coil ID
//*****************************************************************************
int CCoil::GetCoilId(
) const
{
    LPCTSTR FUNC_NAME = _T("CCoil::GetCoilId");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_coil_v) {
        CString str_msg(_T(""));
        str_msg.Format(_T("Coil Id --> %d"), m_coil_v->coil_id);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

        // return m_coilid;
        return m_coil_v->coil_id;   // V9.25 Coil Support@AMT

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_coil_v pointer is NULL"));
        return 0;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//***************************Method Header*************************************
//Method Name    : GetRecieveWBEnable()
//Author         : PATNI/ASG
//Purpose        : Getting the received coil for whole body
//*****************************************************************************
BOOL CCoil::GetRecieveWBEnable(
) const
{
    LPCTSTR FUNC_NAME = _T("CCoil::GetRecieveWBEnable");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_coil_v) {
        CString str_msg = _T("");
        str_msg.Format(_T("RecieveWBEnable --> %d"), m_coil_v->receiveWB_enable);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

        // return m_recievewbenable;
        return m_coil_v->receiveWB_enable;     // V9.25 coil Support@AMT

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_coil_v pointer is NULL"));
        return FALSE;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//***************************Method Header*************************************
//Method Name    : GetPacPattern()
//Author         : PATNI/ASG
//Purpose        : Getting the value of Pac pattern
//*****************************************************************************
long CCoil::GetPacPattern(
) const
{
    LPCTSTR FUNC_NAME = _T("CCoil::GetPacPattern");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_coil_v) {
        CString str_msg(_T(""));
        str_msg.Format(_T("Pac Pattern --> %d"), m_coil_v->pac_pattern);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

        // return m_pacpattern;
        return m_coil_v->pac_pattern;  // V9.25 Coil Support@AMT

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_coil_v pointer is NULL"));
        return 0l;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//***************************Method Header*************************************
//Method Name    : IsWBCoil()
//Author         : PATNI/ASG
//Purpose        : Getting the coil type for the whole body
//*****************************************************************************
BOOL CCoil::IsWBCoil(
) const
{
    LPCTSTR FUNC_NAME = _T("CCoil::IsWBCoil");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_coil_v) {
        CString str_msg(_T(""));
        str_msg.Format(_T("WB Coil --> %d"), m_coil_v->wholeBody);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

        // return m_wbcoil;
        return m_coil_v->wholeBody ;    // V9.25 coil support@AMT

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_coil_v pointer is NULL"));
        return FALSE;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//***************************Method Header*************************************
//Method Name    : GetMaxChannel()
//Author         : PATNI/AMT
//Purpose        : Returns true if section database is present for this coil
//                  else false
//*****************************************************************************
int CCoil::GetMaxChannel() const
{
    LPCTSTR FUNC_NAME = _T("CCoil::GetMaxChannel");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_coil_v) {
        return m_coil_v->maxChannel;

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_coil_v pointer is NULL"));
        return 0;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}
//***************************Method Header*************************************
//Method Name    : GetPort()
//Author         : PATNI/HAR
//Purpose        : Get the Port
//*****************************************************************************
int CCoil::GetPort(
) const
{
    LPCTSTR FUNC_NAME = _T("CCoil::GetPort");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_coil_v) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Port --> ") + m_coil_v->port);

        return m_coil_v->port ;     // V9.25 coil support@AMT

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_coil_v pointer is NULL"));
        return 0;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}
//***************************Method Header*************************************
//Method Name    : GetCoil()
//Author         : PATNI/AMT
//Purpose        : Returns coil structure.
//*****************************************************************************
PqmCoil_t* CCoil::GetCoil() const
{
    return m_coil_v ;
}

//***************************Method Header*************************************
//Method Name    : SetCoil()
//Author         : PATNI/AMT
//Purpose        : Set coil structure.
//*****************************************************************************
bool CCoil::SetCoil(PqmCoil_t* f_coil)
{
    if (f_coil == NULL) {
        return false;
    }

    m_coil_v = f_coil;

    return true;
}

//***************************Method Header*************************************
//Method Name    : CCoilCopy()
//Author         : PATNI/HAR
//Purpose        : Making the single copy
//*****************************************************************************
void CCoil::CCoilCopy(
    const CCoil& f_opr
)
{
    //    m_coilid        = f_opr.m_coilid;
    //    m_coiltype      = f_opr.m_coiltype;
    //    m_insidelimit   = f_opr.m_insidelimit;
    //    m_outsidelimit  = f_opr.m_outsidelimit;
    //    m_pacpattern    = f_opr.m_pacpattern;
    //
    //    m_recievewbenable = f_opr.m_recievewbenable;
    //
    //    m_rfl_limit     = f_opr.m_rfl_limit;
    //    m_speederenable = f_opr.m_speederenable;
    //    m_tunelimit     = f_opr.m_tunelimit;
    //    m_wbcoil        = f_opr.m_wbcoil;
    //    m_enable        = f_opr.m_enable;
    //    m_label         = f_opr.m_label;
    //
    //    m_coil_description = f_opr.m_coil_description;
    //
    //    m_channel       = f_opr.m_channel;
    //    m_port          = f_opr.m_port;
    //    m_coil_key      = f_opr.m_coil_key;
    m_coil_v        = f_opr.m_coil_v;    // V9.25 Coil support@AMT
}

//***************************Method Header*************************************
//Method Name    : SetCoilId()
//Author         : PATNI/ASG
//Purpose        : Setting the value of Coil ID
//*****************************************************************************
void CCoil::SetCoilId(
    const unsigned long id
)
{
    LPCTSTR FUNC_NAME = _T("CCoil::SetCoilId");

    //    CString str_msg(_T(""));
    //    str_msg.Format(_T("%d"),id);
    //
    //    CPQMLogMgr::GetInstance()->WriteTraceLog(DOMAIN_NAME,
    //        USER_FUNC_MARKER,
    //        _T("CCoil::SetCoilId"),
    //        _T("Coil Id --> ")+str_msg);

    //    m_coilid = id;
    if (m_coil_v) {
        m_coil_v->coil_id = id;     // V9.25 Coil Support@AMT

    } else {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_coil_v pointer is NULL"));
    }
}

//***************************Method Header*************************************
//Method Name    : SetPacPattern()
//Author         : PATNI/ASG
//Purpose        : Setting the value of Pac pattern
//*****************************************************************************
void CCoil::SetPacPattern(
    const long pacpattern
)
{
    LPCTSTR FUNC_NAME = _T("CCoil::SetPacPattern");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_coil_v) {
        CString str_msg(_T(""));
        str_msg.Format(_T("Pac Pattern --> %d"), pacpattern);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

        // m_pacpattern = pacpattern;
        m_coil_v->pac_pattern = pacpattern; // V9.25 Coil Support@AMT

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_coil_v pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}


//***************************Method Header*************************************
//Method Name    : SetIsWBCoil()
//Author         : PATNI/ASG
//Purpose        : Setting the coil type for whole body
//*****************************************************************************
void CCoil::SetIsWBCoil(
    const BOOL wbcoil
)
{
    LPCTSTR FUNC_NAME = _T("CCoil::SetIsWBCoil");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_coil_v) {
        CString str_msg(_T(""));
        str_msg.Format(_T("WB Coil --> %d"), wbcoil);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

        // m_wbcoil = wbcoil;
        m_coil_v->wholeBody = wbcoil;   // V9.25 coil support@AMT

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_coil_v pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//***************************Method Header*************************************
//Method Name    : SetEnable()
//Author         : PATNI/ASG
//Purpose        : Setting if the coil is enabled
//*****************************************************************************
void CCoil::SetEnable(
    const BOOL enable
)
{
    LPCTSTR FUNC_NAME = _T("CCoil::SetEnable");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_coil_v) {
        CString str_msg(_T(""));
        str_msg.Format(_T("Enable --> %d"), enable);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

        // m_enable = enable;
        m_coil_v->enable = enable;      // V9.25 coil support@AMT

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_coil_v pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}


//***************************Method Header*************************************
//Method Name    : SetLabel()
//Author         : PATNI/HAR
//Purpose        : Set the value of Label
//*****************************************************************************
void CCoil::SetLabel(
    CString& label
)
{
    LPCTSTR FUNC_NAME = _T("CCoil::SetLabel");

    // + V9.25 coil support@AMT
    // m_label = label;
    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_coil_v) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Label --> ") + label);
        m_coil_v->label = (char*) label.GetBuffer(label.GetLength());

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_coil_v pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    // - V9.25 coil support@AMT
}

//***************************Method Header*************************************
//Method Name    : SetRecieveWBEnable()
//Author         : PATNI/ASG
//Purpose        : Setting the received coil for whole body
//*****************************************************************************
void CCoil::SetRecieveWBEnable(
    const BOOL recievewbenable
)
{
    LPCTSTR FUNC_NAME = _T("CCoil::SetRecieveWBEnable");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_coil_v) {
        CString str_msg(_T(""));
        str_msg.Format(_T("RecieveWBEnable --> %d"), recievewbenable);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

        //m_recievewbenable = recievewbenable;
        m_coil_v->receiveWB_enable = recievewbenable ; // V9.25 coil support@AMT

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_coil_v pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//***************************Method Header*************************************
//Method Name    : SetOutsideLimit()
//Author         : PATNI/ASG
//Purpose        : Setting the outside limit
//*****************************************************************************
void CCoil::SetOutsideLimit(
    const long outsidelimit
)
{
    LPCTSTR FUNC_NAME = _T("CCoil::SetOutsideLimit");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_coil_v) {
        CString str_msg = _T("");
        str_msg.Format(_T("OutSide Limit --> %d"), outsidelimit);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

        // m_outsidelimit = outsidelimit;
        m_coil_v->outSideLimit = outsidelimit;   // V9.25 coil support@AMT

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_coil_v pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//***************************Method Header*************************************
//Method Name    : SetInsideLimit()
//Author         : PATNI/ASG
//Purpose        : Setting the inside limit
//*****************************************************************************
void CCoil::SetInsideLimit(
    const long insidelimit
)
{
    LPCTSTR FUNC_NAME = _T("CCoil::SetInsideLimit");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_coil_v) {
        CString str_msg(_T(""));
        str_msg.Format(_T("InsideLimit --> %d"), insidelimit);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

        // m_insidelimit = insidelimit;
        m_coil_v->inSideLimit = insidelimit ;   // V9.25 coil support@AMT

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_coil_v pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}


//***************************Method Header*************************************
//Method Name    : SetTuneLimit()
//Author         : PATNI/ASG
//Purpose        : Setting the tune limit
//*****************************************************************************
void CCoil::SetTuneLimit(
    const long tunelimit
)
{
    LPCTSTR FUNC_NAME = _T("CCoil::SetTuneLimit");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_coil_v) {
        CString str_msg(_T(""));
        str_msg.Format(_T("TuneLimit --> %d"), tunelimit);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

        // m_tunelimit = tunelimit;
        m_coil_v->tuneLimit = tunelimit ;   // V9.25 coil support@AMT

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_coil_v pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//***************************Method Header*************************************
//Method Name    : SetRFLimit()
//Author         : PATNI/ASG
//Purpose        : Setting the RF limit
//*****************************************************************************
void CCoil::SetRFLLimit(
    const long rfl_limit
)
{
    LPCTSTR FUNC_NAME = _T("CCoil::SetRFLLimit");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_coil_v) {
        CString str_msg(_T(""));
        str_msg.Format(_T("RFLLimit --> %d"), rfl_limit);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

        // m_rfl_limit = rfl_limit;
        m_coil_v->rflLimit = rfl_limit ;    // V9.25 coil support@AMT

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_coil_v pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//***************************Method Header*************************************
//Method Name    : SetCoilKey()
//Author         : PATNI/HAR
//Purpose        : Set the Coil Key
//*****************************************************************************
void CCoil::SetCoilKey(
    const unsigned long coil_key
)
{
    LPCTSTR FUNC_NAME = _T("CCoil::SetCoilKey");
    CString str_msg(_T(""));
    str_msg.Format(_T("CoilKey --> %d"), coil_key);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    m_coil_key = coil_key;
}

//***************************Method Header*************************************
//Method Name    : GetSpeederEnable()
//Author         : PATNI/ASG
//Purpose        : Getting the value of speeder enable
//*****************************************************************************
BOOL CCoil::GetSpeederEnable(
) const
{
    LPCTSTR FUNC_NAME = _T("CCoil::GetSpeederEnable");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_coil_v) {
        CString str_msg(_T(""));
        str_msg.Format(_T("Speeder Enable --> %d"), m_coil_v->speeder_enable);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

        // return m_speederenable;
        return m_coil_v->speeder_enable ;  // V9.25 coil support@AMT

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_coil_v pointer is NULL"));
        return FALSE;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//***************************Method Header*************************************
//Method Name    : SetSpeederEnable()
//Author         : PATNI/ASG
//Purpose        : Setting the value of speeder enable
//*****************************************************************************
void CCoil::SetSpeederEnable(
    const BOOL speederenable
)
{
    LPCTSTR FUNC_NAME = _T("CCoil::SetSpeederEnable");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_coil_v) {
        CString str_msg(_T(""));
        str_msg.Format(_T("Speeder Enable --> %d"), speederenable);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

        // m_speederenable = speederenable;
        m_coil_v->speeder_enable = speederenable;   // V9.25 coil support@AMT

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_coil_v pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//***************************Method Header*************************************
//Method Name    : GetCoilType()
//Author         : PATNI/ASG
//Purpose        : Getting the coil type
//*****************************************************************************
cdb_coil_type_t CCoil::GetCoilType(
)const
{
    LPCTSTR FUNC_NAME = _T("CCoil::GetCoilType");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_coil_v) {
        CString str_msg(_T(""));
        str_msg.Format(_T("CoilType --> %d"), m_coil_v->type);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

        // return m_coiltype;
        return m_coil_v->type;          // V9.25 coil support@AMT

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_coil_v pointer is NULL"));
        return CDB_TRANSMIT;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//***************************Method Header*************************************
//Method Name    : SetCoilType()
//Author         : PATNI/ASG
//Purpose        : Setting the coil type
//*****************************************************************************
void CCoil::SetCoilType(
    const cdb_coil_type_t& coiltype
)
{
    LPCTSTR FUNC_NAME = _T("CCoil::SetCoilType");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_coil_v) {
        CString str_msg(_T(""));
        str_msg.Format(_T("CoilType --> %d"), coiltype);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

        // m_coiltype = coiltype;
        m_coil_v->type = coiltype;      // V9.25 coil support@AMT

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_coil_v pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//***************************Method Header*************************************
//Method Name    : GetCoilDescription()
//Author         : PATNI/HAR
//Purpose        : Get the coil description
//*****************************************************************************
CString CCoil::GetCoilDescription(
) const
{
    LPCTSTR FUNC_NAME = _T("CCoil::GetCoilDescription");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_coil_v) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("CoilDescription --> ") + CString(m_coil_v->description));

        return m_coil_v->description;       // V9.25 coil support@AMT

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_coil_v pointer is NULL"));
        return _T("");
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//***************************Method Header*************************************
//Method Name    : SetCoilDescription()
//Author         : PATNI/HAR
//Purpose        : Set the coil description
//*****************************************************************************
void CCoil::SetCoilDescription(
    CString& coil_desp
)
{
    LPCTSTR FUNC_NAME = _T("CCoil::SetCoilDescription");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CoilDescription --> ") + coil_desp);

    // + V9.25 coil support@AMT
    // m_coil_description = coil_desp;
    // LibAcq AMT not required as its XmString in libAcq\AcqCoilSignal.h
    // m_coil_v->description = (char *)coil_desp.GetBuffer( coil_desp.GetLength() );
    // - V9.25 coil support@AMT
}



//***************************Method Header*************************************
//Method Name    : SetPort()
//Author         : PATNI/HAR
//Purpose        : Set the Port
//*****************************************************************************
//
//void CCoil::SetPort(
//    int port
//) {
//    CString l_port = _T("");
//    l_port.Format(_T("%c"), port) ;
//    CPQMLogMgr::GetInstance()->WriteTraceLog(DOMAIN_NAME,
//        USER_FUNC_MARKER,
//        _T("CCoil::SetPort"),
//        _T("Port --> ") + l_port);
//
//    // m_port = port;
//    m_coil_v->port = port ;     // V9.25 coil support@AMT
//}

//***************************Method Header*************************************
//Method Name    : GetChannel()
//Author         : PATNI/HAR
//Purpose        : Get the Channel
//*****************************************************************************
cdb_channels_e CCoil::GetChannel(
) const
{
    LPCTSTR FUNC_NAME = _T("CCoil::GetChannel");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_coil_v) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Channel --> ") + m_coil_v->channel);

        // return m_channel;
        return m_coil_v->channel;       // V9.25 coil support@AMT

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_coil_v pointer is NULL"));
        return CDB_CHANNEL_A;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//***************************Method Header*************************************
//Method Name    : SetChannel()
//Author         : PATNI/HAR
//Purpose        : Set the Channel
//*****************************************************************************
void CCoil::SetChannel(
    const cdb_channels_e channel
)
{
    LPCTSTR FUNC_NAME = _T("CCoil::SetChannel");

    // m_channel = channel;
    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_coil_v) {
        CString str_msg = _T("") ;
        str_msg.Format(_T("Channel --> %c"), channel) ;
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

        m_coil_v->channel = channel; // V9.25 coil support@AMT

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_coil_v pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}




