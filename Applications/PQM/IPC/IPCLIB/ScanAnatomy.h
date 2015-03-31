// ScanAnatomy.h: interface for the CScanAnatomy class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_SCANANATOMY_H__
#define __SM_IPCLIB_SCANANATOMY_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>

class CScanAnatomy : private MR::ACQ::SM::COMMON::NonCopyable<CScanAnatomy>
{
public:
    CScanAnatomy();
    virtual ~CScanAnatomy();

};
#endif // #ifndef __SM_IPCLIB_SCANANATOMY_H__ 

