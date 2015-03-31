//Redmine-783
//Redmine-779
//Redmine-780
//Redmine-782
// IData.cpp: implementation of the IData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IData::IData():
    m_head(NULL)
{

}

IData::~IData()
{
    DEL_PTR(m_head);

}

void IData::DllFindHead(
)
{
    if (m_head) {					//Patni-MJC/2009Aug17/Modified/cpp test corrections
        DLL_FIND_HEAD(m_head);
    }
}

DllT* IData::GetHead(
) const
{
    return m_head;
}

void IData::SetHead(
    DllT* f_head
)
{
    if (NULL == f_head) {
        return;
    }

    m_head = f_head;
}

int IData::GetCount(
) const
{
    return 10;
}

