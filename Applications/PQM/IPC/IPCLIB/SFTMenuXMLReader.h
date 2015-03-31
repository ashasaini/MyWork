//Redmine-783
//Redmine-780
// SFTMenuXMLReader.h: interface for the CSFTMenuXMLReader class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_SFTMENU_XMLREADER_H__
#define __SM_IPCLIB_SFTMENU_XMLREADER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <PQM/PQMCommonStructures.h>
#include "PQMXMLParser.h"

class CSFTMenuXMLReader : public CPQMXMLParser
{
public:

    //Constructor / Destructor
    CSFTMenuXMLReader();
    virtual ~CSFTMenuXMLReader();

    virtual BOOL LoadXMLFile(CPTString filename);
    void GetSFTMenuXMLData(VARIANT** pData);

private:

    PopupMenu m_main_popup;
    void LoadPopupMenu(CXMLTag* root, const int& child_num, PopupMenu* l_popup)const;
    void FillMenuItem(MenuData* f_menu_item, CPTString f_param_name, CPTString f_param_val)const;
    BOOL LoadMenuItem(CXMLTag* child, PopupMenu* l_popup)const;


};
#endif // #ifndef __SM_IPCLIB_SFTMENU_XMLREADER_H__
