/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#include "stdafx.h"
#include "TiPrep.h"
#include "TiGraphCtrl.h"
#include "TiPrepView.h" 


#include "TiPrepTrace.h"
#include "TiPrepLogHelper.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//Text color.
const COLORREF g_text_color = RGB(200, 200, 200);

//Graph line color.
const COLORREF g_graph_line_color = RGB(255, 255, 255);

//ROI4 graph drawing color.
const COLORREF Roi4_curve_marking_color = RGB(255, 51, 255);//Pink

//ROI5 graph drawing color.
const COLORREF Roi5_curve_marking_color = RGB(51, 51, 255);//Blue

//ROI3 graph drawing color.
const COLORREF Roi3_curve_marking_color = RGB(50, 180, 100);//Green

//ROI1 graph drawing color.
const COLORREF Roi1_curve_marking_color = RGB(247, 241, 43);//Yellow

//ROI2 graph drawing color.
const COLORREF Roi2_curve_marking_color = RGB(208, 26, 21);//Red

//Window parameters
ToolTiPrepWindow CTiGraphCtrl::TiPrepWindow = {0, 0, NULL, 0, 0, 0, 0};

//Margin for Y axis.
#define GRAPH_MARGIN_Y 50
/**
*   Margin for X axis from the top.
*/
#define GRAPH_MARGIN_X_TOP  30
/**
*   Margin for X axis from the bottom.
*/
#define GRAPH_MARGIN_X_BOTTOM 30
/**
*   Marker height on the axis.
*/
#define MARKER_LENGTH 6
/**
*   Total no of divisions on axis.
*/
#define NUM_DIVISION 10
/**
*   Conversion factor for scaling.
*/
#define CONVERT_FACT 10

/**
*   A constructor.
*   This function is used for initialization of class variables.
*/
CTiGraphCtrl::CTiGraphCtrl() : m_wndFlag(FALSE),
m_Xmax(0),
m_Xmin(0),
m_Ymax(0),
m_Ymin(0),
m_dRoi_Org_Data(NULL),
m_dRoi_Inv_Data(NULL),
m_dRoi_Fit_Data(NULL),
m_dRoi_Fit_Rev_Data(NULL),
m_dRoi_Inv_Rev_Data(NULL),
m_dRoi_Ti_Data(NULL),
m_dRoi_T1_Data(NULL),
m_dRoi_MinRev_Ti_Data(NULL),
m_dRoi_MinRev_T1_Data(NULL),
m_csMeanTiValue(L""),
m_dNoise_ROI_Org_Data(NULL),    
m_time_data(NULL) , 
m_t1_map_check_box_status(0) ,
m_DynamicGraphMarginY(GRAPH_MARGIN_Y),
m_nselectImg(0),
m_nNumOfPoints(0),
m_nselectImgNew(0)
{
	l_mybrush.m_hObject = NULL; //AN
    memset(m_ROI,FALSE,sizeof(BOOL)*MAX_NUM_ROI);
    memset(m_NoiseROI,FALSE,sizeof(BOOL)*MAX_NOISE_ROI);
    memset(m_ROIRev,FALSE,sizeof(BOOL)*MAX_NUM_ROI);
    memset(m_graphOpt,FALSE,sizeof(BOOL)*MAX_NUM_ROI);
}

CTiGraphCtrl& CTiGraphCtrl::operator = (CTiGraphCtrl& f_GraphCtrl_ptr)
{
    /*TIPREP_TRACE(CTiGraphCtrl::operator);*/
    
    if(this == &f_GraphCtrl_ptr) {
        return(*this);
    }
        m_wndFlag = f_GraphCtrl_ptr.m_wndFlag;
        m_Xmax = f_GraphCtrl_ptr.m_Xmax;
        m_Xmin= f_GraphCtrl_ptr.m_Xmin;
        m_Ymax= f_GraphCtrl_ptr.m_Ymax;
        m_Ymin= f_GraphCtrl_ptr.m_Ymin;
        m_dRoi_Org_Data= f_GraphCtrl_ptr.m_dRoi_Org_Data;
        m_dRoi_Inv_Data= f_GraphCtrl_ptr.m_dRoi_Inv_Data;
        m_dRoi_Fit_Data= f_GraphCtrl_ptr.m_dRoi_Fit_Data;
        m_dRoi_Fit_Rev_Data= f_GraphCtrl_ptr.m_dRoi_Fit_Rev_Data;
        m_dRoi_Inv_Rev_Data= f_GraphCtrl_ptr.m_dRoi_Inv_Rev_Data;
        m_dRoi_Ti_Data= f_GraphCtrl_ptr.m_dRoi_Ti_Data;
        m_dRoi_T1_Data= f_GraphCtrl_ptr.m_dRoi_T1_Data;
        m_dRoi_MinRev_Ti_Data= f_GraphCtrl_ptr.m_dRoi_MinRev_Ti_Data;
        m_dRoi_MinRev_T1_Data= f_GraphCtrl_ptr.m_dRoi_MinRev_T1_Data;
        m_csMeanTiValue= f_GraphCtrl_ptr.m_csMeanTiValue;
        m_dNoise_ROI_Org_Data= f_GraphCtrl_ptr.m_dNoise_ROI_Org_Data;   
        m_time_data= f_GraphCtrl_ptr.m_time_data; 
        m_t1_map_check_box_status= f_GraphCtrl_ptr.m_t1_map_check_box_status;
        m_DynamicGraphMarginY= f_GraphCtrl_ptr.m_DynamicGraphMarginY;
        m_nselectImg = f_GraphCtrl_ptr.m_nselectImg;
        m_nNumOfPoints= f_GraphCtrl_ptr.m_nNumOfPoints;
        m_nselectImgNew= f_GraphCtrl_ptr.m_nselectImgNew;
        memcpy(m_ROI,f_GraphCtrl_ptr.m_ROI,sizeof(BOOL)*MAX_NUM_ROI);
        memcpy(m_NoiseROI,f_GraphCtrl_ptr.m_NoiseROI,sizeof(BOOL)*MAX_NOISE_ROI);
        memcpy(m_ROIRev,f_GraphCtrl_ptr.m_ROIRev,sizeof(BOOL)*MAX_NUM_ROI);
        memcpy(m_graphOpt,f_GraphCtrl_ptr.m_graphOpt,sizeof(BOOL)*MAX_NUM_ROI);

    return (*this);
}

/**
*   A destructor.
*   This function is used for deleting the memory if used.
*/
CTiGraphCtrl::~CTiGraphCtrl()
{
    //TIPREP_TRACE(CTiGraphCtrl::~CTiGraphCtrl);
    //int l_ni = 0;
	try{
		l_mybrush.DeleteObject();
	}catch(...)
	{
		CString l_csmsg(_T("Exception occurred in CTiGraphCtrl"));
        CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
		
	}

}


