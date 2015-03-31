#ifndef __SM_MOVECOUCHWND_H__
#define __SM_MOVECOUCHWND_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MoveCouchWnd.h : header file

#include "..\PatientOrientationConstants.h"


/////////////////////////////////////////////////////////////////////////////
// CMoveCouchWnd window

class CMoveCouchWnd : public CWnd
{
    // Construction
public:
    CMoveCouchWnd(const COUCH_POSITION f_couch_pos,
                  const PatientInsertDirection f_orientationoption1,
                  const PatientRotationDirection f_orientationoption2,
                  const float f_current_z_offset_min,
                  const float f_current_z_offset_max);
    virtual ~CMoveCouchWnd();

    void InitMoveCouchWnd();

    const float GetDragFactorX() const ;
    void SetImagePos(const float f_percentX);

protected:
    //{{AFX_MSG(CMoveCouchWnd)
    afx_msg void OnPaint();

    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);

    //}}AFX_MSG

private:

    int m_inside_limit;
    int m_outside_limit;
    const int m_couch_center_left;
    const int m_couch_center_top;
    const COUCH_POSITION m_couch_pos;
    const UINT m_patient_ori_image_id;

    CRect m_patient_ori_rect;

    CRect m_couch_image_rect;
    bool m_dragging_couch;
    CPoint m_drag_couch_offset_point;

    void PaintStaticImages(CDC* f_dc) const;
    void DrawCouchImage(CDC* f_dc) const;
    void DrawPatientOriImage(CDC* f_dc) const;

    void DragCouchImage(const CPoint& f_point);
    void CenterRect(CRect& f_rect, CPoint f_center_point) const;

    void UpdateCouchImageRect();

    //void TempUpdatePatientOri();

    const float GetDivFactor();
    const CRect GetPatientOriImageRect(const COUCH_POSITION f_couch_pos) const;
    const UINT GetPatientOriImageID(const COUCH_POSITION f_couch_pos, PatientInsertDirection f_patient_in_or_out, PatientRotationDirection f_patient_side) const;
    const float GetDivFactor() const;

    const CPoint GetCenterPoint() const;
    const CPoint GetOrgCenterPoint() const;


    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #ifndef __SM_MOVECOUCHWND_H__