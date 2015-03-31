//Redmine-783
//Redmine-780
// PqmPathData.cpp: implementation of the CPqmPathData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PqmPathData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPqmPathData::CPqmPathData()
{

}

CPqmPathData::~CPqmPathData()
{
    if (m_head) {			//Patni-MJC/2009Aug17/Modified/cpp test corrections
        // + Patni - HAR / 2009Mar13 / Added / Memory Leak
        while (m_head->next) {
            m_head = m_head->next;
            FREE_PTR(m_head->prev->item);
            FREE_PTR(m_head->prev);
        }

        FREE_PTR(m_head->item);
        FREE_PTR(m_head);
        // - Patni - HAR / 2009Mar13 / Added / Memory Leak
    }
}


// + 578 AMIT PATEL Shimming WB Onsite
//****************************Method Header************************************
//Method Name   : GetPathDataFromKey()
//Author        : PATNI\AMT
//Purpose       : Get the path record based on f_key
//*****************************************************************************
bool CPqmPathData::GetPathDataFromKey(
    const int f_key,
    path_db_rec_p f_rec
)
{
    DllT* l_node = dllNodeType(m_head, f_key);

    if (l_node == NULL) {
        return false;
    }

    memcpy(f_rec, ((path_db_rec_p)l_node->item), sizeof(path_db_rec_t)) ;

    return true;
}
// - 578 AMIT PATEL Shimming WB Onsite

void CPqmPathData::AddData(
    void* f_key,
    path_db_rec_p  f_db_rec_p
)
{
    path_db_rec_p 	l_rec_p = (path_db_rec_p)malloc(sizeof(path_db_rec_t));

    if (l_rec_p == NULL) {

#ifdef NEED_MODI
        DB_PRINTF("", ("%s:%d malloc error\n", __FILE__, __LINE__));
        PqmUiSafeExit(PQM_FATAL);
        // return;   //Patni-RUP/2009Aug24/Modified/cpp test corrections
#endif
        return;   //Patni-RUP/2009Aug24/Modified/cpp test corrections
    }

    memcpy(l_rec_p, f_db_rec_p, sizeof(path_db_rec_t));

    l_rec_p->description  = NULL;
    l_rec_p->len_of_descr = 0;

    dllAppendNode(m_head, l_rec_p, l_rec_p->path_id, &m_head);
}