BEGIN_MESSAGE_MAP(CTiGraphCtrl, CWnd)
//{{AFX_MSG_MAP(TiGraphCtrl)
// NOTE - the ClassWizard will add and remove mapping macros here.
ON_WM_PAINT()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// TiGraphCtrl message handlers


/*************************************************************
* Method:       Create
* Description:  This function is used
* Parameter:    LPCTSTR lpszCaption
* Parameter:    DWORD dwStyle
* Parameter:    const RECT & rect
* Parameter:    CWnd * pParentWnd
* Parameter:    UINT nID
* Returns:      BOOL
*************************************************************/
BOOL CTiGraphCtrl::Create(LPCTSTR lpszCaption,
                          DWORD dwStyle,
                          const RECT& rect,
                          CWnd* pParentWnd,
                          UINT nID)
{
    return CWnd::Create(NULL, lpszCaption, dwStyle, rect, pParentWnd, nID);
}

/*************************************************************
* Method:       OnPaint
* Description:  This function is used
* Returns:      void
*************************************************************/
void CTiGraphCtrl::OnPaint()
{
    try { 
	if (l_mybrush.m_hObject) { //AN
        //l_mybrush.DeleteObject();
    } else {
       l_mybrush.CreateSolidBrush(RGB(0, 0, 0));
	}
    bool l_IsT1MapSelected;
    l_IsT1MapSelected =  CTiPrepView::GetView()->GetT1MapFlag();
    
    if(true == l_IsT1MapSelected) { //map mode is on
        OnPaintMapMode();
    } else { //i.e avg calculation mode is on
        OnPaintAvgMode();
    }
    } catch (...)
    {
        CTiPrepLogHelper::WriteToErrorLog(_T("OnPaint() Exception Occurred"),__LINE__,__FILE__);
		_exit(0);
    }
}

/*************************************************************
* Method:       GetDrawingRect
* Description:  This function is used to draw rectangle for
*               graph displaying.
* Returns:      CRect
*               Drawing area parameters.
*************************************************************/
CRect CTiGraphCtrl::GetDrawingRect()
{
    TIPREP_TRACE(CTiGraphCtrl::GetDrawingRect,MRTraceMarker9);
    CRect l_graph_rect, l_rect_to_draw ;
    GetWindowRect(&l_graph_rect);
    const int l_graph_width = l_graph_rect.Width();
    const int l_graph_height = l_graph_rect.Height();
    const int l_drawing_width = max(l_graph_width - 40, 40);
    const int l_drawing_height = max(l_graph_height - 50, 25);
    l_rect_to_draw.left = l_graph_width - l_drawing_width - 30;
    l_rect_to_draw.top = l_graph_height - l_drawing_height ;
    l_rect_to_draw.right = l_rect_to_draw.left + l_drawing_width ;
    l_rect_to_draw.bottom = l_rect_to_draw.left + l_drawing_height ;
    return l_rect_to_draw ;
}

