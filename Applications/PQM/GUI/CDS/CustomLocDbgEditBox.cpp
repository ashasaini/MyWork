// CustomLocDbgEditBox.cpp: implementation of the CCustomLocDbgEditBox class.
///////////////////////////////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "CustomLocDbgEditBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(CCustomLocDbgEditBox, EditCtrlEx)
    ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

//***************************************************************************************
//Method Name   :  CCustomLocDbgEditBox
//Author        :  iGATE
//Purpose       :  Constructor
//***************************************************************************************
CCustomLocDbgEditBox::CCustomLocDbgEditBox()
{
}

//***************************************************************************************
//Method Name   :  CCustomLocDbgEditBox
//Author        :  iGATE
//Purpose       :  Destructor
//***************************************************************************************
CCustomLocDbgEditBox::~CCustomLocDbgEditBox()
{
}

//***************************************************************************************
//Module         : OnContextMenu
//Author         : iGATE/SP
//Purpose        : Right Click Default Window should not be displayed
//***************************************************************************************
void CCustomLocDbgEditBox::OnContextMenu(CWnd* pNewWnd, CPoint point)
{

}