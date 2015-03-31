//Redmine-774
//Redmine-778
//Redmine-780
//Redmine-781
// ScanAnatomyReader.cpp: implementation of the CScanAnatomyReader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScanAnatomyReader.h"
//+Patni-MP/2009Mar17/Added/ACE-1/MCM0213-00131
#include <tami/dicom/anatomy.h>
#include <libStudyManager/strcnv.h> //Tanoue/MVC5849
#include <PQM/PQMLogMgr.h>
#include <PQM/resource.h>

#define ANATOMY_TYPE_MAX		10

using namespace MR::ACQ::SM::COMMON; //Tanoue/MVC5849

//-Patni-MP/2009Mar17/Added/ACE-1/MCM0213-00131
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//********************************Method Header********************************
//Method Name   :CScanAnatomyReader()

//Author        :PATNI/Mangesh

//Purpose       :Constructor
//*****************************************************************************
CScanAnatomyReader::CScanAnatomyReader()
{

}
//********************************Method Header********************************
//Method Name   :~CScanAnatomyReader()

//Author        :PATNI/Mangesh

//Purpose       :Destructor
//*****************************************************************************
CScanAnatomyReader::~CScanAnatomyReader()
{

}

//********************************Method Header********************************
//Method Name   :GetScanAnatomyData()

//Author        :PATNI/Mangesh