/*************************************************************
* Method:       PlotViewDrawAxis
* Description:  This function is used to draw axis for graph
*               displaying.
* Parameter:    CDC * f_dc
*               Drawing context.
* Parameter:    CRect * f_rect
*               Drawing area parameters.
* Returns:      void
*************************************************************/
void CTiGraphCtrl::PlotViewDrawAxis(CDC* f_dc, CRect* f_rect)
{
    TIPREP_TRACE(CTiGraphCtrl::PlotViewDrawAxis,MRTraceMarker9);
    int     l_ni = 0;
    int     l_nx1 = 0, l_ny1 = 0, l_nx2 = 0, l_ny2 = 0;
    int     l_nscaleonaxis_x = 0,l_nscaleonaxis_y = 0, l_dis = 0;
    CString l_tempStr = _T("");
    CString l_xAxisLable = _T("TIME (ms)");
    CString l_yAxisLabel = _T("SIGNAL INTENSITY");
    int offsetMargin_Y = 10;//default value

//---------------------------------------------------------A Unit Value for X-----------------------------------------------------------------------------------
    if ((m_Xmax - m_Xmin) < 10) {
        int diff = 10 -( m_Xmax - m_Xmin);
        if (diff % 2 == 0)
        {
            m_Xmax += diff/2;
            m_Xmin -= diff/2;
        } else {
            m_Xmax += (diff + 1)/2;
            m_Xmin -= (diff/2);
        }
    }
    l_nscaleonaxis_x = (m_Xmax - m_Xmin) / NUM_DIVISION;
//----------------------------------------------------------A Unit Value for Y---------------------------------------------------------------------------------
    int yunit = 0;

    if ((m_Ymax - m_Ymin) < 10) {
        int diff = 10 -( m_Ymax - m_Ymin);
        if (diff % 2 == 0)
        {
            m_Ymax += diff/2;
            m_Ymin -= diff/2;
        } else {
            m_Ymax += (diff + 1)/2;
            m_Ymin -= (diff/2);
        }
    }
     yunit = (m_Ymax - m_Ymin)/ NUM_DIVISION;

    {
        //Count Max number of digits in a Y values can appear on the scale
        int maxnum = 0;
        int count1 = 0,count2 = 0;
        maxnum = m_Ymax;
        while(maxnum != 0){
            maxnum=maxnum/10;
            count1++;
        }
        maxnum = m_Ymin;
        while(maxnum != 0){
            maxnum=maxnum/10;
            count2++;
        }      
        m_DynamicGraphMarginY = ((count1>count2)?count1*20 :count2*20);
        offsetMargin_Y = ((count1>count2)?count1*15 :count2*15);
    }
//--------------------------------------------------X and Y axis Lines--------------------------------------------------------------------------------------
    l_nx1 = l_nx2 = f_rect -> left + m_DynamicGraphMarginY ;   //Drawing Y Axis
    l_ny1 = f_rect->top;
    l_ny2 = f_rect->top + f_rect->Height();
    DrawLine(f_dc, CPoint(l_nx1, l_ny1), CPoint(l_nx2, l_ny2));
    l_nx1 = f_rect -> left + m_DynamicGraphMarginY;        //Drawing X Axis
    l_nx2 = f_rect->left + f_rect->Width();
    l_ny1 = l_ny2 = (int)(f_rect->top + m_Ymax * ((double)f_rect->Height() / (double)(m_Ymax - m_Ymin)));
    DrawLine(f_dc, CPoint(l_nx1, l_ny1), CPoint(l_nx2, l_ny2));
    l_dis = m_Xmin;
    l_ny2 = l_ny1 + MARKER_LENGTH;
//------------------------------------------------X AXIS SCALES----------------------------------------------------------------------------
    
    for(l_ni = 0; l_ni <= NUM_DIVISION; l_ni++) {//Scales on X axis
        // division for X Axis
        l_nx1 = l_nx2 = (long)(((f_rect->left + m_DynamicGraphMarginY) + ((f_rect->Width() - m_DynamicGraphMarginY)) *
            CalculateDataX((double)(m_Xmax - m_Xmin), (float)(l_dis - m_Xmin))));
        DrawLine(f_dc, CPoint(l_nx1, l_ny1), CPoint(l_nx1, l_ny2));
        //m_nArrsacle[l_ni] = l_dis;
        int l_nbk_mode = f_dc->SetBkMode(TRANSPARENT);
        l_tempStr.Format(_T("%d"), l_dis);
        
        if(l_ni == 0) { 
            f_dc->TextOut(l_nx1 + 1, l_ny1 + 5, l_tempStr);
        } else {
            f_dc->TextOut(l_nx1 - 10, l_ny1 + 5, l_tempStr);
        }
        
        f_dc->SetBkMode(l_nbk_mode);
        l_dis = l_dis + l_nscaleonaxis_x;
    }
//------------------------------------------------Y AXIS LABEL------------------------------------------------------------------------------
    int offx = f_rect->left;
    int offy = f_rect ->top + 100;
    
    for(l_ni = 0; l_ni < l_yAxisLabel.GetLength(); l_ni++) {
        int l_nbk_mode = f_dc->SetBkMode(TRANSPARENT);
        f_dc->SetTextAlign(TA_CENTER);
        f_dc->TextOut(offx , offy, l_yAxisLabel[l_ni]);
        offy = offy + 15;
        f_dc->SetBkMode(l_nbk_mode);
    }
//------------------------------------------------X AXIS LABEL------------------------------------------------------------------------------    
    f_dc->SetTextAlign(TA_LEFT);
    int bk_mode = f_dc->SetBkMode(TRANSPARENT);
    f_dc->TextOut(f_rect -> left + 400, f_rect -> bottom + 20, l_xAxisLable); //adjustment to avoid overlap
    f_dc->SetBkMode(bk_mode);
//------------------------------------------------Y AXIS SCALES----------------------------------------------------------------------------
    l_tempStr.Format(_T("%d"), f_rect->bottom);
    l_ny1 = f_rect->bottom;
    l_nscaleonaxis_y = yunit;//(m_Ymax - m_Ymin) / NUM_DIVISION;
    l_dis = m_Ymax;
    l_nx1 = f_rect->left + m_DynamicGraphMarginY ;
    l_nx2 = l_nx1 - MARKER_LENGTH ;
    
    l_nx1 = f_rect->left + m_DynamicGraphMarginY ;//-18 ;//for text
    int l_nx1scale = f_rect->left + m_DynamicGraphMarginY; //for '-' line as scale on y axis
    l_nx2 = l_nx1scale - (MARKER_LENGTH) ;
    int i = 0;
    int yScale = 0;
    if (l_nscaleonaxis_y == 0) {
        CTiPrepLogHelper::WriteToErrorLog(_T("y unit is 0, graph error"),__LINE__,__FILE__);
        return;
    }
    while(m_Ymin < yScale) { //Negative Scale
        yScale = -1 * i * l_nscaleonaxis_y;
        i++;
        l_ny1 = l_ny2 = long(f_rect->top + ((m_Ymax - yScale) * (double)f_rect->Height() / (double)(m_Ymax - m_Ymin)));
        
        if(l_ny1 <= f_rect->Height() + f_rect->top) {
            DrawLine(f_dc, CPoint(l_nx1scale, l_ny1), CPoint(l_nx2, l_ny1));
            int l_nbk_mode = f_dc->SetBkMode(TRANSPARENT);
            l_tempStr.Format(_T("%d"), yScale);
            f_dc->TextOut(l_nx1 - offsetMargin_Y, l_ny1 - 8, l_tempStr);
            f_dc->SetBkMode(l_nbk_mode);
        }
    }
    
    yScale = 0;
    i = 0;
    
    while(m_Ymax > yScale) { //Positive Scale
        yScale = i * l_nscaleonaxis_y;
        i++;
        l_ny1 = l_ny2 = long(f_rect->top + ((m_Ymax - yScale) * (double)f_rect->Height() / (double)(m_Ymax - m_Ymin)));
        
        if(l_ny1 >= f_rect->top) {
            DrawLine(f_dc, CPoint(l_nx1scale, l_ny1), CPoint(l_nx2, l_ny1));
            int l_nbk_mode = f_dc->SetBkMode(TRANSPARENT);
            l_tempStr.Format(_T("%d"), yScale);
            f_dc->TextOut(l_nx1 - offsetMargin_Y, l_ny1 - 8, l_tempStr);
            f_dc->SetBkMode(l_nbk_mode);
        }
    }
}

/*************************************************************
* Method:       DrawLine
* Description:  This function is used to draw line.
* Parameter:    CDC * f_dc
*               Drawing context.
* Parameter:    CPoint & f_start_point
*               Start point of the line.
* Parameter:    CPoint & f_end_point
*               End point of the line.
* Returns:      void
*************************************************************/
void CTiGraphCtrl::DrawLine(CDC* f_dc , CPoint &f_start_point, CPoint &f_end_point)
{
    TIPREP_TRACE(CTiGraphCtrl::DrawLine,MRTraceMarker9);
    POINT lp ;
    ::MoveToEx(f_dc->m_hDC, f_start_point.x , f_start_point.y , &lp);
    ::LineTo(f_dc->m_hDC, f_end_point.x , f_end_point.y);
}

/*************************************************************
* Method:       CalculateDataX
* Description:  This function is used to calculate scaling for
*               X axis.
* Parameter:    double f_xrange
*               X axis range.
* Parameter:    float f_fx
*               Y axis co-ordinate.
* Returns:      float
*               Scaled value for plotting.
*************************************************************/
float CTiGraphCtrl::CalculateDataX(double f_xrange, float    f_fx)
{
    TIPREP_TRACE(CTiGraphCtrl::CalculateDataX,MRTraceMarker9);
    float result = 0.0;
    
    if(fabs(f_xrange) < 0.0000001) {
        return(0.5);
    }
    
    result = (float)(f_fx / f_xrange);
#ifdef _UT_LOG_
    CTiPrepLogHelper::WriteToTraceLog
        (_T("MRI_TIPREP"), MRTraceMarker12, _T("Success"), _T("CTiGraphCtrl::CalculateDataX"));
#endif
    return(result);
}

