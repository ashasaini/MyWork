// AutoInsertXmlData.h: interface for the AutoInsertXmlData class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_AUTOINSERTXMLDATA_H
#define __SM_IPCLIB_AUTOINSERTXMLDATA_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum ScanRegionLimitEnum { ScanRegionLimit_SL = 0, ScanRegionLimit_PE, ScanRegionLimit_RO , ScanRegionLimit_Total};

struct TagInformation {
    CString m_tag_name;
    CString m_value;
    TagInformation();
    TagInformation(const CString& f_tag_name, const CString& f_value) ;
    ~TagInformation();
};

struct Condition {
    CList<TagInformation*, TagInformation*> m_tag_info;
    Condition();
    ~Condition();
};

struct ScanRegionEnlargement {
    CString m_tag;
    long m_percentage_range;
    ScanRegionEnlargement();
    ScanRegionEnlargement(const CString& f_tag, const long f_percentage_range);
    ~ScanRegionEnlargement();
};

struct ScanRegionLimit {
    CString m_tag;
    float m_limit_range;
    ScanRegionLimit();
    ScanRegionLimit(const CString& f_tag, const float f_limit_range);
    ~ScanRegionLimit();
};

struct ShimMapStudy {
    CString m_filename;
    CList<Condition* , Condition*> m_condition_list;
    CList<ScanRegionEnlargement*, ScanRegionEnlargement*> m_scanenlargement_list;
    ScanRegionLimit m_scanregionlimit_list[ScanRegionLimit_Total];

    ShimMapStudy();
    ~ShimMapStudy();

};

struct AutoInsertXMLData {
    CList<ShimMapStudy*, ShimMapStudy* > m_shim_study;
    CList<ShimMapStudy*, ShimMapStudy*> m_map_study;
    AutoInsertXMLData();
    ~AutoInsertXMLData();
};


#endif // #ifndef __SM_IPCLIB_AUTOINSERTXMLDATA_H
