#if !defined(AFX_EPISPINCTRL_H__B38EF179_E7E8_4DB3_B687_11C0EED9FDB4__INCLUDED_)
#define AFX_EPISPINCTRL_H__B38EF179_E7E8_4DB3_B687_11C0EED9FDB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EPISpinCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEPISpinCtrl window

class CEPISpinCtrl : public CSpinButtonCtrl
{
// Construction
public:
    CEPISpinCtrl();

// Attributes
public:

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CEPISpinCtrl)
    //}}AFX_VIRTUAL

// Implementation
private:
    CSliderCtrl *m_sliderCtrl;
public:
    virtual ~CEPISpinCtrl();
    void SetsliderCtrl(CSliderCtrl*);

    // Generated message map functions
protected:
    //{{AFX_MSG(CEPISpinCtrl)
    afx_msg BOOL OnDeltapos(NMHDR* pNMHDR, LRESULT* pResult);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EPISPINCTRL_H__B38EF179_E7E8_4DB3_B687_11C0EED9FDB4__INCLUDED_)