/*************************************************************
* Method:       CalculateDataY
* Description:  This function is used to calculate scaling for
*               Y axis.
* Parameter:    float f_fy
*               X axis co-ordinate.
* Returns:      float
*               Scaled value for plotting.
*************************************************************/
float CTiGraphCtrl::CalculateDataY(float f_fy)
{
    TIPREP_TRACE(CTiGraphCtrl::CalculateDataY,MRTraceMarker9);
    float result = 0.0f;
    
    if(TiPrepWindow.y[2] == 0.0) {
        return 0.5;
    }
    
    result = (float)(1.0 - (f_fy - TiPrepWindow.y[0]) / TiPrepWindow.y[2]);
#ifdef _UT_LOG_
    CTiPrepLogHelper::WriteToTraceLog
        (_T("MRI_TIPREP"), MRTraceMarker12, _T("Success"), _T("CTiGraphCtrl::CalculateDataY"));
#endif
    return result;
}

/*************************************************************
* Method:       DisplayGraph
* Description:  This function displays the graph on GUI.
* Parameter:    BOOL f_flag
*               Set the flag for the graph display.
* Returns:      void
*************************************************************/
void CTiGraphCtrl::DisplayGraph(BOOL f_flag)
{
    TIPREP_TRACE(CTiGraphCtrl::DisplayGraph,MRTraceMarker9);
    m_wndFlag = f_flag;
    Invalidate();
}

/*************************************************************
* Method:       SetGraphData
* Description:  This function displays the graph on GUI.
* Parameter:    long f_nselectImg
*               Total no of selected images.
* Parameter:    int * f_time_data
*               Timestamp of selected image.
* Parameter:    double * * f_dRoi_Org_Data
*               Original data of ROI.
* Parameter:    double * * f_dRoi_Inv_Data
*               Inverted data of ROI.
* Parameter:    double * * f_dRoi_Fit_Data
*               Fitted data of ROI.
* Parameter:    double * * f_dRoi_Fit_Rev_Data
*               Fitted reverse data of ROI.
* Parameter:    double * * f_dRoi_Inv_Rev_Data
*               Inverted reverse data of ROI.
* Parameter:    double * f_dRoi_Ti
* Parameter:    double * f_dRoi_MinRev_Ti
* Parameter:    double **f_dNoise_Roi_Org_Data
* Parameter:    int
* Returns:      int
*               0 on success.
*************************************************************/
int CTiGraphCtrl::SetGraphData(long f_nselectImg, int *f_time_data,
                               double **f_dRoi_Org_Data,
                               double **f_dRoi_Inv_Data,
                               double **f_dRoi_Fit_Data ,
                               double **f_dRoi_Fit_Rev_Data ,
                               double **f_dRoi_Inv_Rev_Data,
                               double *f_dRoi_Ti,
                               double *f_dRoi_MinRev_Ti,
                               double **f_dNoise_Roi_Org_Data,  
                               int f_Ymax, int f_Ymin,
                               int f_Xmax, int f_Xmin, long f_nNumOfPoints,
                               double *f_dRoi_T1,
                               double *f_dRoi_MinRev_T1) 
{
    TIPREP_TRACE(CTiGraphCtrl::SetGraphData,MRTraceMarker3);
    
    try{
        if(NULL == f_dRoi_Org_Data ||
            NULL == f_dRoi_Inv_Data ||
            NULL == f_dRoi_Fit_Data ||
            NULL == f_dRoi_Fit_Rev_Data ||
            NULL == f_dRoi_Inv_Rev_Data ||
            NULL == f_dRoi_Ti ||
            NULL == f_dRoi_MinRev_Ti ||
            NULL == f_dNoise_Roi_Org_Data || 
            0 >= f_nselectImg ||
            0 >= f_nNumOfPoints) {
            CTiPrepLogHelper::WriteToErrorLog(_T("CTiGraphCtrl::SetGraphData Input Paramter NULL"), __LINE__, __FILE__);
            return (E_ERROR);
        }
        
        m_nselectImgNew = f_nselectImg;
        m_Ymax = f_Ymax;
        m_Ymin = f_Ymin;
        m_Xmax = f_Xmax;
        m_Xmin = f_Xmin;
        
        if(m_Ymax == 0 && m_Ymin == 0) {
            m_Ymax = 10;
            m_Ymin = -10;
        }
        
#ifdef _UT_LOG_
        CString l_csvalue(_T(""));
        l_csvalue.Format(_T("X-Max Val:  %d, X-Min Val: %d\n Y-Max val: %d, Y-Min Val: %d\n"), f_Xmax, f_Xmin, f_Ymax, f_Ymin);
        CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker12, l_csvalue, _T("CTiGraphCtrl::SetGraphData"));
#endif
        m_nNumOfPoints = f_nNumOfPoints;   
        m_dNoise_ROI_Org_Data = f_dNoise_Roi_Org_Data;
        m_dRoi_Org_Data = f_dRoi_Org_Data;
        m_dRoi_Fit_Data = f_dRoi_Fit_Data;
        m_dRoi_Fit_Rev_Data = f_dRoi_Fit_Rev_Data;
        m_dRoi_Inv_Data = f_dRoi_Inv_Data;
        m_dRoi_Inv_Rev_Data = f_dRoi_Inv_Rev_Data;
        m_time_data = f_time_data;
        m_dRoi_Ti_Data = f_dRoi_Ti;
        m_dRoi_MinRev_Ti_Data = f_dRoi_MinRev_Ti;
        m_dRoi_T1_Data = f_dRoi_T1;
        m_dRoi_MinRev_T1_Data = f_dRoi_MinRev_T1;
        m_rect = GetDrawingRect();
        
    }catch(...){
        CTiPrepLogHelper::WriteToErrorLog(_T("SetGraphData:Exception Occurred"), __LINE__, __FILE__);
        return (E_ERROR);
    }
    return E_NO_ERROR;
}

/*************************************************************
* Method:       ResetGraphWindow
* Description:  This function resets the graph window.
* Parameter:    BOOL f_windFlag
*               Flag for the graph window.
* Parameter:    BOOL f_ROI[]
*               selected ROI status.
* Parameter:    BOOL f_ROIRev[]
*               ROI reverse status.
* Parameter:    BOOL f_graphOpt[]
*               Graph drawing status.
* Returns:      void
*************************************************************/
void CTiGraphCtrl::ResetGraphWindow(BOOL f_windFlag,
                                    BOOL f_ROI[],
                                    BOOL f_ROIRev[],
                                    BOOL f_graphOpt[])
{
    TIPREP_TRACE(CTiGraphCtrl::ResetGraphWindow,MRTraceMarker9);
    
    if(!f_windFlag) {
        DisplayGraph(f_windFlag);
    } else {
        m_ROI[0] = f_ROI[0];
        m_ROI[1] = f_ROI[1];
        m_ROI[2] = f_ROI[2];
        m_ROIRev[0] = f_ROIRev[0];
        m_ROIRev[1] = f_ROIRev[1];
        m_ROIRev[2] = f_ROIRev[2];
        m_graphOpt[0] = f_graphOpt[0];
        m_graphOpt[1] = f_graphOpt[1];
        m_graphOpt[2] = f_graphOpt[2];
        DisplayGraph(f_windFlag);
    }
    
    CTiPrepLogHelper::WriteToTraceLog
        (_T("MRI_TIPREP"), MRTraceMarker12, _T("Success"), _T("CTiGraphCtrl::ResetGraphWindow"));
}


