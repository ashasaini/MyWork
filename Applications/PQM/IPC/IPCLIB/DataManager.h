//Redmine-783
//Redmine-780
// DataManager.h: interface for the CDataManager class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_DATAMANAGER_H__
#define __SM_IPCLIB_DATAMANAGER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>

class CPqm;
class CPqmSectionData;
class CPqmCoilData;
class CSelectionCoilData;
class CSelectionSectionData;
class CPqmPathData;
class CInterlockDB;
class CInterlockData;

class CDataManager : private MR::ACQ::SM::COMMON::NonCopyable<CDataManager>
{

public:

    static CDataManager* GetInstance(/*CPqm * f_pqm*/);

    bool InitializeDB();
    bool ReadSelectionSectionDB(int f_test, ...);

    static void DestroyInstanc();

    CPqm* GetPQM() const;
    bool  SetPQM(CPqm* f_pqm);

    CPqmSectionData* GetPQMSectionData() const;
    CPqmCoilData* GetPQMCoilData() const;
    CSelectionCoilData* GetSelectionCoilData() const;
    CPqmPathData* GetPqmPathData() const;
    CInterlockData* GetInterlockData() const;

private:

    static  CDataManager*       m_data_mgr;

    CPqm*               m_pqm;
    CPqmSectionData*    m_section_data;
    CPqmCoilData*       m_pqmcoil_data;
    CSelectionCoilData*	m_selection_coil_data;
    CSelectionSectionData* m_selection_section_data;
    CPqmPathData*       m_pqm_path_data;
    CInterlockDB*		m_interlock_db;
    CInterlockData*		m_interlock_data;

    CDataManager(/*CPqm* f_pqm*/);

    virtual ~CDataManager();

    bool ReadSectionDB();
    bool ReadCoilDB();
    bool ReadInterlockDB();
    bool ReadSelectionCoilDB();
    bool ReadPathDB();

};

#endif // #ifndef __SM_IPCLIB_DATAMANAGER_H__ 
