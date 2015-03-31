// AutoLocatorParameterSetter.h: interface for the CAutoLocatorParameterSetter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUTOLOCATORPARAMETERSETTER_H__46545211_EE0F_4CA0_AE84_1F887EBAFBB6__INCLUDED_)
#define AFX_AUTOLOCATORPARAMETERSETTER_H__46545211_EE0F_4CA0_AE84_1F887EBAFBB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>

class CPqm;
class CPqmProtocol;
class CPqmPrMode;

class CAutoLocatorParameterSetter : private MR::ACQ::SM::COMMON::NonCopyable<CAutoLocatorParameterSetter>
{
public:
    CAutoLocatorParameterSetter(CPqm* f_pqmptr);
    virtual ~CAutoLocatorParameterSetter();
    BOOL SetOffsetParameters(CPqmProtocol* f_protocol);
    void GetRefParentInfo(CPqmProtocol* f_protocol, CPqmPrMode* f_parent_info);
private:
    CPqm* m_pqmptr;
};

#endif // !defined(AFX_AUTOLOCATORPARAMETERSETTER_H__46545211_EE0F_4CA0_AE84_1F887EBAFBB6__INCLUDED_)
