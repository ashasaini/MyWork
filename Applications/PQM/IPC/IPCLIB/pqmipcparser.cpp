//Redmine-778
//Remine-779
//Remine-780
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: pqmipcparser.cpp
 *  Overview: Implementation of CPQMIPCParser class.
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
// MSXMLParser.cpp: implementation of the CPQMIPCParser class.
#include "stdafx.h"
#include "PQMIPCParser.h"
#include <MSXML4DomParser.h>
#include <PQM/PQMLogMgr.h>
#include <PQM/Resource.h>
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"


using namespace MR::ACQ::SM::COMMON;

//*****************************************************************************

// Function Name:- CPQMIPCParser

// Author       :- PATNI\MSN

// Purpose      :- Constructor

//*****************************************************************************
CPQMIPCParser::CPQMIPCParser() :
    m_nodelist(NULL),
    m_nodecnt(0
             )
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCParser::CPQMIPCParser");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMIPCParser::CPQMIPCParser"));
    // Init MSXML members
    m_nodelist.clear();
}

//*****************************************************************************

// Function Name:- ~CPQMIPCParser

// Author       :- PATNI\MSN

// Purpose      :- Destructor

//*****************************************************************************
CPQMIPCParser::~CPQMIPCParser(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCParser::~CPQMIPCParser");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMIPCParser::~CPQMIPCParser"));

    UTIL::DeleteAndClearContainer(&m_nodelist);
}

//*****************************************************************************

// Function Name:- LoadXMLFile

// Author       :- PATNI\MSN

// Purpose      :- Loads XML file specified by the filename

//*****************************************************************************
BOOL CPQMIPCParser::LoadXMLFile(
    CPTString filename
)
{

    LPCTSTR FUNC_NAME = _T("CPQMIPCParser::LoadXMLFile");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMIPCParser::LoadXMLFile"));


    struct _stat statusstruct;
    int filestatus = _wstat(filename, &statusstruct);

    CString temp_string = _T("");                   // Added By KT/ACE-2/15-06-2009/Multilingualisation

    if (filestatus != 0) {

        //+Patni-PJS/2009May10/Modify/IR-82 review comment
        CString string = _T("");
        string.LoadString(IDS_ERR_XMLFILE_LOADING_FAILED);

        CPQMLogMgr::GetInstance()->WriteEventLog(string,
                ERROR_LOG_NOTIFY1,
                _T("PQMIPCParser.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82
        /*CPQMLogMgr::GetInstance()->WriteEventLog(IDS_ERR_XMLFILE_LOADING_FAILED,
                ERROR_LOG_NOTIFY1,
                _T("PQMIPCParser.cpp"));*/

        //-Patni-PJS/2009May10/Modify/IR-82 review comment

        temp_string = CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_XMLFILE_LOADING_FAILED"));    // Added By KT/ACE-2/15-06-2009/Multilingualisation

        CPQMLogMgr::GetInstance()->DisplayError(temp_string,
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
        CPQMLogMgr::GetInstance()->WriteEventLog(
            _T("Invalid values present in XML File"),
            ERROR_LOG_NOTIFY1,
            _T("CPQMIPCParser.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82

        temp_string = CPqmUtilities::GetMultiLingualString(_T("IDS_INVALID_VALUES_IN_XML_FILE"));    // Added By KT/ACE-2/15-06-2009/Multilingualisation

        CPQMLogMgr::GetInstance()->DisplayError(
            //   _T("Invalid values present in XML File"),		// Added By KT/ACE-2/15-06-2009/Multilingualisation
            temp_string,			// Added By KT/ACE-2/15-06-2009/Multilingualisation
            _T("PQMIPCParser.cpp"),
            __LINE__,
            ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        throw;
        return FALSE;
    }

    CXMLTag* root = (CXMLTag*) domparser->GetRoot();

    int child_count = root->GetChildPSCount();

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

// Function Name:- SetNodeCnt

// Author       :- PATNI\MSN

// Purpose      :- Sets the Node Count

//*****************************************************************************
void CPQMIPCParser::SetNodeCnt(
    int nodecnt
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCParser::SetNodeCnt");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMIPCParser::SetNodeCnt"));
    m_nodecnt = nodecnt;
}

//*****************************************************************************

// Function Name:- GetNodeCnt

// Author       :- PATNI\MSN

// Purpose      :- Returns the Node Count

//*****************************************************************************
int CPQMIPCParser::GetNodeCnt(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCParser::GetNodeCnt");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMIPCParser::GetNodeCnt"));
    return m_nodecnt;
}

//*****************************************************************************

// Function Name:- PopulateData

// Author       :- PATNI\MSN

// Purpose      :- Stores data into the data structure

//*****************************************************************************
void CPQMIPCParser::PopulateData(
    CXMLTag& tag,
    int nodecnt
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCParser::PopulateData");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMIPCParser::PopulateData"));
    // get the child count
    int child_count = tag.GetChildPSCount();

    for (int i = 0 ; i < child_count ; i++) {
        // get the child tag
        CXMLTag* child = (CXMLTag*) tag.GetChildPS(i);

        if (child) {
            CPTString attribname, attribvalue;
            CXMLTag::NV_POS pos = child->GetFirstAttribute(attribname, attribvalue);             // get the first attribute
            CXMLTag::NV_POS validate = NULL;

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

// Function Name:- SetTagInformation

// Author       :- PATNI\MSN

// Purpose      :- Sets the tag information

//*****************************************************************************
void CPQMIPCParser::SetTagInformation(
    int node_index,
    CXMLTag& node_tagname,
    CXMLTag& child_tagname,
    CPTString attrib_name,
    CPTString attrib_value
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCParser::SetTagInformation");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMIPCParser::SetTagInformation"));

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

//*****************************************************************************

// Function Name:- GetAttributeValue

// Author       :- PATNI\MSN

// Purpose      :- Gets the tag information in string format

//*****************************************************************************
CPTString CPQMIPCParser::GetAttributeValue(
    const int node_index,
    CPTString node_tagname,
    CPTString child_tagname,
    CPTString attrib_name,
    const int list_pos
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCParser::GetAttributeValue");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMIPCParser::GetAttributeValue"));

    if (m_nodelist.size() <= list_pos) {
        return L"";
    }

    // traverse nodelist
    if (list_pos) {
        if (m_nodelist[list_pos]->m_child_tagname.GetName() == child_tagname) {
            return m_nodelist[list_pos]->m_attribvalue;
        }

    } else {
        NODEDATA* node = NULL;

        for (int j = list_pos; j < m_nodelist.size(); j++) {
            node = m_nodelist[j];
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

    return L"";
}

//*****************************************************************************

// Function Name:- GetTagPosition

// Author       :- PATNI\MSN

// Purpose      :- Gets the tag information in Structure format

//*****************************************************************************
int CPQMIPCParser::GetTagPosition(
    const int node_index,
    CPTString node_tagname,
    CPTString child_tagname,
    CPTString attrib_name
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCParser::GetTagPosition");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMIPCParser::GetTagPosition"));

    NODEDATA* node = NULL;

    // traverse nodelist
    for (int j = 0; j < m_nodelist.size(); j++) {
        node = m_nodelist[j];
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

    return -1;
}

//*****************************************************************************

// Function Name:- ReadStringValue

// Author       :- PATNI\MSN

// Purpose      :- Function for changing CPTString to char*

//*****************************************************************************
char* CPQMIPCParser::ReadStringValue(
    CPTString string
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCParser::ReadStringValue");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMIPCParser::ReadStringValue"));

    //+Patni-HAR/2009Aug31/Modified/Corrected call for wcstombs()
    int n = string.GetLength() + 1;
    char* char_string = new char[n];
    memset(char_string, 0, sizeof(char) * (n));
    wcstombs(char_string, string, n);
    //char_string[n] = '\0';
    //-Patni-HAR/2009Aug31/Modified/Corrected call for wcstombs()
    return char_string;
}

