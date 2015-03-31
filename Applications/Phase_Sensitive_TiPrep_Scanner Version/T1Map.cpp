// T1Map.cpp: implementation of the CT1Map class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#include "stdafx.h"
#include "T1Map.h"
#include "TiPrepLogHelper.h"
#include "TiPrepTrace.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*************************************************************
* Method:    	CT1Map
* Description: 	This function is used 
* Returns:   	
*************************************************************/
CT1Map::CT1Map():
m_map(NULL),
m_mapsize( 0),
m_width(0),
m_height(0)
{
}

/*************************************************************
* Method:    	~CT1Map
* Description: 	This function is used 
* Returns:   	
*************************************************************/
CT1Map::~CT1Map()
{
    if(m_map) {
        delete [] m_map;
        m_map = NULL;
    }
}

/*************************************************************
* Method:    	SetMap
* Description: 	This function is used 
* Parameter: 	byte * roi_map :
* Parameter: 	int height :
* Parameter: 	int width :
* Returns:   	int
*************************************************************/
int CT1Map::SetMap(byte *roi_map, int height, int width)
{
    CString l_cserrmsg(_T(""));
    TIPREP_TRACE(CT1Map::SetMap,MRTraceMarker8);
    
    try {
        if(roi_map == NULL) {
            l_cserrmsg.Format(_T("SetMap() input buffer roi_map is NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
            return E_ERROR_NULL_FUNCPARAMS;
        }
        
        if(height <= 0 || width <= 0) {
            l_cserrmsg.Format(_T("SetMap() height / width are 0"));
            CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
            return E_ERROR;
        }
        
        m_height = height;  
        m_width = width;    
        
        if(m_map) {
            delete [] m_map;
            m_map = NULL;
        }
        
        int counter = 0;
        m_map = new long[height * width];
        
        if(m_map == NULL) {
            l_cserrmsg.Format(_T("SetMap() Memory allocation failed"));
            CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
            return E_MEM_ALLOC;
        }
        
        int i = 0;
        int j = 0;
        
        for(i = 0; i < height; i++) {
            for(j = 0; j < width; j++) {
                m_map[i*width + j] = -1;
                
                if(1 == roi_map[i*width+j]) {
                    m_map[i*width + j] = counter;
                    counter++;
                }
            }
        }
        
        m_mapsize = height * width;
    } catch(...) {
        l_cserrmsg.Format(_T("SetMap() Exception occurred"));
        CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
        return E_ERR_EXCEPTION;
    }
    
    return E_NO_ERROR;
}

/*************************************************************
* Method:    	GetMap
* Description: 	This function is used 
* Returns:   	long *
*************************************************************/
const long * CT1Map::GetMap()
{
    TIPREP_TRACE(CT1Map::GetMap,MRTraceMarker8);
    return m_map;
}

/*************************************************************
* Method:    	CreateT1Map
* Description: 	This function is used 
* Parameter: 	double * t1val :
* Parameter: 	CString & f_filename :
* Returns:   	int
*************************************************************/
int CT1Map::CreateT1Map(double * t1val, CString &f_filename)
{
    double * tmpMap = NULL;
    TIPREP_TRACE(CT1Map::CreateT1Map,MRTraceMarker3);
    
    try {
        if(m_mapsize <= 0 || m_map == NULL) {
            CString l_cserrmsg(_T(""));
            l_cserrmsg.Format(_T("CreateT1Map() Error: map not initialized"));
            CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
            return E_ERROR;
        }
        
        if(t1val == NULL) {
            CString l_cserrmsg(_T(""));
            l_cserrmsg.Format(_T("CreateT1Map() Error: Input Parameter is NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
            return E_ERROR_NULL_FUNCPARAMS;
        }
        
        tmpMap = new double[m_mapsize];
        
        if(tmpMap == NULL) {
            CString l_cserrmsg(_T(""));
            l_cserrmsg.Format(_T("CreateT1Map() Memory allocation failed"));
            CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
            return E_MEM_ALLOC;
        }
        
        int index = 0;
        
        for(int i = 0; i < m_mapsize; i++) {
            tmpMap[i] = -1;
            
            if(m_map[i] != -1) {
                index = m_map[i];
                tmpMap[i] = t1val[index];
            }
        }
        
        CMapWriter l_CmapWriter;
        int l_nstatus = E_NO_ERROR;
        l_nstatus = l_CmapWriter.WriteToFile(f_filename, (void *)tmpMap, m_mapsize * sizeof(double));
        if(E_NO_ERROR != l_nstatus){
            CString lcsmsg = _T("CreateT1Map(): WriteToFile() returned error");
            CTiPrepLogHelper::WriteToErrorLog(lcsmsg, __LINE__, __FILE__);
            
            if(NULL != tmpMap) {
                delete [] tmpMap;
                tmpMap = NULL;
            }
            
            return(l_nstatus);		
            
        }
        if(NULL != tmpMap) {
            delete [] tmpMap;
            tmpMap = NULL;
        }
    } catch(...) {
        CString l_cserrmsg(_T(""));
        l_cserrmsg.Format(_T("CreateT1Map() Exception occurred"));
        CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
        return E_ERR_EXCEPTION;
    }
    
    return E_NO_ERROR;
}

/*************************************************************
* Method:    	FindSelectedIndex
* Description: 	This function is used 
* Parameter: 	int x :
* Parameter: 	int y :
* Parameter: 	int * f_nSelectedIndex :
* Returns:   	int
*************************************************************/
int CT1Map::FindSelectedIndex(int x, int y, int *f_nSelectedIndex)
{
    int l_nSelectedIndex = 0;
    TIPREP_TRACE(CT1Map::FindSelectedIndex,MRTraceMarker3);
    try{
        
        if((x < 0) || (y < 0) || (f_nSelectedIndex == NULL)) {
            CString l_cserrmsg(_T(""));
            l_cserrmsg.Format(_T("FindSelectedIndex() Input parameters NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
            return(E_ERROR);
        }
        
        if((m_width <= 0) || (m_map == NULL)) {
            CString l_cserrmsg(_T(""));
            l_cserrmsg.Format(_T("FindSelectedIndex() map not set yet"));
            CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
            return(E_ERROR);
        }
        
        l_nSelectedIndex = (m_width * y) + x;
        
        if((l_nSelectedIndex) < 0 || (l_nSelectedIndex > m_mapsize)) {
            CString l_cserrmsg(_T(""));
            l_cserrmsg.Format(_T("FindSelectedIndex() Unable to find index within map"));
            CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
            return(E_ERROR);
        }
        
        *(f_nSelectedIndex) = m_map[l_nSelectedIndex];
    }catch(...){
        CString l_cserrmsg(_T(""));
        l_cserrmsg.Format(_T("FindSelectedIndex() Exception occurred"));
        CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
        return E_ERR_EXCEPTION;
    }
    return(E_NO_ERROR);
}   

/*************************************************************
* Method:    	GetWidhtForT1Map
* Description: 	This function is used 
* Returns:   	int
*************************************************************/
int CT1Map::GetWidhtForT1Map()
{
    TIPREP_TRACE(CT1Map::GetWidhtForT1Map,MRTraceMarker8);
    return(m_width);
}

/*************************************************************
* Method:    	GetHeightForT1Map
* Description: 	This function is used 
* Returns:   	int
*************************************************************/
int CT1Map::GetHeightForT1Map()
{
    TIPREP_TRACE(CT1Map::GetHeightForT1Map,MRTraceMarker8);
    return(m_height);
}

/*************************************************************
* Method:    	GetMouseXY
* Description: 	This function is used 
* Parameter: 	int f_nidx :
* Parameter: 	int & mx :
* Parameter: 	int & my :
* Returns:   	int
*************************************************************/
int CT1Map::GetMouseXY(int f_nidx, int &mx, int &my)
{
    mx = 0;
    my = 0;
    TIPREP_TRACE(CT1Map::GetMouseXY,MRTraceMarker3);
    
    if(m_map == NULL) {
        CString l_cserrmsg(_T("GetMouseXY() m_map is NULL"));
        CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
        return E_ERROR;
    }
    
    if(f_nidx != -1) {
        for(int i = 0; i < m_height; i++) {
            for(int j = 0; j < m_width; j++) {
                if(m_map[i*m_width + j] == f_nidx) {
                    mx = j;
                    my = i;
                    break;
                }
            }
        }
    }
    
    return(E_NO_ERROR);
}

/*************************************************************
* Method:    	Copy Constructor
* Description: 	Copies data
* Parameter: 	CT1Map f_map :
* Returns:   	NA
*************************************************************/
CT1Map::CT1Map(CT1Map& f_map)
{
	/*TIPREP_TRACE(CT1Map::CT1Map);*/
	
    if (f_map.m_width > 0 && f_map.m_height > 0)
    {
        if (f_map.m_map)
        {
            if(NULL != m_map){
                delete m_map;
                m_map = NULL;
            }

            m_map = new long[m_width * m_height];

            if(m_map == NULL) 
            {
                CString l_cserrmsg(_T("CT1Map() m_map is NULL"));
                CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
            }
        }

        m_width = f_map.m_width;
        m_height = f_map.m_height;
        m_mapsize = f_map.m_mapsize;
    }
}

/*************************************************************
* Method:    	Overloaded assignment operator
* Description: 	Copies data
* Parameter: 	CT1Map f_map
* Returns:   	CT1Map &
*************************************************************/
CT1Map& CT1Map::operator = (CT1Map& f_map)
{
	/*TIPREP_TRACE(CT1Map::operator=);*/
	
    if(this == &f_map) {
        return(*this);
    }

    if (f_map.m_width > 0 && f_map.m_height > 0)
    {
        if (f_map.m_map)
        {
            
            if(NULL != m_map){
                delete m_map;
                m_map = NULL;
            }
            
            m_map = new long[m_width * m_height];

            if(m_map == NULL) 
            {
                CString l_cserrmsg(_T("operator =() m_map is NULL"));
                CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
            }
        }

        m_width = f_map.m_width;
        m_height = f_map.m_height;
        m_mapsize = f_map.m_mapsize;
    }

	return(*this);
}

///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////