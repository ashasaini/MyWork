// REadWriteFile.cpp: implementation of the REadWriteFile class.
//
/*****************************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: PlanDuplicator
    Author: PATNI-PlanDuplicator Team
*****************************************************************************************/

#include "stdafx.h"
#include "PlanDuplicator.h"
#include "PDDataFileReader.h"
#include "dataTypes.h"
#include "SmartMPRData.h"
#include "SmartVOIData.h"
#include "PDTrace.h"
#include "PDLogHelper.h"

#include <WinNT.H>
const int NUM_LINES_MPR_DATA = 5; //Mpower 030/2012-06-26/AS/Modified/cpp change
const int NUM_LINES_VOI_DATA = 3; //Mpower 030/2012-06-26/AS/Modified/cpp change
const int MAX_CHARS_IN_LINE  = 256; //Mpower 030/2012-06-26/AS/Modified/cpp change // changed value from 1000 to 256
const int NUM_LIMIT_3  = 3;
const int NUM_VECTORS_VOI = 2;
const int NUM_PARAM_FOV = 2;
const int NUM_PARAM_LOCATOR = 1;
#define TrimString(str) str.TrimLeft();str.TrimRight(); //AN
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//****************************Method Header************************************

//Method Name   :ReadWriteFile

//Author        :PATNI

//Purpose       :Constructor
//*****************************************************************************

CPDDataFileReader::CPDDataFileReader():m_error_code(0)
{
    PD_TRACE(CPDDataFileReader::CPDDataFileReader);
}

//****************************Method Header************************************

//Method Name   :ReadWriteFile

//Author        :PATNI

//Purpose       :Destructor
//*****************************************************************************
CPDDataFileReader::~CPDDataFileReader()
{
    PD_TRACE(CPDDataFileReader::~CPDDataFileReader);
}

//****************************Method Header************************************

//Method Name   :ReadVarfieldDataFile

//Author        :PATNI

