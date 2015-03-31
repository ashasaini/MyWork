// DailogInterfaces.h: interface for the DailogInterfaces class.
//
//////////////////////////////////////////////////////////////////////
//TMSC-REDMINE-780

//Redmine-777
#ifndef __SM_DAILOG_INTERFACES_H__
#define __SM_DAILOG_INTERFACES_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PatientOrientationConstants.h"


class CPatientOrientationInterface
{
public :
    virtual void OnOrienationChangedFor(const OrientationType orientation_type)const = 0;
};



class CBaseInterfaceForQueueDialog
{

public :
    virtual void SendMessageToProcon(const ProconMessageEnum procon_message)const = 0;
};



class CBaseInterfaceForMRSPreScanDlg
{

public :
    virtual void MrsScanStartUiCB(SMRSScan_s* mrsscan_values)const  = 0;
    virtual void MrsScanAbort()const = 0;
    virtual void MrsScanClose()const = 0;
};


class CBaseAxisLocatorDialogInterface
{

public :
    virtual void OnAxisLocatorDialogClosed(bool ok_pressed = true)const = 0;
    virtual bool ApplySelectedCoils()const = 0;

    virtual float GetMagneticCenterPos()const = 0;
    virtual int GetChannelModeSelection() = 0;
    virtual BOOL GetIsChannelModeApplicable() = 0;
};

class CBaseInterfaceClassForScanCommentDlg
{

public :
    virtual bool CanModifyWithCommentDialog()const = 0;

    virtual bool GetCurrentScanCommentAndScanID(CString& f_scan_id, CString& f_scan_comment)const = 0;
    virtual void SetCurrentScanCommentAndScanID(CString* f_scan_id = NULL, CString* f_scan_comment = NULL)const = 0;

    virtual CString GetCurrentScanComment()const = 0;
    virtual CString GetCurrentScanID()const = 0;

};



#endif // #ifndef __SM_DAILOG_INTERFACES_H__
