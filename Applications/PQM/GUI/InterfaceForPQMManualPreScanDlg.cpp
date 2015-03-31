//TMSC-REDMINE-782
//TMSC-REDMINE-780
//TMSC-REDMINE-778
// InterfaceForPQMManualPreScanDlg.cpp: implementation of the InterfaceForPQMManualPreScanDlg class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "InterfaceForPQMManualPreScanDlg.h"
#include "PQMView.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

InterfaceForPQMManualPreScanDlg::InterfaceForPQMManualPreScanDlg(
    CPQMView* parent
):	m_parent(parent)
{
}

InterfaceForPQMManualPreScanDlg::~InterfaceForPQMManualPreScanDlg()
{

}


void InterfaceForPQMManualPreScanDlg::PQMManualPrescanClose(
)const
{
    LPCTSTR FUNC_NAME = _T("InterfaceForPQMManualPreScanDlg::PQMManualPrescanClose");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_parent) {

        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("m_parent pointer is NULL"));

    } else {
        //-Patni-DKH/2009Aug24/Modified/cpp test corrections
        m_parent->PQMManualPrescanClose();
    }//Patni-DKH/2009Aug24/Modified/cpp test corrections

}


void InterfaceForPQMManualPreScanDlg::PQMManualPrescanApply(
    VARIANT* f_pdata
)const
{
    LPCTSTR FUNC_NAME = _T("InterfaceForPQMManualPreScanDlg::PQMManualPrescanApply");
    PQM_TRACE(USER_FUNC_MARKER,	FUNC_NAME,	_T("Trace"));

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_parent) {

        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("m_parent pointer is NULL"));

    } else {
        int l_scan_idx = m_parent->GetScanIndex();
        BOOL l_bASCompassflag = m_parent->CheckASCompassOnOff(l_scan_idx);

        if (FALSE == l_bASCompassflag) {
            //-Patni-DKH/2009Aug24/Modified/cpp test corrections
            m_parent->PQMManualPrescanApply(f_pdata);

        } else {
            m_parent->PQMManualPrescanClose();
            CPQMLogMgr::GetInstance()->DisplayScanError(
                IDS_ERR_SCAN_ABORTED, _T("IDS_ERR_SCAN_ABORTED"),
                SCAN_ERR_NOTIFY,
                L"CPQMView.cpp");
        }
    }
}



//Patni-AMT/2009Jul06/Modified/ Code Review + TMSC comment
BOOL InterfaceForPQMManualPreScanDlg::GetManualPreScanDefaultData(
    VARIANT** f_pdata
)const
{
    LPCTSTR FUNC_NAME = _T("InterfaceForPQMManualPreScanDlg::GetManualPreScanDefaultData");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_parent) {

        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("m_parent pointer is NULL"));

        return FALSE;

    } else {
        //-Patni-DKH/2009Aug24/Modified/cpp test corrections
        return m_parent->GetManualPreScanDefaultData(f_pdata);
    }//Patni-DKH/2009Aug24/Modified/cpp test corrections

}


void InterfaceForPQMManualPreScanDlg::SetPrescanHook(
    const BOOL f_prescan_hook
)const
{
    LPCTSTR FUNC_NAME = _T("InterfaceForPQMManualPreScanDlg::SetPrescanHook");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_parent) {

        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("m_parent pointer is NULL"));

    } else {
        //-Patni-DKH/2009Aug24/Modified/cpp test corrections
        m_parent->SetPrescanHook(f_prescan_hook);
    }//Patni-DKH/2009Aug24/Modified/cpp test corrections
}