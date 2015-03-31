//REDMINE-1213
//Redmine-783
//Redmine-779
//Redmine-780
//Redmine-781
//Redmine-781
//Redmine-782
// DataManager.cpp: implementation of the CDataManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataManager.h"
#include <PQM/PQMLogMgr.h>
#include "PQMSectionData.h"
#include "PQMCoilData.h"
#include "SelectionCoilData.h"
#include "SelectionSectionData.h"
#include "PqmPathData.h"
#include "interlockdata.h"
#include "CoilDBV.h"
#include "SectionDB.h"
#include "PathDB.h"
#include "interlockdb.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CDataManager* CDataManager::m_data_mgr = NULL;

//****************************Method Header************************************
//Method Name   : GetInstance()
//Author        : PATNI\AMT
//Purpose       :
//*****************************************************************************
CDataManager* CDataManager::GetInstance(
    //   CPqm *f_pqm;
)
{
    if (NULL == m_data_mgr) {
        m_data_mgr = new CDataManager();
    }

    return m_data_mgr;
}

//+Patni-AMT/2009Nov16/Modified/DeFT#MVC004996
//****************************Method Header************************************
//Method Name   : InitializeDB()
//Author        : PATNI\AMT
//Purpose       :
//*****************************************************************************
bool CDataManager::InitializeDB(
)
{
    LPCTSTR FUNC_NAME = _T("CDataManager::InitializeDB");

    bool l_ret_value = ReadCoilDB();

    if (l_ret_value == false) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ReadCoilDB() failed."));
        return false;
    }

    l_ret_value = ReadSectionDB();

    if (l_ret_value == false) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ReadSectionDB() failed."));
        return false;
    }

    l_ret_value = ReadSelectionCoilDB();

    if (l_ret_value == false) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ReadSelectionCoilDB() failed."));
        return false;
    }

    l_ret_value = ReadPathDB();

    if (l_ret_value == false) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ReadPathDB() failed."));
        return false;
    }

    l_ret_value = ReadInterlockDB();

    if (l_ret_value == false) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ReadInterlockDB() failed."));
        return false;
    }

    //ReadGainDB();

    return true;
}
//-Patni-AMT/2009Nov16/Modified/DeFT#MVC004996

//****************************Method Header************************************
//Method Name   : ReadSelectionSectionDB()
//Author        : PATNI\HAR
//Purpose       :
//*****************************************************************************
bool CDataManager::ReadSelectionSectionDB(
    int f_test,
    ...
)
{
    CSectionDB* l_db = new CSectionDB();

    DEL_PTR(m_selection_section_data);

    m_selection_section_data = new CSelectionSectionData();
    //va_list args;
    va_list args = 0;
    va_start(args, f_test);
    bool l_return_val = l_db->Search(m_selection_section_data, va_pass(args));
    va_end(args);

    DEL_PTR(l_db);

    return l_return_val;
}

//****************************Method Header************************************
//Method Name   : DestroyInstanc()
//Author        : PATNI\AMT
//Purpose       :
//*****************************************************************************
void CDataManager::DestroyInstanc(
)
{
    DEL_PTR(m_data_mgr);
}

//****************************Method Header************************************
//Method Name   : GetPQM()
//Author        : PATNI\AMT
//Purpose       :
//*****************************************************************************
CPqm* CDataManager::GetPQM() const
{
    return m_pqm;
}

//****************************Method Header************************************
//Method Name   : SetPQM()
//Author        : PATNI\AMT
//Purpose       :
//*****************************************************************************
bool CDataManager::SetPQM(
    CPqm* f_pqm
)
{
    if (NULL == f_pqm) {

        return false;

    } else {

        m_pqm = f_pqm;
        return true;
    }
}

//****************************Method Header************************************
//Method Name   : GetPQMSectionData()
//Author        : PATNI\AMT
//Purpose       :
//*****************************************************************************
CPqmSectionData* CDataManager::GetPQMSectionData(
) const
{
    return m_section_data;
}

//****************************Method Header************************************
//Method Name   : GetPQMCoilData()
//Author        : PATNI\AMT
//Purpose       :
//*****************************************************************************
CPqmCoilData* CDataManager::GetPQMCoilData() const
{
    return m_pqmcoil_data;
}

//****************************Method Header************************************
//Method Name   : GetSelectionCoilData()
//Author        : PATNI\HAR
//Purpose       :
//*****************************************************************************
CSelectionCoilData* CDataManager::GetSelectionCoilData(
) const
{
    return m_selection_coil_data;
}

//****************************Method Header************************************
//Method Name   : GetPqmPathData()
//Author        : PATNI\AMT
//Purpose       :
//*****************************************************************************
CPqmPathData* CDataManager::GetPqmPathData() const
{
    return m_pqm_path_data;
}