void CTiGraphCtrl::ResetGraphWindowNoiseRoi(BOOL f_windFlag, BOOL f_ROI[])
{
    TIPREP_TRACE(CTiGraphCtrl::ResetGraphWindowNoiseRoi,MRTraceMarker9);
    
    if(!f_windFlag) {
        DisplayGraph(f_windFlag);
    } else {
        m_NoiseROI[0] = f_ROI[0];    
        m_NoiseROI[1] = f_ROI[1];    
        DisplayGraph(f_windFlag);
    }
    
    CTiPrepLogHelper::WriteToTraceLog
        (_T("MRI_TIPREP"), MRTraceMarker12, _T("Success"), _T("CTiGraphCtrl::ResetGraphWindowNoiseRoi"));
}

/*************************************************************
* Method:       PlotViewDrawData
* Description:  This function plots the data on the graph.
* Parameter:    CDC * f_dc
*               Drawing context.
* Parameter:    CRect * f_rect
*               Drawing area parameters.
* Parameter:    double * f_fplotArray
*               Graph data.
* Returns:      void
*************************************************************/
void CTiGraphCtrl::PlotViewDrawData(CDC* f_dc, CRect* f_rect, double *f_fplotArray, int FitDataFlag)
{
    TIPREP_TRACE(CTiGraphCtrl::PlotViewDrawData,MRTraceMarker9);
    CPoint l_plotPoint1, l_plotPoint2;
    l_plotPoint1.x = long(f_rect->left + m_DynamicGraphMarginY + ((f_rect->Width() - m_DynamicGraphMarginY) *
        CalculateDataX((double)(m_Xmax - m_Xmin), (float)(m_time_data[0] - m_time_data[0]))));
    l_plotPoint1.y = long(f_rect->top + ((m_Ymax - (int)f_fplotArray[0]) *
        (double)f_rect->Height() / (double)(m_Ymax - m_Ymin)));
    int loopLim = 0;
    
    if((FitDataFlag == 1) && (m_nselectImgNew > m_nselectImg)) {
        loopLim = m_nselectImgNew;
    } else {
        loopLim = m_nselectImg;
    }
    
    for(int i = 1; i < loopLim; i++) {
        if(FitDataFlag == 0 && m_nselectImgNew > m_nselectImg) {
            l_plotPoint2.x = long(f_rect->left + m_DynamicGraphMarginY + ((f_rect->Width() - m_DynamicGraphMarginY) *
                CalculateDataX((double)(m_Xmax - m_Xmin), (float)(m_time_data[5*i] - m_time_data[0])))); 
        } else {
            l_plotPoint2.x = long(f_rect->left + m_DynamicGraphMarginY + ((f_rect->Width() - m_DynamicGraphMarginY) *
                CalculateDataX((double)(m_Xmax - m_Xmin), (float)(m_time_data[i] - m_time_data[0]))));
        }
        
        l_plotPoint2.y = long(f_rect->top + ((m_Ymax - (int)f_fplotArray[i]) *
            (double)f_rect->Height() / (double)(m_Ymax - m_Ymin)));
        DrawLine(f_dc, l_plotPoint1, l_plotPoint2);
        CString l_tempStr = _T("");
        l_tempStr.Format(_T("%f"), f_fplotArray[i]);
        int bk_mode = f_dc->SetBkMode(TRANSPARENT);
        f_dc->SetBkMode(bk_mode);
        l_plotPoint1 = l_plotPoint2;
    }
}

/*************************************************************
* Method:       SetMeanTiValue
* Description:  This function is used
* Parameter:    CString & f_csMeanTi
* Returns:      void
*************************************************************/
void CTiGraphCtrl::SetMeanTiValue(CString &f_csMeanTi)
{
    m_csMeanTiValue = f_csMeanTi;
}

/*************************************************************
* Method:       SetXAxisMinVal
* Description:  This function is used
* Parameter:    int f_Xmin
* Returns:      void
*************************************************************/
void CTiGraphCtrl::SetXAxisMinVal(int f_Xmin)
{
    m_Xmin = f_Xmin;
}

/*************************************************************
* Method:       SetXAxisMaxVal
* Description:  This function is used
* Parameter:    int f_Xmax
* Returns:      void
*************************************************************/
void CTiGraphCtrl::SetXAxisMaxVal(int f_Xmax)
{
    m_Xmax = f_Xmax;
}