//*****************************************************************************

// Function Name:- ReadWCharStringValue

// Author       :- PATNI\MSN

// Purpose      :- Function for changing CPTString to TCHAR*

//*****************************************************************************
WCHAR* CPQMIPCParser::ReadWCharStringValue(
    CPTString string
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCParser::ReadWCharStringValue");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMIPCParser::ReadWCharStringValue"));

    int n = string.GetLength();
    TCHAR* tchar_string = new TCHAR[n + 1];
    memset(tchar_string, 0, sizeof(TCHAR) * (n + 1));
    char* char_string = new char[n + 1];
    memset(char_string, 0, sizeof(char) * (n + 1));
    wcstombs(char_string, string, n + 1);	//+Patni-HAR/2009Aug31/Modified/Corrected call for wcstombs()
    //char_string[n] = '\0';
    mbstowcs(tchar_string, char_string, n);
    tchar_string[n] = _T('\0');
    return tchar_string;
}

//*****************************************************************************

// Function Name:- ReadLongValue

// Author       :- PATNI\MSN

// Purpose      :- Function for changing CPTString to long

//*****************************************************************************
unsigned long CPQMIPCParser::ReadLongValue(
    CPTString string
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCParser::ReadLongValue");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMIPCParser::ReadLongValue"));

    //+Patni-HAR/2009Aug31/Modified/Corrected call for wcstombs()
    int n = string.GetLength() + 1;
    char* char_string = new char[n];
    memset(char_string, 0, sizeof(char) * (n));
    wcstombs(char_string, string, n);
    //char_string[n] = '\0';
    //-Patni-HAR/2009Aug31/Modified/Corrected call for wcstombs()
    unsigned long value = atol(char_string);

    DEL_PTR_ARY(char_string);

    return value;
}

//*****************************************************************************

// Function Name:- ReadLongValue

// Author       :- PATNI\MSN

// Purpose      :- Function for changing CPTString to float

//*****************************************************************************
float CPQMIPCParser::ReadFloatValue(
    CPTString string
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCParser::ReadFloatValue");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMIPCParser::ReadFloatValue"));

    //+Patni-HAR/2009Aug31/Modified/Corrected call for wcstombs()
    int n = string.GetLength() + 1;
    char* char_string = new char[n];
    memset(char_string, 0, sizeof(char) * (n));
    wcstombs(char_string, string, n);
    //char_string[n] = '\0';
    //-Patni-HAR/2009Aug31/Modified/Corrected call for wcstombs()
    float value = atof(char_string);

    DEL_PTR_ARY(char_string);

    return value;
}
