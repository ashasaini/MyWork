//Redmine-783
//Redmine-780
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMXMLParser.h
 *  Overview: Structure of CPQMXMLParser class.
 *  Last Modified: 2008/6/10 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2007/6/15 12:00:00     PQM5 Baselined code
 *   2.0          Patni            2007/8/10 12:00:00     Implemented PQM6 requirements & Formatted
 *   3.0          Patni/LK         2008/6/10 12:00:00     class name had been
                                                            changed after TA review
 *****************************************************************************/
#ifndef __SM_IPCLIB_PQMXML_PARSER_H__
#define __SM_IPCLIB_PQMXML_PARSER_H__

#ifdef _MSXMLPARSER

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>
#include <PDataParser/XMLTag.h>

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

class CPQMXMLParser : private MR::ACQ::SM::COMMON::NonCopyable<CPQMXMLParser>
{
public:

    CPQMXMLParser();
    virtual ~CPQMXMLParser();

    //+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
    // Function for loading data from xml file
    //BOOL LoadXMLFile(CPTString filename);
    virtual BOOL LoadXMLFile(CPTString filename);
    //-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu

    // Converts PTString to char*
    char* ReadStringValue(CPTString string);

    // Converts PTString to long
    unsigned long ReadLongValue(CPTString string);

    // Converts PTString to float
    float ReadFloatValue(CPTString string);

    // Returns the Tag information of the nodes and child specified
    CPTString GetAttributeValue(const int node_index, CPTString node_tagname, CPTString child_tagname, CPTString attrib_name, int list_pos = 0);

    // Returns the Tag information of the nodes and child specified in structure format
    int GetTagPosition(const int node_index, CPTString node_tagname, CPTString child_tagname, CPTString attrib_name);

    // Returns the count of the nodes present in the file
    int GetNodeCnt()const;

private:

    vector <NODEDATA*> m_nodelist;
    int m_nodecnt;


    // Function for loading data from tag specified
    void PopulateData(CXMLTag& tag, const int nodecnt);

    // Converts PTString to TCHAR*
    WCHAR* ReadWCharStringValue(CPTString string);

    // Set the count of the nodes present in the file
    void SetNodeCnt(const int nodecnt);

    // Stores the Tag information in the node list
    void SetTagInformation(const int node_index, CXMLTag& node_tagname, CXMLTag& child_tagname, CPTString attrib_name, CPTString attrib_value);
};

#endif

#endif // #ifndef __SM_IPCLIB_PQMXML_PARSER_H__ 

