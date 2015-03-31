//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-774
//Redmine-778
//Redmine-779
//Redmine-780
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMXMLParser.cpp
 *  Overview: Implementation of CPQMXMLParser class.
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
// MSXMLParser.cpp: implementation of the CPQMXMLParser class.
#include "stdafx.h"
#include "PQMXMLParser.h"
#include <PDataParser/PSXMLParser.h>
#include <PDataParser/XMLTag.h>
#include <PDataParser/MSXML4DomParser.h>
#include <PQM/Resource.h>
#include <PQM/PQMLogMgr.h>
#include <PQM/PqmUtilities.h> // Added By KT/ACE-2/15-06-2009/Multilingualisation
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"

using namespace MR::ACQ::SM::COMMON;

//*****************************************************************************
// Function Name:- PQMXMLParser
// Author       :- PATNI\MSN
// Purpose      :- Constructor
//*****************************************************************************
CPQMXMLParser::CPQMXMLParser() :
    m_nodelist(NULL),
    m_nodecnt(0
             )
{
    LPCTSTR FUNC_NAME = _T("CPQMXMLParser::CPQMXMLParser");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMXMLParser Constructor"));
    // Init MSXML members
    m_nodelist.clear();
}

//*****************************************************************************
// Function Name:- ~CPQMXMLParser
// Author       :- PATNI\MSN
// Purpose      :- Destructor
//*****************************************************************************
CPQMXMLParser::~CPQMXMLParser(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMXMLParser::~CPQMXMLParser");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMXMLParser Destructor"));

    UTIL::DeleteAndClearContainer(&m_nodelist);
}

//*****************************************************************************
// Function Name:- LoadXMLFile
// Author       :- PATNI\MSN
// Purpose      :- Loads XML file specified by the filename
//*****************************************************************************
BOOL CPQMXMLParser::LoadXMLFile(
    CPTString filename
)
{
    LPCTSTR FUNC_NAME = _T("CPQMXMLParser::LoadXMLFile");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMXMLParser::LoadXMLFile"));

    struct _stat statusstruct;
    int filestatus = _wstat(filename, &statusstruct);

    if (filestatus != 0) {

        CString string = L"";
        string.LoadString(IDS_ERR_XMLFILE_LOADING_FAILED);

        CPQMLogMgr::GetInstance()->WriteEventLog(string,
                ERROR_LOG_INFORMATION1,
                _T("PQMIPCParser.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82


        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_XMLFILE_LOADING_FAILED")),
            _T("PQMIPCParser.cpp"),
            __LINE__,
            ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);

        return FALSE;
    }

    CMSXML4DomParser* domparser = new CMSXML4DomParser();

    try {
        domparser->LoadFromFile(filename);

    }   catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception Occured"));
        CPQMLogMgr::GetInstance()->WriteEventLog(
            _T("Invalid values present in XML File"),
            ERROR_LOG_INFORMATION1,
            _T("PQMXMLParser.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_INVALID_VALUES_IN_XML_FILE")),
            _T("PQMIPCParser.cpp"),
            __LINE__,
            ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        throw;
        return FALSE;
    }

    CXMLTag* root = (CXMLTag*) domparser->GetRoot();

    const int child_count = root->GetChildPSCount();

    // Sets the node count
    SetNodeCnt(child_count);

    for (int i = 0 ; i < child_count ; i++) {
        CXMLTag* child = (CXMLTag*) root->GetChildPS(i);
        PopulateData(*child, i);
    }

    DEL_PTR(domparser);

    return TRUE;
}

//*****************************************************************************
// Function Name:- ReadStringValue
// Author       :- PATNI\MSN
// Purpose      :- Function for changing CPTString to char*
//*****************************************************************************
char* CPQMXMLParser::ReadStringValue(
    CPTString string
)
{
    LPCTSTR FUNC_NAME = _T("CPQMXMLParser::ReadStringValue");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMXMLParser::ReadStringValue"));

    //+Patni-HAR/2009Aug31/Modified/Corrected call for wcstombs()
    int n = string.GetLength() + 1;
    char* char_string = new char[n];
    memset(char_string, 0, sizeof(char) * (n));	// MEM_INI AMIT 20081230
    wcstombs(char_string, string, n);
    //char_string[n] = '\0';
    //-Patni-HAR/2009Aug31/Modified/Corrected call for wcstombs()
    return char_string;
}


