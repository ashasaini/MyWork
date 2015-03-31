//Redmine-783
// ScanCommentXMLReader.h: interface for the CScanCommentXMLReader class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_SCANCOMMENT_XMLREADER_H__
#define __SM_IPCLIB_SCANCOMMENT_XMLREADER_H__
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PQMXMLParser.h"

#include "PQM\PQMCommonStructures.h"

class CScanCommentXMLReader : public CPQMXMLParser
{

public:
    //Constructor / Destructor
    CScanCommentXMLReader();
    virtual ~CScanCommentXMLReader();

    bool GetScanCommentXMLData(VARIANT** f_scan_comment_data);

private:

    bool IterateChild(CXMLTag* f_child, CList<CString, CString>* f_list);


};
#endif // #ifndef __SM_IPCLIB_SCANCOMMENT_XMLREADER_H__
