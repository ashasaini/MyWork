//Redmine-779
//Redmine-778
//Redmine-781
//+Patni-AJS/2010Sep3/Added/ScanCommentDialog/IR-163

// ScanCommentXMLReader.cpp: implementation of the CScanCommentXMLReader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScanCommentXMLReader.h"
#include <PQM/PqmUtilities.h>
#include <PQM/PQMLogMgr.h>
#include <PDataParser/XMLTag.h>
#include <PDataParser/MSXML4DomParser.h>

//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//********************************Method Header********************************
//Method Name   :CScanCommentXMLReader()
//Author        :PATNI/AJS
//Purpose       :
//*****************************************************************************
CScanCommentXMLReader::CScanCommentXMLReader()
{

}

//********************************Method Header********************************
//Method Name   :CScanCommentXMLReader()
//Author        :PATNI/AJS
//Purpose       :
//*****************************************************************************
CScanCommentXMLReader::~CScanCommentXMLReader()
{

}

//********************************Method Header********************************
//Method Name   :CScanCommentXMLReader()
//Author        :PATNI/AJS
//Purpose       :
//*****************************************************************************
bool CScanCommentXMLReader::GetScanCommentXMLData(VARIANT** f_scan_comment_data)
{

    LPCTSTR FUNC_NAME = _T("CScanCommentXMLReader::GetScanCommentXMLData");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("Trace of CScanCommentXMLReader::GetScanCommentXMLData"));

    CPTString filename = CPqmUtilities::GetScanCommentFileName();

    //TA Review Comment_4
    if (filename.IsEmpty()) {
        return false;
    }

    struct _stat statusstruct;

    const int filestatus = _wstat(filename, &statusstruct);

    if (filestatus != 0) {
        return false;
    }

    CMSXML4DomParser* domparser = new CMSXML4DomParser();

    try {
        domparser->LoadFromFile(filename);

    }   catch (...) {

        DEL_PTR(domparser);

        return false;
    }


    CXMLTag* root = (CXMLTag*) domparser->GetRoot();
    const int l_child_count = root->GetChildPSCount();

    //CPTString l_scancomment;
    ScanCommentXMLData* l_scan_comment_data = (ScanCommentXMLData*)(*f_scan_comment_data)->byref;

    const CString l_seq_lable_id = _T("SEQ_LabelID");
    const CString l_cont_labelid = _T("CONT_LabelID");
    const CString l_opt_labelid = _T("OPT_LabelID");

    bool l_return_val = true;

    for (int i = 0 ; (l_return_val && (i < l_child_count)) ; i++) {

        // get the child tag
        CXMLTag* l_child = (CXMLTag*) root->GetChildPS(i);

        if (l_child) {

            const CPTString l_child_name = l_child->GetName() ;

            if (l_child_name == l_seq_lable_id) {
                if (!IterateChild(l_child, &(l_scan_comment_data->m_seq_comment_list))) {
                    l_return_val = false;
                }

            } else if (l_child_name == l_cont_labelid) {
                if (!IterateChild(l_child, &(l_scan_comment_data->m_cont_comment_list))) {
                    l_return_val = false;
                }

            } else if (l_child_name == l_opt_labelid) {
                if (!IterateChild(l_child, &(l_scan_comment_data->m_opt_comment_list))) {
                    l_return_val = false;
                }
            }
        }
    }

    DEL_PTR(domparser);

    return l_return_val;
}

//********************************Method Header********************************
//Method Name   :CScanCommentXMLReader()
//Author        :PATNI/AJS
//Purpose       :
//*****************************************************************************
bool CScanCommentXMLReader::IterateChild(
    CXMLTag* f_child,
    CList<CString, CString>* f_list
)
{
    if (!f_child || !f_list) {
        return false;
    }

    const CPTString l_lable_tag = _T("LABEL");

    const CPTString l_def_tag = _T("DEFVAL");

    CPTString l_attribname1, l_attribute_val1, l_attribname2, l_attribute_val2 ;

    // get the first attribute from right
    CXMLTag::NV_POS l_pos = f_child->GetFirstAttribute(l_attribname2, l_attribute_val2);

    CXMLTag::NV_POS l_validate = NULL;

    if (l_pos == l_validate) {
        return false;
    }

    if (l_attribname2 != l_def_tag) {
        return false;
    }


    l_pos = f_child->GetNextAttribute(l_pos, l_attribname1, l_attribute_val1);

    if ((l_attribname1 != l_lable_tag) || (l_attribute_val1.IsEmpty())) {
        return false;
    }

    CString l_lable_str = ReadStringValue(l_attribute_val1);

    CString l_multi_ling_lable_str = CPqmUtilities::GetMultiLingualString(l_lable_str, false) ;

    if (l_multi_ling_lable_str.IsEmpty()) {

        if (l_attribute_val2.IsEmpty()) {
            l_multi_ling_lable_str = l_lable_str;

        } else {
            l_multi_ling_lable_str = ReadStringValue(l_attribute_val2) ;
        }
    }

    f_list->AddTail(l_multi_ling_lable_str);

    return true;
}