/*************************************************************
* Method:       SetYAxisMinVal
* Description:  This function is used
* Parameter:    int f_Ymin
* Returns:      void
*************************************************************/
void CTiGraphCtrl::SetYAxisMinVal(int f_Ymin)
{
    m_Ymin = f_Ymin;
}
/*************************************************************
* Method:       SetYAxisMaxVal
* Description:  This function is used
* Parameter:    int f_Ymax
* Returns:      void
*************************************************************/
void CTiGraphCtrl::SetYAxisMaxVal(int f_Ymax)
{
    m_Ymax = f_Ymax;
}
/*************************************************************
* Method:       GetT1MapCheckBoxStatus
* Description:  This function is used
* Parameter:    int & f_t1_map_check_box_status
* Returns:      void
*************************************************************/
void CTiGraphCtrl::GetT1MapCheckBoxStatus(int &f_t1_map_check_box_status) 
{
    m_t1_map_check_box_status = f_t1_map_check_box_status;
    Invalidate();
}
/*************************************************************
* Method:    	OnPaintAvgMode
* Description: 	This function is used 
* Returns:   	void
*************************************************************/
void CTiGraphCtrl::OnPaintAvgMode()
{
    try {
    CPaintDC l_dc(this); // device context for painting
    RECT l_rect;
    CBrush  *l_poldbrush = NULL;//AN
    CPen* l_poldpen = NULL;
    CString l_tempStr = _T("");
   //l_mybrush.CreateSolidBrush(RGB(0, 0, 0));//AN
    CPen frame_rect_pen(PS_SOLID, 1 , RGB(42, 80, 145));
    l_poldbrush = l_dc.SelectObject(&l_mybrush);
    l_poldpen = l_dc.SelectObject(&frame_rect_pen);
    GetClientRect(&l_rect);
    l_dc.Rectangle(&l_rect);
    int l_nPlacement = 475;
    COLORREF old_color = l_dc.SetTextColor(g_text_color);
    // Create a solid red pen of width 1.
    CPen gray_pen(PS_SOLID , 1 , g_graph_line_color);
    l_dc.SelectObject(&gray_pen);
    
    if(m_wndFlag) {
        if(NULL == m_dRoi_Org_Data || NULL == m_dRoi_Org_Data[0]) {
            return ;
        }
        
        PlotViewDrawAxis(&l_dc, &m_rect);
        int l_nbk_mode = l_dc.SetBkMode(TRANSPARENT);
        l_tempStr.Format(_T("Mean Ti:"));
        l_dc.TextOut(m_rect.left + l_nPlacement, 5, l_tempStr);
        
        if(m_csMeanTiValue.IsEmpty()) {
            m_csMeanTiValue.Format(_T("---.---"));
            l_dc.TextOut(m_rect.left + l_nPlacement, 25, m_csMeanTiValue);
        } else {
            l_dc.TextOut(m_rect.left + l_nPlacement, 25, m_csMeanTiValue);
        }
        
        l_dc.SetBkMode(l_nbk_mode);
        CPen Roi1_pen(PS_SOLID , 1 , Roi1_curve_marking_color); 
        l_dc.SetBkMode(TRANSPARENT); 
        l_dc.SetTextColor(Roi1_curve_marking_color);
        l_nPlacement = 100;
        l_nbk_mode = l_dc.SetBkMode(TRANSPARENT);
        l_tempStr.Format(_T("ROI 1:"));
        l_dc.TextOut(m_rect.left + l_nPlacement, 5, l_tempStr);
        l_dc.SetBkMode(l_nbk_mode);
        CPen Roi2_pen(PS_SOLID , 1 , Roi2_curve_marking_color);
        l_dc.SetBkMode(TRANSPARENT); 
        l_dc.SetTextColor(Roi2_curve_marking_color);
        l_nPlacement = 225;
        l_nbk_mode = l_dc.SetBkMode(TRANSPARENT);
        l_tempStr.Format(_T("ROI 2:"));
        l_dc.TextOut(m_rect.left + l_nPlacement, 5, l_tempStr);
        l_dc.SetBkMode(l_nbk_mode);
        CPen Roi3_pen(PS_SOLID , 1 , Roi3_curve_marking_color); 
        l_dc.SetBkMode(TRANSPARENT); 
        l_dc.SetTextColor(Roi3_curve_marking_color);
        l_nPlacement = 350;
        l_nbk_mode = l_dc.SetBkMode(TRANSPARENT);
        l_tempStr.Format(_T("ROI 3:"));
        l_dc.TextOut(m_rect.left + l_nPlacement, 5, l_tempStr);
        l_dc.SetBkMode(l_nbk_mode);
        CPen Roi4_pen(PS_SOLID , 1 , Roi4_curve_marking_color);  
        l_nbk_mode = l_dc.SetBkMode(TRANSPARENT);
        l_dc.SetBkMode(l_nbk_mode);
        CPen Roi5_pen(PS_SOLID , 1 , Roi5_curve_marking_color); 
        l_nbk_mode = l_dc.SetBkMode(TRANSPARENT);
        l_dc.SetBkMode(l_nbk_mode);
        CPen l_fit_data_pen_roi1(PS_DOT , 1 , Roi1_curve_marking_color);
        CPen l_fit_data_pen_roi2(PS_DOT , 1 , Roi2_curve_marking_color);
        CPen l_fit_data_pen_roi3(PS_DOT , 1 , Roi3_curve_marking_color);
        
        for(int l_nroi = 0; l_nroi < MAX_NUM_ROI ; l_nroi++) { 
            switch(l_nroi) {
            case 0:
                l_dc.SelectObject(&Roi1_pen);
                l_dc.SetTextColor(Roi1_curve_marking_color);
                l_tempStr.Format(_T("ROI	1:"));
                l_nPlacement = 100;
                break;
            case 1:
                l_dc.SelectObject(&Roi2_pen);
                l_dc.SetTextColor(Roi2_curve_marking_color);
                l_tempStr.Format(_T("ROI	2:"));
                l_nPlacement = 225;
                break;
            case 2:
                l_dc.SelectObject(&Roi3_pen);
                l_dc.SetTextColor(Roi3_curve_marking_color);
                l_tempStr.Format(_T("ROI	3:"));
                l_nPlacement = 350;
                break;
            default:
                break;
                ;
            }
            
            if(m_ROI[l_nroi]) {
                if(m_ROIRev[l_nroi]) {
                    l_nbk_mode = l_dc.SetBkMode(TRANSPARENT);
                    if ( m_dRoi_MinRev_T1_Data[l_nroi] != -1)
                    {
                        l_tempStr.Format(_T("%0.1lf"), m_dRoi_MinRev_Ti_Data[l_nroi]/*+0.05*/);
                        l_dc.TextOut(m_rect.left + l_nPlacement, 25, l_tempStr);
                    }else{
                        l_tempStr.Format(_T("Err"));
                        l_dc.TextOut(m_rect.left + l_nPlacement, 25, l_tempStr);
                    }
                    l_dc.SetBkMode(l_nbk_mode);
                } else {
                    l_nbk_mode = l_dc.SetBkMode(TRANSPARENT);
                    if ( m_dRoi_T1_Data[l_nroi] != -1)
                    {
                        l_tempStr.Format(_T("%0.1lf"), m_dRoi_Ti_Data[l_nroi]/*+0.05*/);
                        l_dc.TextOut(m_rect.left + l_nPlacement, 25, l_tempStr);
                    }else{
                        l_tempStr.Format(_T("Err"));
                        l_dc.TextOut(m_rect.left + l_nPlacement, 25, l_tempStr);
                    }
//                     l_tempStr.Format(_T("%0.1lf"), m_dRoi_Ti_Data[l_nroi]+0.05);
//                     l_dc.TextOut(m_rect.left + l_nPlacement, 25, l_tempStr);
                    l_dc.SetBkMode(l_nbk_mode);
                }
                
                if(m_graphOpt[0]) {
                    PlotViewDrawData(&l_dc, &m_rect, m_dRoi_Org_Data[l_nroi], 0);
                }
                
                if(m_graphOpt[1]) {
                    if(m_ROIRev[l_nroi]) {
                        PlotViewDrawData(&l_dc, &m_rect, m_dRoi_Inv_Rev_Data[l_nroi], 0);
                    } else {
                        PlotViewDrawData(&l_dc, &m_rect, m_dRoi_Inv_Data[l_nroi], 0);
                    }
                }
                
                if(m_graphOpt[2]) {
                    if(l_nroi == 0) {
                        l_dc.SelectObject(&l_fit_data_pen_roi1);
                    }
                    
                    if(l_nroi == 1) {
                        l_dc.SelectObject(&l_fit_data_pen_roi2);
                    }
                    
                    if(l_nroi == 2) {
                        l_dc.SelectObject(&l_fit_data_pen_roi3);
                    }
                    
                    l_dc.SetBkMode(TRANSPARENT);
                    
                    if(m_ROIRev[l_nroi]) {
                        PlotViewDrawData(&l_dc, &m_rect, m_dRoi_Fit_Rev_Data[l_nroi], 1);
                    } else {
                        PlotViewDrawData(&l_dc, &m_rect, m_dRoi_Fit_Data[l_nroi], 1);
                    }
                }
            } else {
                l_nbk_mode = l_dc.SetBkMode(TRANSPARENT);
                l_tempStr.Format(_T("---.---"));
                l_dc.TextOut(m_rect.left + l_nPlacement, 25, l_tempStr);
                l_dc.SetBkMode(l_nbk_mode);
            }
        }
        
        //plotting noise data
        for(l_nroi = 0; l_nroi < MAX_NOISE_ROI ; l_nroi++) {
            if(m_NoiseROI[l_nroi]) {
                switch(l_nroi) {
                case 0:
                    l_dc.SelectObject(&Roi4_pen);
                    l_dc.SetTextColor(Roi4_curve_marking_color);
                    break;
                case 1:
                    l_dc.SelectObject(&Roi5_pen);
                    l_dc.SetTextColor(Roi5_curve_marking_color);
                    break;
                default:
                    ;
                }
                if(m_graphOpt[ORG_DATA]) {
                    PlotViewDrawData(&l_dc, &m_rect, m_dNoise_ROI_Org_Data[l_nroi], 0);
                }
                
            }
        }
    }
    
    CPen blue_pen(PS_DOT , 1 , Roi3_curve_marking_color); 
    l_dc.SelectObject(&blue_pen);
    l_dc.SetTextColor(old_color);
    l_dc.SelectObject(l_poldpen);
    l_dc.SelectObject(l_poldbrush);
    } catch (...)
        {
        CTiPrepLogHelper::WriteToErrorLog(_T("Exception in OnPaintAvgMode()"),__LINE__,__FILE__);
    }
}

