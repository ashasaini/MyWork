//TMSC-REDMINE-782
//TMSC-REDMINE-781
//TMSC-REDMINE-777
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMSliderCtrl.cpp
 *  Overview: Implementation of CPQMSliderCtrl class.
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
#include "stdafx.h"
#include "PQMSliderCtrl.h"
#include <PQM/resource.h>
#include "PQMSetScanOffsetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COLOR_SLIDER_UPPER_BORDER   RGB(172,168,153)
#define COLOR_SLIDER_LOWER_BORDER   RGB(255,255,255)

#define SILDER_CONTROL_FOCUS_COLOR          RGB(255,255,255)

BEGIN_MESSAGE_MAP(CPQMSliderCtrl, SliderCtrlEx)
    //{{AFX_MSG_MAP(CPQMSliderCtrl)
    ON_WM_KEYDOWN()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

//************************************Method Header**************************************

//Method Name:  CPQMSliderCtrl

//Author:  PATNI/SVP

//Purpose: Constructor
//**************************************************************************************
CPQMSliderCtrl::CPQMSliderCtrl(
):	m_old_pos(-111)
{
}

//************************************Method Header**************************************

//Method Name:CPQMSliderCtrl

//Author:  PATNI/SVP

//Purpose: Destructor
//**************************************************************************************
CPQMSliderCtrl::~CPQMSliderCtrl(
)
{
}



//************************************Method Header**************************************
//Method Name:  OnKeyDown

//Author:  PATNI/SVP

//Purpose: Get slider position and set value for it
//**************************************************************************************
void CPQMSliderCtrl::OnKeyDown(
    UINT ip_char,
    UINT rep_cnt,
    UINT flags
)
{
    if (ip_char == VK_LEFT ||
        ip_char == VK_RIGHT
       ) {

        if (GetPos() != m_old_pos) {

            m_old_pos = GetPos();
            CPQMSetScanOffsetDlg* p_parent = ((CPQMSetScanOffsetDlg*)GetParent());

            switch (GetDlgCtrlID()) {

                case IDC_SLIDER_X:
                    p_parent->DisplayScanOffset(p_parent->GetXSliderCtrl(), p_parent->GetXScanOffset());
                    break;

                case IDC_SLIDER_Y:
                    p_parent->DisplayScanOffset(p_parent->GetYSliderCtrl(), p_parent->GetYScanOffset());
                    break;

                case IDC_SLIDER_Z:
                    p_parent->DisplayScanOffset(p_parent->GetZSliderCtrl(), p_parent->GetXScanOffset());
                    break;
            }
        }

        SliderCtrlEx::OnKeyDown(ip_char, rep_cnt, flags);
    }
}
