// DefaultPGUIDXMLReader.h: interface for the CDefaultPGUIDXMLReader class.
//
//////////////////////////////////////////////////////////////////////

/*#if !defined(AFX_DEFAULTPGUIDXMLREADER_H__3F0606D7_3499_4645_84ED_D85A457F89F9__INCLUDED_)
#define AFX_DEFAULTPGUIDXMLREADER_H__3F0606D7_3499_4645_84ED_D85A457F89F9__INCLUDED_*/

#ifndef __SM_IPCLIB_DEFAULTPGUIDXMLREADER_H__
#define __SM_IPCLIB_DEFAULTPGUIDXMLREADER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PGUIData.h"
#include "PQMXMLParser.h"

class CXMLTag;
class CDefaultPGUIDXMLReader : public CPQMXMLParser
{
public:
    CDefaultPGUIDXMLReader();
    virtual ~ CDefaultPGUIDXMLReader();

    bool ReadDefaultPGUIDXML(PGUIData& f_pguid_insert_data);
private:
    void InsertData(CXMLTag* f_child, DefaultPGUIDXMLData* f_defaultpguidxmldata, TagDataInformation* l_tagdata_info);
};

#endif // #ifndef __SM_IPCLIB_DEFAULTPGUIDXMLREADER_H__
