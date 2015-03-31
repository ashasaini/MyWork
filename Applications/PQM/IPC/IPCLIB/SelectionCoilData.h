//Redmine-783
//Redmine-774
//Redmine-780
// SectionCoilData.h: interface for the CSectionCoilData class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_SECTION_COILDATA_H__
#define __SM_IPCLIB_SECTION_COILDATA_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tami/libgpdb/coil.h>
#include <PQM/CoilDefinitions.h>
#include "ICoilData.h"

class CSelectionCoilData : public ICoilData
{
public:
    //Constructor / Destructor
    CSelectionCoilData();
    virtual ~CSelectionCoilData();

    void CoilMaintain()const ;

    CoilSelectionCoil_t* GetSelectionCoilData();

private:

    CoilSelectionCoil_t* m_temp_selection_coil;

    void AddData(void* f_key, cdb_rec_p f_db_rec_p);
    BOOL CoilInstallCheckGroupCoil(unsigned short* f_group_id)const ;

};
#endif // #ifndef __SM_IPCLIB_SECTION_COILDATA_H__