//Purpose       :Reads data from config file into list
//*****************************************************************************
bool CScanAnatomyReader::GetScanAnatomyData(VARIANT** f_scan_anatomy_data, const long f_max_saranatomy_limit, const long f_max_scananatomy_limit)
{

    LPCTSTR FUNC_NAME = _T("CScanAnatomyReader::GetScanAnatomyData");

    if (f_scan_anatomy_data == NULL) {	//Patni-MJC/2009Aug27/Modified/cpp test corrections
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_scan_anatomy_data is NULL"));
        return false;
    }

    CPTString l_scan_anatomy_name = L"";
    CPTString l_scan_anatomy_index = L"";

    //+Patni-Hemant/2010Apr26/Added/JaFT/IR-137
    CPTString l_literality = L"";
    //-Patni-Hemant/2010Apr26/Added/JaFT/IR-137

    CScanAnatomyData** l_scananatomy_data = ((CScanAnatomyData**)(*f_scan_anatomy_data)->byref) ;


    const int l_nodecnt = GetNodeCnt();
    //long l_max_saranatomy_limit = ReadMaxSarAnatomyLimitfromFile();
    //long l_max_scananatomy_limit = ReadMaxScanAnatomyLimitfromFile();

    if (f_max_saranatomy_limit < l_nodecnt) {

        //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
        CPQMLogMgr::GetInstance()->DisplayScanError(IDS_MAX_SARANATOMY, _T("IDS_MAX_SARANATOMY"),
                SCAN_ERR_NOTIFY, _T("ScanAnatomyReader.cpp"));

        return false;
    }

    CPTString l_sar_anatomy_name = _T("");
    CPTString l_scan_anatomy_count = _T("");
    int l_sar_seq = 0, l_scananatomy_count = 0, l_currentlistpos = 0;
    int count = 0;
    CString l_temp_str(_T(""));

    for (int l_index = 0; l_index < l_nodecnt; l_index++) {

        l_sar_anatomy_name = GetAttributeValue(l_index,
                                               NODE_TAG_NAME_ANATOMY,
                                               CHILD_TAG_NAME_SAR_ANATOMY,
                                               ATTRIBUTE_NAME_1);

        l_scan_anatomy_count = GetAttributeValue(l_index,
                               NODE_TAG_NAME_ANATOMY,
                               NODE_TAG_NAME_SCAN_ANATOMY_COUNT,
                               ATTRIBUTE_NAME_1);


        l_sar_seq = GetSarAnatomyIndex(l_sar_anatomy_name) - VFC_ANATOMY_ABDOMEN;
        l_scananatomy_count = 0;

        if (l_scan_anatomy_count) {

            l_scananatomy_count = ReadLongValue(l_scan_anatomy_count);
        }

        if (f_max_scananatomy_limit < l_scananatomy_count) {
            //show error message
            //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
            CPQMLogMgr::GetInstance()->DisplayScanError(IDS_MAX_SCANANATOMY, _T("IDS_MAX_SCANANATOMY"),
                    SCAN_ERR_NOTIFY, _T("ScanAnatomyReader.cpp"));

            for (int count = 0; count < l_nodecnt; count++) {
                l_scananatomy_data[count]->RemoveAll();
            }

            return false;
        }

        l_currentlistpos = GetTagPosition(l_index,
                                          NODE_TAG_NAME_ANATOMY,
                                          NODE_TAG_NAME_SCAN_ANATOMY_COUNT,
                                          ATTRIBUTE_NAME_1);

        for (count = 0; count < l_scananatomy_count; count++) {


            l_scan_anatomy_name = GetAttributeValue(NUM_1,
                                                    NODE_TAG_NAME_SCAN_ANATOMY,
                                                    CHILD_TAG_NAME_NAME,
                                                    ATTRIBUTE_NAME_1,
                                                    ++l_currentlistpos);

            l_scan_anatomy_index = GetAttributeValue(NUM_1,
                                   NODE_TAG_NAME_SCAN_ANATOMY,
                                   CHILD_TAG_NAME_VFINDEX,
                                   ATTRIBUTE_NAME_1,
                                   ++l_currentlistpos);

            //+Patni-Hemant/2010Apr26/Added/JaFT/IR-137
            l_literality = GetAttributeValue(NUM_1,
                                             NODE_TAG_NAME_SCAN_ANATOMY,
                                             CHILD_TAG_NAME_LITERALITY,
                                             ATTRIBUTE_NAME_1,
                                             ++l_currentlistpos);
            //+Patni-Hemant/2010Jun11/Added/MaFT/MVC007915

            l_temp_str.Empty();
            UTIL::Pstr2Wcs(&l_temp_str, l_literality);

            if (l_literality.CompareNoCase(_T("")) == 0) {
                --l_currentlistpos ;
            }

            //-Patni-Hemant/2010Jun11/Added/MaFT/MVC007915
            //-Patni-Hemant/2010Apr26/Added/JaFT/IR-137


            if (l_scan_anatomy_index) {
                //+Patni-MP/2009Mar17/Added/ACE-1/MCM0213-00131
                SScanAantomy l_scan_anatomy;
                l_scan_anatomy.m_scan_anatomy_vf = l_sar_seq;
                //-Patni-MP/2009Mar17/Added/ACE-1/MCM0213-00131


                //Tanoue/MVC5849
                UTIL::Pstr2Wcs(&l_scan_anatomy.m_scan_anatomy_name, l_scan_anatomy_name);

                //+Patni-Hemant/2010May20/Modified/MaFT/MVC007915
                l_scan_anatomy.SetLiteralityFlag(l_temp_str.CompareNoCase(_T("ON")) == 0) ;
                //+Patni-Hemant/2010May20/Modified/MaFT/MVC007915

                //-Patni-Hemant/2010Apr26/Added/JaFT/IR-137

                if (l_scan_anatomy.m_scan_anatomy_name.GetLength() > 0) {
                    l_scananatomy_data[l_sar_seq]->AddTail(l_scan_anatomy);
                }
            }

        }//end for

    } //end for


    return true;

}
//********************************Method Header********************************
//Method Name   :GetSarAnatomyIndex()

//Author        :PATNI/Mangesh

//Purpose       :Returns Sar Anatomy index
//*****************************************************************************
int  CScanAnatomyReader::GetSarAnatomyIndex(CPTString f_sar_anatomy
                                           )
{
    int l_ret = -1;
    char* l_scananatomy_name = NULL;//Patni-MJC/2009Aug17/Modified/cpp test corrections

    if (f_sar_anatomy) {
        l_scananatomy_name = ReadStringValue(f_sar_anatomy);
    }

    //+Patni-MP/2009Mar17/Added/ACE-1/MCM0213-00131

    for (int i = 0; i < ANATOMY_TYPE_MAX; i++) {
        if (!strncmp(l_scananatomy_name, anatomyType[i].anatomyStr, strlen(anatomyType[i].anatomyStr))) {
            return anatomyType[i].anatomyNum;
        }

    }

    //-Patni-MP/2009Mar17/Added/ACE-1/MCM0213-00131
    return l_ret;

}





