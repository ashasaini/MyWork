//TMSC-REDMINE-783
//TMSC-Redmine-780
//Redmine-777
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMGraphCtrl.h
 *  Overview: Structure of CPQMGraphCtrl class.
 *  Last Modified: 2009/05/06 15:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2007/6/15 12:00:00     PQM5 Baselined code
 *   2.0          Patni            2007/8/10 12:00:00     Implemented PQM6 requirements & Formatted
 *	 3.0          Patni            2009/05/06 15:00:00     Re implemented according to Vantage 9.30
 *****************************************************************************/
#ifndef __SM_PQM_GRAPH_CTRL_H__
#define __SM_PQM_GRAPH_CTRL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxwin.h>

// structure used to store the x and y coordinates from CF.curveData

//Patni-ARD/20090315/Added/ACE-1 # Prescan Start
typedef struct {
    char*	str;
    int		sw;
    double	x;
    double	y;
} shimPeak_t;


typedef struct {
    flt32_t	x;
    char*	str;
} shimAxisScale_t;

//+Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413
typedef struct ShimWindow {

    int			shimFlag;
    int			ppm_offset;

    double*		cf_drawP;
    CPoint*		cf_curveP;
    int			cf_drawPoint;

    shimAxisScale_t*	scale;
    int			num_scale;

    //PqmPrData_t		*currObj;

    double		x[3];
    double		y[3];

    shimPeak_t		peak[2];

    float		centerFreq;

} PqmShimWindow;
//-Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413

typedef struct PreScanCFMethod_s {
    int		overSampleRate;
    int		startPoint;
    int		endPoint;
    int		centerPoint;
    int		shiftDirection;
    int		numCFAvg;
    int		adcPitch;
    int		readoutSample;
} PreScanCFMethod_t, *PreScanCFMethod_p;
//Patni-ARD/20090315/Added/ACE-1 # Prescan End

//+Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413
class CPQMGraphCtrl : public CWnd
{
public:
    static PqmShimWindow ShimWindow;	//Patni-ARD/20090315/Added/ACE-1# Prescan
    CPQMGraphCtrl();
    virtual ~CPQMGraphCtrl();
    virtual BOOL Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, const UINT nID);

protected:

    //{{AFX_MSG(CPQMGraphCtrl)
    void OnPaint();
    //}}AFX_MSG

private :
    void shimPlotViewDrawAxis(CDC* dc, CRect* rect);
    void shimPlotViewDrawData(CDC* dc, CRect* rect);
    float shimDataX(const float x)const;
    float shimDataY(const float y)const;
    void DrawLine(CDC* dc , const CPoint& f_start_point, const CPoint& f_end_point);
    CRect GetDrawingRect();

    DECLARE_MESSAGE_MAP()
};
//-Patni-HEMANT/ADDED On 9/12/2009 4:38:39 PM/MVC003413

#endif // #ifndef __SM_PQM_GRAPH_CTRL_H__