//*****************************************************************************
// Function Name:- ReadLongValue
// Author       :- PATNI\MSN
// Purpose      :- Function for changing CPTString to long
//*****************************************************************************
unsigned long CPQMXMLParser::ReadLongValue(
    CPTString string
)
{
    LPCTSTR FUNC_NAME = _T("CPQMXMLParser::ReadLongValue");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMXMLParser::ReadLongValue"));

    //-Patni-HAR/2009Aug31/Modified/Corrected call for wcstombs()
    int n = string.GetLength() + 1;
    char* char_string = new char[n];
    memset(char_string, 0, sizeof(char) * (n));  // MEM_INI AMIT 20081230
    wcstombs(char_string, string, n);
    //-Patni-HAR/2009Aug31/Modified/Corrected call for wcstombs()

    unsigned long value = atol(char_string);

    DEL_PTR_ARY(char_string);

    return value;
}

//*****************************************************************************
// Function Name:- ReadFloatValue
// Author       :- PATNI\MSN
// Purpose      :- Function for changing CPTString to float
//*****************************************************************************
float CPQMXMLParser::ReadFloatValue(
    CPTString string
)
{
    LPCTSTR FUNC_NAME = _T("CPQMXMLParser::ReadFloatValue");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMXMLParser::ReadFloatValue"));

    //+Patni-HAR/2009Aug31/Modified/Corrected call for wcstombs()
    int n = string.GetLength() + 1;
    char* char_string = new char[n];
    memset(char_string, 0, sizeof(char) * (n));   // MEM_INI AMIT 20081230
    wcstombs(char_string, string, n);
    //-Patni-HAR/2009Aug31/Modified/Corrected call for wcstombs()
    float value = (float)atof(char_string);

    DEL_PTR_ARY(char_string);

    return value;
}

//*****************************************************************************
// Function Name:- GetAttributeValue
// Author       :- PATNI\MSN
// Purpose      :- Gets the tag information in string format
//*****************************************************************************
CPTString CPQMXMLParser::GetAttributeValue(
    const int node_index,
    CPTString node_tagname,
    CPTString child_tagname,
    CPTString attrib_name,
    int list_pos
)
{
    LPCTSTR FUNC_NAME = _T("CPQMXMLParser::GetAttributeValue");

    //Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(attrib_name);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMXMLParser::GetAttributeValue"));

    if ((int)m_nodelist.size() <= list_pos) {
        return L"";
    }

    // traverse nodelist
    if (list_pos) {
        if (m_nodelist[list_pos]) {		//Patni-MJC/2009Aug17/Modified/cpp test corrections
            if (m_nodelist[list_pos]->m_child_tagname.GetName() == child_tagname) {
                return m_nodelist[list_pos]->m_attribvalue;
            }

        } else {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_nodelist[list_pos] pointer is NULL"));
        }

    } else {
        NODEDATA* node = NULL;

        for (int j = list_pos; j < (int)m_nodelist.size(); j++) {
            node = m_nodelist[j];

            if (node) {
                CPTString nodename = node->m_nodename.GetName();
                // check if node present

                if ((nodename == node_tagname) && (node->m_nodeindex == node_index)) {
                    CPTString childname = node->m_child_tagname.GetName();

                    // check if child present
                    if (childname == child_tagname) {
                        return node->m_attribvalue;
                    }
                }
            }
        }
    }

    return L"";
}

//*****************************************************************************
// Function Name:- GetTagPosition
// Author       :- PATNI\MSN
// Purpose      :- Gets the tag information in Structure format
//*****************************************************************************
int CPQMXMLParser::GetTagPosition(
    const int node_index,
    CPTString node_tagname,
    CPTString child_tagname,
    CPTString attrib_name
)
{
    LPCTSTR FUNC_NAME = _T("CPQMXMLParser::GetTagPosition");

    //Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(attrib_name);

    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMXMLParser::GetTagPosition"));

    NODEDATA* node = NULL;

    // traverse nodelist
    for (int j = 0; j < (int)m_nodelist.size(); j++) {
        node = m_nodelist[j];

        if (node) {
            CPTString nodename = node->m_nodename.GetName();

            // check if node present
            if ((nodename == node_tagname) && (node->m_nodeindex == node_index)) {
                CPTString childname = node->m_child_tagname.GetName();

                // check if child present
                if (childname == child_tagname) {
                    return node->m_listpos;
                }
            }
        }
    }

    return -1;
}

