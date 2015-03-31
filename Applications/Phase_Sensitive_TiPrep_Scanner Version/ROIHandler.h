// ROIHandler.h: interface for the CROIHandler class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#if !defined(AFX_ROIHANDLER_H__E5FE0201_5CEA_4170_B508_3FCDD1A2827C__INCLUDED_)
#define AFX_ROIHANDLER_H__E5FE0201_5CEA_4170_B508_3FCDD1A2827C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MamROIManager.h"
#include "CosVis\custom\Component\CosVis.h"
#include <afxtempl.h>
#include "ROIReader.h"

/**
*  This class is a used to draw ROI and  to handle ROI operations.
*/
#define AVGROIFILE _T("c:/MrMplus/Data/TiPrep/AVGROIInfo.txt")
#define MAPROIFILE _T("c:/MrMplus/Data/TiPrep/MAPROIInfo.txt")
struct CurVePtInfo{
    CurvePoint_t CurvePts[2];
};
class CROIHandler
{
public:
    virtual ~CROIHandler();
private:
/**
*  This variable is used initialize CosVis channel.
    */
    ICosVis* m_ppCosVisObject;
    /**
    *  This variable is used as control manager for ROI handling.
    */
    IMamROIControlHandlerManager* m_ppMamROIControlHandlerManager;
    /**
    *  This variable is used to store path for communication.
    */
    CStringArray m_opPathList;
    /**
    *  This variable stores GL name for communication.
    */
    CString m_csGLname;
    /**
    *  This structure is used to store UID for ROI.
    */
    typedef struct Uid_Point {
        CString uid;
        float x;
        float y;
        float radius; //for elliptical ROI its height
        float width;
    } Uid_Point;
    /**
    *  This variable is used to store number of ROI drawn on image.
    */
    int m_Roi_count;
    /**
    *  This variable is used to store UID for each ROI.
    */
    CArray<Uid_Point, Uid_Point> m_uid_point_list;
    /**
    *  This variable stores Operation path for ROI creation.
    */
    CString m_csOperationPath;  //operation path
    vector<CCircleROIParams> m_vRoiparam;
    vector<CEllipticalROIParams> m_vEllipseRoiparam;
    CROIReader m_RoiParamabj;
    CurVePtInfo m_curveptinfo[MAX_LINE_ROI];
    ROIObject_t m_RoiObject[MAX_LINE_ROI];
    
private:
    HRESULT GetSpecifiedScreenAllPath(
        BSTR f_bsScreenName,
        int* f_npPathCount,
        BSTR** f_ppSpecifiedPathList);
    void ReleaseVectorList(CurvePoint_t* f_pVectorList);
    HRESULT CreateROIObject(ROIObject_t* f_stROIObject, int f_nRoiNumber);
    int CalCircleCordination(float &f_x1, float &f_y1, float &f_x2, float &f_y2, float &f_radius, int f_nroi_no);
    void GetROICoordinatesFromFile();
    void ReleaseUIDList(BSTR* f_pUIDList, int f_nCount);
    CROIHandler();
    static CROIHandler* m_roi_ptr;
    int SaveDrawROIPosition(BSTR f_UID, BSTR f_PATH);
    BOOL SaveNewROIPosition(BSTR f_UID, float f_csCenterX, float f_csCenterY, float f_csRadius, float f_fWidth);
    int CalEllipseCordination(float &x1,
        float &y1,
        float &x2,
        float &y2,
        float &f_fHeight,
        float &f_fWidth,
        int f_nroi_no);
	CString m_protocol_name;
    
public:
    BOOL InitializeCosVisChannel(CString f_parentname);
    HRESULT InitializeObserver(CString f_channel_name);
    BOOL DrawROIComplete(CString f_csSender, CString f_csCommand, CString f_csParam, _bstr_t f_custom);
    BOOL AutoSaveDB(BOOL f_flag);
    BOOL DrawROI(int f_nRoiNumber, BOOL f_show = FALSE);
    BOOL MouseMoveROI(BOOL f_bCheckFlag);
    static CROIHandler* GetInstance();
    void DeleteInstance();
    BOOL GetROIPosition(CString *f_csMoveCenterX, CString *f_csMoveCenterY, CString *f_csMoveRadius, CString *f_csMoveWidth);
    BOOL ResetROIPosition(CString cs,int f_nImageHt,int f_nImageWt);
    BOOL SaveMoveROI(CString f_csMoveCenterX[], CString f_csMoveCenterY[], CString f_csMoveRadius[], CString f_csMoveWidth[]);
    BOOL DeleteROI();
    BOOL HideROI(int f_nRoiNumber, BOOL f_flag);
    BOOL GetROIBufferInfo(byte* l_csImROI[], int *l_nROIH, int *l_nROIW);
    BOOL GetsaveROIParams(CString &f_csProtocolName, CString cs,int ,int);
    BOOL SaveUpdateROIParamsInFile(CString &f_csProtocolName, CString &cs);
    BOOL SetRoiPosition();
	BOOL SetProtocolNameForSetROIParams(CString &f_protocol_name); 
	CString GetProtocolNameForSetROIParams();	
	BOOL ResetROIPos(CString cs);
	BOOL RedrawROI(bool isreset,  CString &protocol,CString &csmode);
    BOOL SetROIPos(CString f_Roi_cenX[], CString f_Roi_cenY[], CString f_Roi_height[], CString f_Roi_Width[]);
    
};

#endif // !defined(AFX_ROIHANDLER_H__E5FE0201_5CEA_4170_B508_3FCDD1A2827C__INCLUDED_)



