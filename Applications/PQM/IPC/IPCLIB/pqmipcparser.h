//Remine-780
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: pqmipcparser.h
 *  Overview: Structure of CPQMIPCParser class.
 *  Last Modified: 2007/8/10 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2007/6/15 12:00:00     PQM5 Baselined code
 *   2.0          Patni            2007/8/10 12:00:00     Implemented PQM6 requirements & Formatted
 *
 *****************************************************************************/
#ifndef __SM_IPCLIB_PQM_IPCPARSER_H__
#define __SM_IPCLIB_PQM_IPCPARSER_H__

#ifdef _MSXMLPARSER

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <PXMLFWC/PSXMLParser.h>
#include <PXMLFWC/XMLTag.h>
#include <libStudyManager/NonCopyable.h>

#import "msxml4.dll" rename_namespace("MSXML4") named_guids

struct NODEDATA {
    CXMLTag m_nodename;     // node name
    int m_nodeindex;            // node count
    CXMLTag m_child_tagname;    // child node name
    CPTString m_attribname; // attribute name
    CPTString m_attribvalue;    // attribute value
    int m_listpos;          // node position in the list

    NODEDATA() :
        m_nodeindex(0),
        m_attribname(_T("")),
        m_attribvalue(_T("")),
        m_listpos(0) {
    }
};

using namespace std;

class CPQMIPCParser : private MR::ACQ::SM::COMMON::NonCopyable<CPQMIPCParser>
{
public:
    CPQMIPCParser();
    virtual ~CPQMIPCParser();

    // Function for loading data from xml file
    BOOL LoadXMLFile(CPTString filename);

    // Returns the Tag information of the nodes and child specified
    CPTString GetAttributeValue(const int node_index, CPTString node_tagname, CPTString child_tagname, CPTString attrib_name, int list_pos = 0);

    // Returns the Tag information of the nodes and child specified in structure format
    int GetTagPosition(const int node_index, CPTString node_tagname, CPTString child_tagname, CPTString attrib_name);

    // Converts PTString to long
    unsigned long ReadLongValue(CPTString string);

    // Converts PTString to char*
    char* ReadStringValue(CPTString string);

    // Converts PTString to float
    float ReadFloatValue(CPTString string);

    // Converts PTString to TCHAR*
    WCHAR* ReadWCharStringValue(CPTString string);

    // Returns the count of the nodes present in the file
    int GetNodeCnt()const;

private:
    // Function for loading data from tag specified
    void PopulateData(CXMLTag& tag, const int nodecnt);

    // Set the count of the nodes present in the file
    void SetNodeCnt(const int nodecnt);

    // Stores the Tag information in the node list
    void SetTagInformation(const int node_index, CXMLTag& node_tagname, CXMLTag& child_tagname, CPTString attrib_name, CPTString attrib_value);

private:
    vector <NODEDATA*> m_nodelist;
    int m_nodecnt;

};

#endif

#endif // #ifndef __SM_IPCLIB_PQM_IPCPARSER_H__ 