//*****************************************************************************
// Function Name:- GetNodeCnt
// Author       :- PATNI\MSN
// Purpose      :- Returns the Node Count
//*****************************************************************************
int CPQMXMLParser::GetNodeCnt(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMXMLParser::GetNodeCnt");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMXMLParser::GetNodeCnt"));
    return m_nodecnt;
}

//*****************************************************************************
// Function Name:- PopulateData
// Author       :- PATNI\MSN
// Purpose      :- Stores data into the data structure
//*****************************************************************************
void CPQMXMLParser::PopulateData(
    CXMLTag& tag,
    const int nodecnt
)
{
    LPCTSTR FUNC_NAME = _T("CPQMXMLParser::PopulateData");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMXMLParser::PopulateData"));

    // get the child count
    const int child_count = tag.GetChildPSCount();
    CXMLTag* child = NULL ;
    CPTString attribname = _T(""), attribvalue = _T("");

    CXMLTag::NV_POS pos = NULL ;

    const CXMLTag::NV_POS validate = NULL;

    for (int i = 0 ; i < child_count ; i++) {
        // get the child tag
        child = (CXMLTag*) tag.GetChildPS(i);

        if (child) {

            attribname = _T("");
            attribvalue = _T("");

            pos = child->GetFirstAttribute(attribname, attribvalue);             // get the first attribute

            while (pos != validate) {
                // set the tag information
                SetTagInformation(nodecnt, tag, *child, attribname, attribvalue);
                // get the next attribute
                pos = child->GetNextAttribute(pos, attribname, attribvalue);
            }
        }

        PopulateData(*child, i);

    }
}

//*****************************************************************************
// Function Name:- ReadWCharStringValue
// Author       :- PATNI\MSN
// Purpose      :- Function for changing CPTString to TCHAR*
//*****************************************************************************
WCHAR* CPQMXMLParser::ReadWCharStringValue(
    CPTString string
)
{
    LPCTSTR FUNC_NAME = _T("CPQMXMLParser::ReadWCharStringValue");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMXMLParser::ReadWCharStringValue"));

    int n = string.GetLength() + 1;
    TCHAR* tchar_string = new TCHAR[n];
    char* char_string = new char[n];
    memset(tchar_string , 0, sizeof(TCHAR) * n);    // MEM_INI AMIT 20081230
    memset(char_string, 0, sizeof(char) * n);   // MEM_INI AMIT 20081230
    wcstombs(char_string, string, n);
    //char_string[n] = '\0';
    mbstowcs(tchar_string, char_string, n);
    //tchar_string[n] = _T('\0');
    return tchar_string;
}

//*****************************************************************************
// Function Name:- SetNodeCnt
// Author       :- PATNI\MSN
// Purpose      :- Sets the Node Count
//*****************************************************************************
void CPQMXMLParser::SetNodeCnt(
    const int nodecnt
)
{
    LPCTSTR FUNC_NAME = _T("CPQMXMLParser::SetNodeCnt");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMXMLParser::SetNodeCnt"));
    m_nodecnt = nodecnt;
}

//*****************************************************************************
// Function Name:- SetTagInformation
// Author       :- PATNI\MSN
// Purpose      :- Sets the tag information
//*****************************************************************************
void CPQMXMLParser::SetTagInformation(
    const int node_index,
    CXMLTag& node_tagname,
    CXMLTag& child_tagname,
    CPTString attrib_name,
    CPTString attrib_value
)
{
    LPCTSTR FUNC_NAME = _T("CPQMXMLParser::SetTagInformation");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMXMLParser::SetTagInformation"));
    // add into the node list
    NODEDATA* nodeinfo = new NODEDATA();
    nodeinfo->m_nodename = node_tagname;
    nodeinfo->m_nodeindex = node_index;
    nodeinfo->m_child_tagname = child_tagname;
    nodeinfo->m_attribname = attrib_name;
    nodeinfo->m_attribvalue = attrib_value;
    nodeinfo->m_listpos = m_nodelist.size();
    m_nodelist.push_back(nodeinfo);
}



