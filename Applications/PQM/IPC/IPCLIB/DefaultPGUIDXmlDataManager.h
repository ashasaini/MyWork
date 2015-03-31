// DefaultPGUIDXmlDataManager.h: interface for the CDefaultPGUIDXmlDataManager class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_DEFAULTPGUIDXMLDATAMANAGER_H__
#define __SM_IPCLIB_DEFAULTPGUIDXMLDATAMANAGER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>
#include "PGUIData.h"
#include "PQMStudy.h"

class CPqmProtocol;

class CDefaultPGUIDXmlDataManager :
    private MR::ACQ::SM::COMMON::NonCopyable<CDefaultPGUIDXmlDataManager>
{
public:
    CDefaultPGUIDXmlDataManager();
    virtual ~CDefaultPGUIDXmlDataManager();
    bool ReadDefaultPGUIDXML();
    CString GetPGUIDFromXML(int f_inscan_type, const int f_acq_order, CPQMStudy* f_pqm_study)const;
private:
    PGUIData	m_pguid_insert_data;
    bool IsConditionSatisfied(const DefaultPGUIDXMLData& f_xml_info , const int f_inscan_type, bool f_is_checking_thisl)const;
    CString GetInScanString(int f_inscantype)const;
    CString GetAnatomyString(int f_anatomy)const;
};
#endif // #ifndef __SM_IPCLIB_DEFAULTPGUIDXMLDATAMANAGER_H__

