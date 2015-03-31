#ifndef __SM_IMAGEOFFSETWND_H__
#define __SM_IMAGEOFFSETWND_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageOffsetWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImageOffsetWnd window

class CImageOffsetWnd : public CWnd
{
    // Construction
public:
    CImageOffsetWnd(const float f_max_x, const float f_max_y);
    virtual ~CImageOffsetWnd();

    const float GetDragFactorX() const ;
    const float GetDragFactorY() const ;

    void SetImagePos(const float f_percentX, const float f_percentY);

    // Generated message map functions
protected:
    //{{AFX_MSG(CImageOffsetWnd)
    afx_msg void OnPaint();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC) ;

    //}}AFX_MSG


private:

    const float m_max_x_CM;
    const float m_max_y_CM;

    CRect m_border_rect;

    CRect m_image_offset_pos;
    bool m_dragging_image_offset;
    CPoint m_drag_offset_point;

    void PaintPatientImage(CDC* f_dc);
    void UpdateBorderRect();
    void DragImageOffset(const CPoint& f_point);

    void PaintStaticImages(CDC* f_dc);
    void PaintImageOffsetPos(CDC* f_dc);
    CRect GetCenterImageRect() const;

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #ifndef __SM_IMAGEOFFSETWND_H__
