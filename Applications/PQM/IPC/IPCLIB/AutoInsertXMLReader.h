// AutoInsertXMLReader.h: interface for the CAutoInsertData class.
//
//////////////////////////////////////////////////////////////////////
#ifndef __SM_IPCLIB_AUTOINSERTXMLREADER_H__
#define __SM_IPCLIB_AUTOINSERTXMLREADER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AutoInsertXmlData.h"
#include "PQMXMLParser.h"

class CXMLTag;
class CAutoInsertXMLReader :
    public CPQMXMLParser
{
public :
    bool ReadAutoInsertProtocolXML(AutoInsertXMLData& f_auto_insert_data);

private:
    void InsertData(CXMLTag* f_child, CList<ShimMapStudy*, ShimMapStudy*>& f_shim_study);

};

#endif // #ifndef __SM_IPCLIB_AUTOINSERTXMLREADER_H__
