//TMSC-REDMINE-783
//TMSC-REDMINE-782
//TMSC-REDMINE-780
//Redmine-775
//Redmine-777

// InterfaceClasses.h: interface for the InterfaceClasses class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_INTERFACE_CLASSES_H__
#define __SM_INTERFACE_CLASSES_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>
#include "DailogInterfaces.h"


class CPQMView;

class CPatientOrientationHandler : public CPatientOrientationInterface,
    private MR::ACQ::SM::COMMON::NonCopyable<CPatientOrientationHandler>
{
public :
    explicit CPatientOrientationHandler(CPQMView* pqm_view);

private:
    CPQMView* m_pqm_view;

    virtual void OnOrienationChangedFor(const OrientationType orientation_type)const ;
};

class CQueueDialogHandler : public CBaseInterfaceForQueueDialog,
    private MR::ACQ::SM::COMMON::NonCopyable<CQueueDialogHandler>
{
public :
    explicit CQueueDialogHandler(CPQMView* pqm_view);
    virtual void SendMessageToProcon(const ProconMessageEnum procon_message)const;
private:
    CPQMView* m_pqm_view;
};

class CMRSPreScanDlgHandler : public CBaseInterfaceForMRSPreScanDlg,
    private MR::ACQ::SM::COMMON::NonCopyable<CMRSPreScanDlgHandler>
{
public :
    explicit CMRSPreScanDlgHandler(CPQMView* pqm_view);

private:
    CPQMView* m_pqm_view;
    void MrsScanStartUiCB(SMRSScan_s* mrsscan_values)const ;
    void MrsScanAbort()const;
    void MrsScanClose()const;
};


class CAxisLocatorDialogInterface : public CBaseAxisLocatorDialogInterface,
    private MR::ACQ::SM::COMMON::NonCopyable<CAxisLocatorDialogInterface>
{
public :
    explicit CAxisLocatorDialogInterface(CPQMView* pqm_view);
private:
    CPQMView* m_pqm_view;
    virtual void OnAxisLocatorDialogClosed(bool ok_pressed = true)const;
    virtual bool ApplySelectedCoils()const;
    virtual float GetMagneticCenterPos()const ;
    virtual int GetChannelModeSelection();
    virtual BOOL GetIsChannelModeApplicable();
};

class CInterfaceClassForScanCommentDlg : public CBaseInterfaceClassForScanCommentDlg,
    private MR::ACQ::SM::COMMON::NonCopyable<CInterfaceClassForScanCommentDlg>
{
public :
    explicit CInterfaceClassForScanCommentDlg(CPQMView* pqm_view);

private:
    CPQMView* m_pqm_view;
    virtual bool CanModifyWithCommentDialog()const ;

    virtual bool GetCurrentScanCommentAndScanID(CString& f_scan_id, CString& f_scan_comment)const ;
    virtual void SetCurrentScanCommentAndScanID(CString* f_scan_id = NULL, CString* f_scan_comment = NULL)const ;

    virtual CString GetCurrentScanComment()const ;
    virtual CString GetCurrentScanID()const;
};








#endif // #ifndef __SM_INTERFACE_CLASSES_H__