//****************************Method Header************************************
//Method Name   : GetInterlockData()
//Author        : PATNI\HAR
//Purpose       :
//*****************************************************************************
CInterlockData* CDataManager::GetInterlockData(
) const
{
    return m_interlock_data;
}

//****************************Method Header************************************
//Method Name   : CDataManager()
//Author        : PATNI\AMT
//Purpose       :
//*****************************************************************************
//+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
//CDataManager::CDataManager()
CDataManager::CDataManager():
    m_pqm(NULL),
    m_section_data(NULL),
    m_pqmcoil_data(NULL),
    m_selection_coil_data(NULL),
    m_selection_section_data(NULL),
    m_pqm_path_data(NULL),
    m_interlock_db(NULL),
    m_interlock_data(NULL)
{

}

//****************************Method Header************************************
//Method Name   : ~CDataManager()
//Author        : PATNI\AMT
//Purpose       :
//*****************************************************************************
CDataManager::~CDataManager(
)
{
    DEL_PTR(m_interlock_data);
    DEL_PTR(m_interlock_db);
    DEL_PTR(m_section_data);
    DEL_PTR(m_pqmcoil_data);
    DEL_PTR(m_selection_coil_data);
    DEL_PTR(m_selection_section_data);
    DEL_PTR(m_pqm_path_data);
    DEL_PTR(m_pqm_path_data);
}



//****************************Method Header************************************
//Method Name   : ReadSectionDB()
//Author        : PATNI\AMT
//Purpose       :
//*****************************************************************************
bool CDataManager::ReadSectionDB()
{
    //Patni-AMT/2009Nov16/Modified/DeFT#MVC004996
    CSectionDB*  l_db = new CSectionDB();

    DEL_PTR(m_section_data);

    m_section_data = new CPqmSectionData();

    bool        l_return_val = l_db->Search(m_section_data, -1, NULL);

    DEL_PTR(l_db);

    return l_return_val;
}


//****************************Method Header************************************
//Method Name   : ReadCoilDB()
//Author        : PATNI\AMT
//Purpose       :
//*****************************************************************************
bool CDataManager::ReadCoilDB()
{
    CCoilDBV* l_coil_db = new CCoilDBV();

    DEL_PTR(m_pqmcoil_data);

    m_pqmcoil_data = new CPqmCoilData();

    if (l_coil_db->Search(m_pqmcoil_data, CDB_KEY_INSTALLED, CDB_ON, NULL)) {

        DEL_PTR(l_coil_db);

        m_pqmcoil_data->CoilMaintain();
        return true;
    }

    DEL_PTR(l_coil_db);

    return false ;
}

//+Patni-AMT/2009Nov16/Modified/DeFT#MVC004996
//****************************Method Header************************************
//Method Name   : ReadInterlockDB()
//Author        : PATNI \ HAR
//Purpose       :
//*****************************************************************************
bool CDataManager::ReadInterlockDB(
)
{
    bool l_ret_value = false;
    m_interlock_db		= new CInterlockDB();
    m_interlock_data	= new CInterlockData();

    if (m_interlock_db) {
        l_ret_value = m_interlock_db->Search(m_interlock_data);
    }

    return l_ret_value;
}
//-Patni-AMT/2009Nov16/Modified/DeFT#MVC004996

//****************************Method Header************************************
//Method Name   : ReadSelectionCoilDB()
//Author        : PATNI\HAR
//Purpose       :
//*****************************************************************************
bool CDataManager::ReadSelectionCoilDB(
)
{
    //Patni-AMT/2009Nov16/Added/DeFT#MVC004996

    CCoilDBV* l_coil_db = new CCoilDBV();

    DEL_PTR(m_selection_coil_data);

    m_selection_coil_data = new CSelectionCoilData();

    bool l_ret_value = l_coil_db->Search(m_selection_coil_data, CDB_KEY_INSTALLED, CDB_ON, NULL);

    DEL_PTR(l_coil_db);

    if (l_ret_value == true) {
        m_selection_coil_data->CoilMaintain();
    }

    return l_ret_value;
}//-Patni-AMT/2009Nov16/Added/DeFT#MVC004996



//****************************Method Header************************************
//Method Name   : ReadPathDB()
//Author        : PATNI\AMT
//Purpose       :
//*****************************************************************************
bool CDataManager::ReadPathDB()
{
    CPathDB* l_path_db = new CPathDB();

    DEL_PTR(m_pqm_path_data);


    m_pqm_path_data = new CPqmPathData();
    bool l_return_val = l_path_db->Search(m_pqm_path_data);

    DEL_PTR(l_path_db);

    return l_return_val;
}





