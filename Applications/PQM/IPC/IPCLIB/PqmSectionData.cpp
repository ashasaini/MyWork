//Redmine-779
// PqmSectionData.cpp: implementation of the CPqmSectionData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PqmSectionData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPqmSectionData::CPqmSectionData()
{

}

CPqmSectionData::~CPqmSectionData()
{
    if (m_head) {		//Patni-MJC/2009Aug17/Modified/cpp test corrections
        // + Patni - HAR / 2009Mar13 / Added /Memory Leak
        while (m_head->next) {
            m_head = m_head->next;
            FREE_PTR(m_head->prev->item);
            FREE_PTR(m_head->prev);
        }

        FREE_PTR(m_head->item);
        FREE_PTR(m_head);
        // - Patni - HAR / 2009Mar13 / Added /Memory Leak
    }
}

void CPqmSectionData::AddData(
    void* f_key,
    sedb_rec_p f_db_rec_p
)
{
    sedb_rec_p 	    l_rec_p = NULL;

    l_rec_p = (sedb_rec_p)malloc(sizeof(sedb_rec_t));

    if (NULL == l_rec_p) {
#ifdef NEED_MODI
        DB_PRINTF("", ("%s:%d malloc error\n", __FILE__, __LINE__));
        PqmUiSafeExit(PQM_FATAL);
#endif
        return;
    }

    memcpy(l_rec_p, f_db_rec_p, sizeof(sedb_rec_t));

    l_rec_p->description = NULL;
    l_rec_p->len_of_descr = 0;
    dllAppendNode(m_head, l_rec_p, l_rec_p->coil_id, &m_head);
}