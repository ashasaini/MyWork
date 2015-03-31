/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#if !defined(AFX_TIGRAPHCTRL_H__F73D1850_5790_4FAA_8990_B3201DDE33B5__INCLUDED_)
#define AFX_TIGRAPHCTRL_H__F73D1850_5790_4FAA_8990_B3201DDE33B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>

/**
*  Max no of image nodes.
*/
//#define MAX_IMAGE_NODE 1024   //Van064/20130530/AS/commented

/**
* This structure is used for scaling of axis.
*/
typedef struct {
    float   x;
    char*   str;
} AxisScale_t;

/**
* This structure is used to store x and y co-ordinate.
*/
typedef struct {
    char*   str;
    int     sw;
    double  x;
    double  y;
} tiPrepPeak_t;
/**
* This structure is used draw graph.
*/
typedef struct TiPrepWindow {
    
    int         shimFlag;
    int         ppm_offset;
    double*     cf_drawP;
    CPoint*     cf_curveP;
    int         cf_drawPoint;
    AxisScale_t*    scale;
    int         num_scale;
    double      x[3];
    double      y[3];
    tiPrepPeak_t        peak[2];
    float       centerFreq;
    
} ToolTiPrepWindow;
/**
*  This class is a used for drwing graph on GUI.
*/
class CTiGraphCtrl : public CWnd
{
public:
    CTiGraphCtrl();
    CTiGraphCtrl& operator = (CTiGraphCtrl& f_GraphCtrl_ptr);

    virtual ~CTiGraphCtrl();
    
public:
    int SetGraphData(long f_nselectImg,
        int *f_time_data,
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
        double *f_dRoi_T1,double *f_dRoi_MinRev_T1);
    void SetSelectedIndex(int f_nSelectedIndex);    
    
    void ResetGraphWindow(BOOL f_windFlag, BOOL f_ROI[], BOOL f_ROIRev[], BOOL f_graphOpt[]);
    void ResetGraphWindowMapMode(BOOL f_windFlag, BOOL f_ROI, BOOL f_ROIRev, BOOL f_graphOpt[]);    
    void ResetGraphWindowNoiseRoi(BOOL f_windFlag, BOOL f_ROI[]);   
    void SetMeanTiValue(CString &f_csMeanTi);
    void SetXAxisMinVal(int f_Xmin);
    void SetXAxisMaxVal(int f_Xmax);
    void SetYAxisMinVal(int f_Ymin);
    void SetYAxisMaxVal(int f_Ymax);
    
    // Operations
    virtual BOOL Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
    static ToolTiPrepWindow TiPrepWindow;
    void GetT1MapCheckBoxStatus(int &f_t1_map_check_box_status); 
    
    bool SetNumberImgSelInGraph(long f_numImages);
    void DisplayGraph(BOOL f_flag);

public:
    
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(TiGraphCtrl)
    //}}AFX_VIRTUAL
    
    // Generated message map functions
protected:
    //{{AFX_MSG(TiGraphCtrl)
    // NOTE - the ClassWizard will add and remove member functions here.
    void OnPaint();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
        
// Implementation
private:
	CBrush l_mybrush;//AN
    int m_t1_map_check_box_status; 
    CRect GetDrawingRect();
    void PlotViewDrawAxis(CDC* dc, CRect* rect);
    void DrawLine(CDC* dc , CPoint &f_start_point, CPoint &f_end_point);
    float CalculateDataX(double x, float y);
    float CalculateDataY(float y);
    void PlotViewDrawData(CDC* f_dc, CRect* f_rect, double *f_fplotArry, int FitDataFlag);
    
    void OnPaintAvgMode();
    void OnPaintMapMode();
    
    /**
    *   X axis max value.
    */
    int m_Xmax;
    /**
    *   X axis min value.
    */
    int m_Xmin;
    /**
    *   Y axis max value.
    */
    int m_Ymax;
    /**
    *   Y axis min value.
    */
    int m_Ymin;
    /**
    *   Division points of axis.
    */
    //int m_nArrsacle[10];
    BOOL m_wndFlag;
    BOOL m_ROI[MAX_NUM_ROI];    
    
    BOOL m_NoiseROI[MAX_NOISE_ROI];
    BOOL m_ROIRev[MAX_NUM_ROI];
    BOOL m_graphOpt[MAX_NUM_ROI];
    /**
    *   Time stamp data.
    */
    int *m_time_data; 
    /**
    *   Original graph data.
    */
    double **m_dRoi_Org_Data ;
    /**
    *   Inverted graph data.
    */
    double **m_dRoi_Inv_Data ;
    /**
    *   Fit graph data.
    */
    double **m_dRoi_Fit_Data ;
    /**
    *   Fit reverse graph data.
    */
    double **m_dRoi_Fit_Rev_Data ;
    /**
    *   Inverted reverse graph data.
    */
    double **m_dRoi_Inv_Rev_Data ;
    /**
    * This variable stores original Noise ROI data.
    */
    double **m_dNoise_ROI_Org_Data;
    
    double *m_dRoi_Ti_Data;
    double *m_dRoi_MinRev_Ti_Data;
    double *m_dRoi_T1_Data;
    double *m_dRoi_MinRev_T1_Data;
    CString m_csMeanTiValue;
    long m_nselectImg;
    long m_nNumOfPoints;    
    long m_nselectImgNew;
    CRect m_rect;
    int m_DynamicGraphMarginY;
    
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIGRAPHCTRL_H__F73D1850_5790_4FAA_8990_B3201DDE33B5__INCLUDED_)