//Purpose       :Read data from file and store in container classes
//*****************************************************************************
int CPDDataFileReader:: ReadVarfieldDataFile(const char* f_csfilename,
                                             vector<CPDCoordinates*> & f_VantageDataVector,
                                             PDAppMode_t f_app_mode)
{
    PD_TRACE(CPDDataFileReader::ReadVarfieldDataFile);
    
    // char l_line[MAX_CHARS_IN_LINE]={0};
    //AN
    CStdioFile file;
    CString buf (L"");
    int l_nbuf_length=0;
    CString file_name = f_csfilename;
    //AN
    float l_data[NUM_LIMIT_3]={0};
    f32vec3_t l_data_vec={0,0,0};
    int l_varfieldtype = 0;
    int l_i = 0;//sb/revw.. have meaningfull name.
    int l_dataRead = 0;
    int l_newData = 0;
    int l_error = 0;
    int l_checkFile = 0;
    char *l_token = NULL;
    //CString l_token (_T(""));
    bool l_isScout = FALSE;
    f32vec2_t l_fFov = {0.0,0.0};
    
    if (0 < f_VantageDataVector.size()){
        f_VantageDataVector.clear();
    }else{
        //dummy
    }
    DWORD dwAttrs; 
    CString filename = f_csfilename;
    dwAttrs = GetFileAttributes(filename);
    if ( INVALID_FILE_ATTRIBUTES == dwAttrs ){
        m_error_code = PD_DATA_FILE_OPEN;
        CString l_csErrMsg = ErrorList[m_error_code];
        CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__,CUSTOM_ERRORDIALOG_DISPLAY);
        return(E_ERROR);
    }
    
    /*  fstream file_io;
    file_io.open(f_csfilename,ios::in);      
    if(file_io.fail()){
    m_error_code = PD_DATA_FILE_OPEN;
    CString l_csErrMsg = ErrorList[m_error_code];
    CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__,CUSTOM_ERRORDIALOG_DISPLAY);
    return(E_ERROR);
    }else{
    //dummy
    }
    */
    if (!file.Open(file_name, CFile::modeRead)) {
        m_error_code = PD_DATA_FILE_OPEN;
        CString l_csErrMsg = ErrorList[m_error_code];
        CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__,CUSTOM_ERRORDIALOG_DISPLAY);
        return(E_ERROR);
    }else{
        //dummy
    }
    
    int l_nnumdata = 0;
    //char * l_csplane_name = NULL;
    CString l_csplane_name = (_T("")); //AN
    CPDCoordinates *l_spObj = NULL;
    int l_nplanename_size = 0;
    
    if ( f_app_mode == SMART_MPR ) {
        l_spObj =   new CSmartMPRData();      
        if(NULL != l_spObj)				//Mpower 040/2012-08-24/AS/Added
        {
            l_csplane_name = _T("SlicePlane");
            //l_nplanename_size = (strlen("SlicePlane")) + 1;         //11 changed after CPP Test Change
            l_nnumdata = NUM_LINES_MPR_DATA;
        }else{
            CString l_cserrmsg = ErrorList[PD_MEMORY_ALLOCATION];
            CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
            return(E_ERROR);
        }
    } else if ( f_app_mode == SMART_VOI ){
        l_spObj = new CSmartVOIData();      
        if(NULL != l_spObj)				//Mpower 040/2012-08-24/AS/Added
        {
            l_csplane_name = _T("VOI"); 
            l_nnumdata = NUM_LINES_VOI_DATA;
            // l_nplanename_size = (strlen("VOI")) + 1;   //4 changed after CPP Test Change
        }else{
            CString l_cserrmsg =  ErrorList[PD_MEMORY_ALLOCATION];
            CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
            return(E_ERROR);
        }
    } else {
        CString l_cserr (_T("Invalid mode selection for VOI or MPR"));
        CPDLogHelper::WriteToErrorLog(l_cserr,__LINE__);
        return(E_ERROR);
    }
    
    /* while(1) {
    file_io.getline(l_line,MAX_CHARS_IN_LINE);
    
      if ( (file_io.eof()) || (file_io.fail()) ){
      break;
      }
      
        if ('#' == l_line[0]){
        continue;
        }
    */
    //AN
    while (file.ReadString(buf)) {
        
        TrimString(buf);
        if (buf.IsEmpty() || (buf.GetAt(0) == L'#')) {
            continue;
        } CString l_csPDPlane (_T(""));//AN
        //AN
        //   if (0 < strstr(l_line,l_csplane_name) ){ //CPP Test Change
        if (buf.Find(l_csplane_name) != -1) { //AN Added
            
            l_checkFile = 1;
            l_newData = 1;
            l_dataRead++;
            l_varfieldtype = 0;
            // l_spObj->setPDPlan(l_line + l_nplanename_size);
            //AN added
            int pos = -1;
            TrimString(buf);
            pos = l_csplane_name.GetLength() + 1;
            l_csPDPlane = buf.Mid(pos);
            TrimString(l_csPDPlane);
                                    
            l_spObj->setPDPlan(l_csPDPlane);
            //AN added
            // if (0 == strcmp(l_spObj->getPDPlan(), "")){       
            if (l_spObj->getPDPlan().IsEmpty()){   //AN Added 
                l_error = 1;
                break;
            }
            
            l_varfieldtype = 0;
            
            //   if (0 < strstr(l_spObj->getPDPlan(), SLICE_PLANE_SCOUT2)) { //CPP Test change
            if (0 <= l_spObj->getPDPlan().Find(_T(SLICE_PLANE_SCOUT2))) { //AN Added
                l_isScout = TRUE;
            }
            continue;           
        }
        
        if (0 < l_newData){
            l_i = 0;
            l_varfieldtype++;     
           
            //CString Str = buf;
            
            if ((buf.ReverseFind(',')+1)==(buf.GetLength())){
                l_error=1;
                break;
            }
            l_nbuf_length = buf.GetLength();
            char * l_str = new char[l_nbuf_length+1];
            wcstombs(l_str,buf,(l_nbuf_length+1));    
            l_token = strtok(l_str,",");
            
            while ( NULL != l_token ){
                //AN // Validation for l_i
                if (NUM_LIMIT_3 <= l_i)  //CPP Test change
                {
                    CString l_cserrmsg (_T("Invalid Format"));
                    CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                    l_error = 1;
                    break;
                }
                l_data[l_i] = (float)atof(l_token);
                
                //AN //end
                l_token = strtok (NULL, ",");               
                l_i++;              
            }
             //AN // Added
            if(NULL != l_str){
                delete[] l_str;
                l_str = NULL;
            }
            if (1 == l_error)
            {
                break;
            }
            //AN // end
            int l_non_3_valued = 0;
            if (SMART_MPR == f_app_mode)
            {
                l_non_3_valued = l_nnumdata -1;
            }
            else
            {
                l_non_3_valued = l_nnumdata ;
            }
            if(l_varfieldtype != l_non_3_valued)
            {
                if (NUM_LIMIT_3 != l_i) { //CPP Test change
                    l_error = 1;   //DICOM DATA FILE is Corrupted.
                    break;
                }else{
                    l_data_vec.x = l_data[0];
                    l_data_vec.y = l_data[1];
                    l_data_vec.z = l_data[2];
                    if ((SMART_MPR == f_app_mode) && (l_varfieldtype == l_nnumdata))
                    {
                        int l_nslice_num = (int)l_data_vec.z;
                        l_spObj->SetSliceAttributes(SLICE_THICKNESS,(char*)&l_data_vec.x); 
                        l_spObj->SetSliceAttributes(SLICE_GAP,(char*)&l_data_vec.y); 
                        l_spObj->SetSliceAttributes(SLICE_NUM,(char*)&l_nslice_num); 
                    }
                    l_spObj->SetSliceVectorinfo((PDData_t)l_varfieldtype,l_data_vec); 
                    
                     if (TRUE == l_isScout)
                    {
                        l_data_vec.x = 0.0;
                        l_data_vec.y = 0.0;
                        l_data_vec.z = 0.0;
                        
                        l_spObj->SetSliceVectorinfo((PDData_t)(l_varfieldtype+1),l_data_vec);   
                    }
                }
            }else{
                if(SMART_MPR == f_app_mode)
                {
                    if (NUM_PARAM_FOV != l_i) {  //CPP Test Tool change
                        l_error = 1;   //DICOM DATA FILE is Corrupted.
                        break;
                    }else{
                        l_fFov.x = l_data[0];
                        l_fFov.y = l_data[1];
                        l_spObj->SetSliceAttributes(FOV,(char*)&l_fFov);
                        
                    }
                }
                else
                {
                    if (NUM_PARAM_LOCATOR != l_i) {    //CPP Test Tool change
                        l_error = 1;   //DICOM DATA FILE is Corrupted.
                        break;
                    }else{
                        l_fFov.x = l_data[0];
                        l_fFov.y = l_data[0];
                        l_spObj->SetSliceAttributes(FOV,(char*)&l_fFov);
                        
                    }
                }
            }   
        }

        if ((l_nnumdata == l_varfieldtype) || ((TRUE == l_isScout) && (l_varfieldtype == 1))){ 
            l_newData = 0;          
            f_VantageDataVector.push_back(l_spObj);
            
            if ( f_app_mode == SMART_MPR ) {
                l_spObj =(CPDCoordinates *)new CSmartMPRData();  // null check
                if (NULL == l_spObj)
                {
                    CString l_cserrmsg (_T("Memory allocation error in ReadVarfieldDataFile"));
                    CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                    return(E_ERROR);
                }
                
            } else {
                l_spObj =(CPDCoordinates *)new CSmartVOIData(); // null check
                if (NULL == l_spObj)
                {
                    CString l_cserrmsg (_T("Memory allocation error in ReadVarfieldDataFile"));
                    CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                    return(E_ERROR);
                }
                
            }
            
            l_isScout = FALSE;
        }
    }   
    
    unsigned int l_size = f_VantageDataVector.size();
    if (l_dataRead != (int)l_size){
        l_error=0;
        m_error_code = PD_DATA_FILE_INVALID;
        l_dataRead=0;
        CString l_csErrMsg = ErrorList[m_error_code];
        CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__,CUSTOM_ERRORDIALOG_DISPLAY);
        return(E_ERROR);
    }
    
    if ((0 != l_error) || (0 == l_checkFile)){
        l_error = 0;  
        m_error_code = PD_DATA_FILE_INVALID;
        CString l_csErrMsg = ErrorList[m_error_code];
        CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__,CUSTOM_ERRORDIALOG_DISPLAY);
        return(E_ERROR);
    }
    
    l_dataRead=0;
    file.Close();
    //file_io.close();
    
    return(E_NO_ERROR);
}

//****************************Method Header************************************

//Method Name   :getErrorCode

//Author        :PATNI

//Purpose       :returns value stored in m_error_code.
//*****************************************************************************
int CPDDataFileReader::getErrorCode(void)
{
    PD_TRACE(CPDDataFileReader::getErrorCode);
    return(m_error_code);
}

////////////////////////////////    End of File     /////////////////////////////