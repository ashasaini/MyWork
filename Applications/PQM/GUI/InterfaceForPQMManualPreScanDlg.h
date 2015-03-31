//TMSC-REDMINE-783
//TMSC-REDMINE-782
//TMSC-REDMINE-780
// InterfaceForPQMManualPreScanDlg.h: interface for the InterfaceForPQMManualPreScanDlg class.
//
//////////////////////////////////////////////////////////////////////
//Redmine-775

//Redmine-777
#ifndef __SM_INTERFACE_FOR_PQM_MANUAL_PRESCAN_DLG_H__
#define __SM_INTERFACE_FOR_PQM_MANUAL_PRESCAN_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>


class BaseInterfaceForPQMManualPreScanDlg
{

public :

    virtual void PQMManualPrescanClose()const = 0;
    virtual void PQMManualPrescanApply(VARIANT* f_pdata)const = 0;
    virtual BOOL GetManualPreScanDefaultData(VARIANT** f_pdata)const = 0;
    virtual void SetPrescanHook(const BOOL f_prescan_hook)const = 0;
};


class CPQMView ;

class InterfaceForPQMManualPreScanDlg : public BaseInterfaceForPQMManualPreScanDlg,
    private MR::ACQ::SM::COMMON::NonCopyable<InterfaceForPQMManualPreScanDlg>
{

public:
    explicit InterfaceForPQMManualPreScanDlg(CPQMView* parent);
    virtual ~InterfaceForPQMManualPreScanDlg();
private:
    CPQMView* m_parent;

    virtual void PQMManualPrescanClose()const;
    virtual void PQMManualPrescanApply(VARIANT* f_pdata)const;
    virtual BOOL GetManualPreScanDefaultData(VARIANT** f_pdata)const;
    virtual void SetPrescanHook(const BOOL f_prescan_hook)const ;

};

#endif // #ifndef __SM_INTERFACE_FOR_PQM_MANUAL_PRESCAN_DLG_H__