/*************************************************************
* Method:    	OnPaintMapMode
* Description: 	This function is used 
* Returns:   	void
*************************************************************/
void CTiGraphCtrl::OnPaintMapMode()
{
try {
    CPaintDC l_dc(this); // device context for painting
    RECT l_rect;
    CBrush  *l_poldbrush = NULL;//AN
    CPen* l_poldpen = NULL;
    CString l_tempStr = _T("");
//    l_mybrush.CreateSolidBrush(RGB(0, 0, 0));//AN
    CPen frame_rect_pen(PS_SOLID, 1 , RGB(42, 80, 145));
    l_poldbrush = l_dc.SelectObject(&l_mybrush);
    l_poldpen = l_dc.SelectObject(&frame_rect_pen);
    GetClientRect(&l_rect);
    l_dc.Rectangle(&l_rect);
    int l_nPlacement = 225;
    COLORREF old_color = l_dc.SetTextColor(g_text_color);
    // Create a solid red pen of width 1.
    CPen gray_pen(PS_SOLID , 1 , g_graph_line_color);
    l_dc.SelectObject(&gray_pen);
    
    if(m_wndFlag) {
        if(NULL == m_dRoi_Org_Data || NULL == m_dRoi_Org_Data[0]) {
            return ;
        }
        
        PlotViewDrawAxis(&l_dc, &m_rect);
        int l_nbk_mode = l_dc.SetBkMode(TRANSPARENT);
        l_tempStr.Format(_T("Mean Ti:"));
        l_dc.TextOut(m_rect.left + l_nPlacement, 5, l_tempStr);
        
        if(m_csMeanTiValue.IsEmpty()) {
            m_csMeanTiValue.Format(_T("---.---"));
            l_dc.TextOut(m_rect.left + l_nPlacement, 25, m_csMeanTiValue);
        } else {
            l_dc.TextOut(m_rect.left + l_nPlacement, 25, m_csMeanTiValue);
        }
        
        l_dc.SetBkMode(l_nbk_mode);
        CPen Roi1_pen(PS_SOLID , 1 , Roi1_curve_marking_color);
        l_dc.SetTextColor(Roi1_curve_marking_color);
        l_nPlacement = 100;
        l_nbk_mode = l_dc.SetBkMode(TRANSPARENT);
        l_tempStr.Format(_T("Map Ti:")); 
        l_dc.TextOut(m_rect.left + l_nPlacement, 5, l_tempStr);
        l_dc.SetBkMode(l_nbk_mode);
        CPen Roi4_pen(PS_SOLID , 1 , Roi4_curve_marking_color);
        l_nbk_mode = l_dc.SetBkMode(TRANSPARENT);
        l_dc.SetBkMode(l_nbk_mode);
        CPen Roi5_pen(PS_SOLID , 1 , Roi5_curve_marking_color);
        l_nbk_mode = l_dc.SetBkMode(TRANSPARENT);
        l_dc.SetBkMode(l_nbk_mode);
        long l_nNumOfPoints = 0;
        l_nNumOfPoints = m_nNumOfPoints;
        int l_nSelectedIndex = 0;
        l_nSelectedIndex = CTiPrepView::GetView()->GetSelectedIndex();
        
        if((l_nSelectedIndex < 0) || (l_nSelectedIndex >= l_nNumOfPoints)) { 
            l_nbk_mode = l_dc.SetBkMode(TRANSPARENT);
            l_tempStr.Format(_T("---.---"));
            l_dc.TextOut(m_rect.left + l_nPlacement, 25, l_tempStr);
            CString l_csval(_T(""));
            l_csval.Format(_T("OnPaintMapMode: Selected Index: %d"),l_nSelectedIndex);
            CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker14, l_csval, _T("OnPaintMapMode(): Selected index out of range error"));
            return;
        }
        
        l_dc.SelectObject(&Roi1_pen);
        
        if(m_ROI[0]) {
            if(m_ROIRev[0]) {   //pending //check index
                l_nbk_mode = l_dc.SetBkMode(TRANSPARENT);
                l_tempStr.Format(_T("%0.1lf"), m_dRoi_MinRev_Ti_Data[l_nSelectedIndex]/*+0.05*/);
                if(l_tempStr.IsEmpty()) {
                    l_tempStr.Format(_T("---.---"));
                    l_dc.TextOut(m_rect.left + l_nPlacement, 25, l_tempStr);
                } else {
                    if (m_dRoi_MinRev_T1_Data[l_nSelectedIndex] != -1)
                    {
                        l_dc.TextOut(m_rect.left + l_nPlacement, 25, l_tempStr);//chk here
                    }else{
                        l_tempStr.Format(_T("Err"));
                        l_dc.TextOut(m_rect.left + l_nPlacement, 25, l_tempStr);
                    }
                    
                }
                l_dc.SetBkMode(l_nbk_mode);
            } else {
                l_nbk_mode = l_dc.SetBkMode(TRANSPARENT);
                l_tempStr.Format(_T("%0.1lf"), m_dRoi_Ti_Data[l_nSelectedIndex]/*+0.05*/);
                if(l_tempStr.IsEmpty()) {
                    l_tempStr.Format(_T("---.---"));
                    l_dc.TextOut(m_rect.left + l_nPlacement, 25, l_tempStr);
                } else {
                    if (m_dRoi_T1_Data[l_nSelectedIndex] != -1)
                    {
                        l_dc.TextOut(m_rect.left + l_nPlacement, 25, l_tempStr);//chk here
                    }else{
                        l_tempStr.Format(_T("Err"));
                        l_dc.TextOut(m_rect.left + l_nPlacement, 25, l_tempStr);
                    }
                    
                    //l_dc.TextOut(m_rect.left + l_nPlacement, 25, l_tempStr);//chk here
                    
                }
                l_dc.SetBkMode(l_nbk_mode);
            }
        }else{
            //if roi is off mean ti and map ti will Display "---.---"
            l_nPlacement = 225;// for mean ti value
            l_nbk_mode = l_dc.SetBkMode(TRANSPARENT);
            l_tempStr.Format(_T("---.---"));
            l_dc.TextOut(m_rect.left + l_nPlacement, 25, l_tempStr);
            l_dc.SetBkMode(l_nbk_mode);

            l_nPlacement = 100;// for map ti value
            l_nbk_mode = l_dc.SetBkMode(TRANSPARENT);
            l_tempStr.Format(_T("---.---"));
            l_dc.TextOut(m_rect.left + l_nPlacement, 25, l_tempStr);
            l_dc.SetBkMode(l_nbk_mode);
        }
        
        if(m_ROI[0]) {
            if(m_graphOpt[ORG_DATA]) {
                PlotViewDrawData(&l_dc, &m_rect, m_dRoi_Org_Data[l_nSelectedIndex], 0);
            }
            
            if(m_graphOpt[INV_DATA]) {
                if(m_ROIRev[0]) {
                    PlotViewDrawData(&l_dc, &m_rect, m_dRoi_Inv_Rev_Data[l_nSelectedIndex], 0);
                } else {
                    PlotViewDrawData(&l_dc, &m_rect, m_dRoi_Inv_Data[l_nSelectedIndex], 0);
                }
            }
            
            if(m_graphOpt[FIT_DATA]) {
                CPen l_fit_data_pen_roi1(PS_DOT , 1 , Roi1_curve_marking_color);
                l_dc.SelectObject(&l_fit_data_pen_roi1);
                l_dc.SetBkMode(TRANSPARENT);
                
                if(m_ROIRev[0]) {
                    PlotViewDrawData(&l_dc, &m_rect, m_dRoi_Fit_Rev_Data[l_nSelectedIndex], 1);
                } else {
                    PlotViewDrawData(&l_dc, &m_rect, m_dRoi_Fit_Data[l_nSelectedIndex], 1);
                }
            }
        } 
        
        for(int l_nroi = 0; l_nroi < MAX_NOISE_ROI ; l_nroi++) {
            if(m_NoiseROI[l_nroi]) {
                switch(l_nroi) {    
                case 0:
                    l_dc.SelectObject(&Roi4_pen);
                    l_dc.SetTextColor(Roi4_curve_marking_color);
                    break;
                case 1:
                    l_dc.SelectObject(&Roi5_pen);
                    l_dc.SetTextColor(Roi5_curve_marking_color);
                    break;
                default:
                    ;
                }
                
                if(m_graphOpt[ORG_DATA]) {  
                    PlotViewDrawData(&l_dc, &m_rect, m_dNoise_ROI_Org_Data[l_nroi], 0); 
                }
            }
        }
    }
    
    CPen blue_pen(PS_DOT , 1 , Roi1_curve_marking_color); 
    l_dc.SelectObject(&blue_pen);
    l_dc.SetTextColor(old_color);
    l_dc.SelectObject(l_poldpen);
    l_dc.SelectObject(l_poldbrush);
	} catch (...)
        {
        CTiPrepLogHelper::WriteToErrorLog(_T("Exception in OnPaintMapMode()"),__LINE__,__FILE__);
    }
}       

