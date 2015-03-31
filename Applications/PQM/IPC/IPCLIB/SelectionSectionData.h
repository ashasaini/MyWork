//Redmine-783
// SelectionSectionData.h: interface for the CSelectionSectionData class.
//
//////////////////////////////////////////////////////////////////////
#ifndef __SM_IPCLIB_SELECTION_SECTIONDATA_H__
#define __SM_IPCLIB_SELECTION_SECTIONDATA_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tami/libgpdb/section.h>
#include "ISectionData.h"

class CSelectionSectionData : public ISectionData
{
public:
    //Constructor / Destructor
    CSelectionSectionData();
    virtual ~CSelectionSectionData();

private:

    void AddData(void* f_key, sedb_rec_p f_sedb_rec);

};
#endif // #ifndef __SM_IPCLIB_SELECTION_SECTIONDATA_H__
