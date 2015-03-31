// AutoInsertXmlDataManager.h: interface for the CAutoInsertData class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_AUTOINSERTXMLDATAMANAGER_H__
#define __SM_IPCLIB_AUTOINSERTXMLDATAMANAGER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>
#include "AutoInsertXmlData.h"
#include "AutoInsertConstant.h"

class CPqmProtocol;

class CAutoInsertXmlDataManager :
    private MR::ACQ::SM::COMMON::NonCopyable<CAutoInsertXmlDataManager>
{
public:
    CAutoInsertXmlDataManager();
    virtual ~CAutoInsertXmlDataManager();

    bool ReadAutoInsertProtocolXML();
    ShimMapStudy* GenerateMapShimData(CPqmProtocol* const f_targ_protocol , const scan_mode f_mode);
    ShimMapStudy* GetAutoShimMapData() const ;

private:
    ShimMapStudy* m_shimmapstudy_ptr;
    AutoInsertXMLData m_study_file_selection;
    ShimMapStudy* GetAutoShimMapData(CPqmProtocol* const f_targ_protocol , const scan_mode f_mode) const;

    bool IsConditionSatisfied(const TagInformation& f_tag_info , CPqmProtocol* f_targ_protocol, const scan_mode f_mode)const ;
    bool IsConditionSatisfiedForCoil(const TagInformation& f_tag_info , CPqmProtocol* const f_targ_protocol, bool& f_is_checking_this, const scan_mode f_mode)const ;
    bool IsConditionSatisfiedForSar(const TagInformation& f_tag_info , CPqmProtocol* const f_targ_protocol, bool& f_is_checking_this)const ;
    int GetAnatomyValue(const CString& f_str_anatomy)const;
    int IsCoilConditionSatisfiedForAutoMap(const TagInformation& f_tag_info, CPqmProtocol* const f_targ_protocol, bool f_is_checking_this)const;
    int IsCoilConditionSatisfiedForAutoShimming(const TagInformation& f_tag_info, CPqmProtocol* const f_targ_protocol, bool f_is_checking_this)const;
};

#endif // #ifndef __SM_IPCLIB_AUTOINSERTXMLDATAMANAGER_H__