/*************************************************************
* Method:    	ResetGraphWindowMapMode
* Description: 	This function is used 
* Parameter: 	BOOL f_windFlag :
* Parameter: 	BOOL f_ROI :
* Parameter: 	BOOL f_ROIRev :
* Parameter: 	BOOL f_graphOpt :
* Returns:   	void
*************************************************************/
void CTiGraphCtrl::ResetGraphWindowMapMode(BOOL f_windFlag, BOOL f_ROI, 
                                           BOOL f_ROIRev, BOOL f_graphOpt[])
{
    TIPREP_TRACE(CTiGraphCtrl::ResetGraphWindowMapMode,MRTraceMarker9);
    
    if(!f_windFlag) {
        DisplayGraph(f_windFlag);
    } else {
        m_ROI[0] = f_ROI;
        m_ROI[1] = false;
        m_ROI[2] = false;
        m_ROIRev[0] = f_ROIRev;
        m_ROIRev[1] = false;
        m_ROIRev[2] = false;
        m_graphOpt[0] = f_graphOpt[0];
        m_graphOpt[1] = f_graphOpt[1];
        m_graphOpt[2] = f_graphOpt[2];
        DisplayGraph(f_windFlag);
    }
    
    CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker12, _T("Success"), _T("CTiGraphCtrl::ResetGraphWindowMapMode"));
}   

/*************************************************************
* Method:    	SetNumberImgSelInGraph
* Description: 	This function is used 
* Parameter: 	long f_numImages :
* Returns:   	bool
*************************************************************/
bool CTiGraphCtrl::SetNumberImgSelInGraph(long f_numImages)
{
    if(f_numImages <= 0) {
        return false;
    }
    
    m_nselectImg = f_numImages;
    return true;
}

///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////
