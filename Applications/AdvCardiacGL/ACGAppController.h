// ACGAppController.h: interface for the ACGAppController class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACGAPPCONTROLLER_H__146CE6D6_8CAC_47D1_B632_D5C364852F4A__INCLUDED_)
#define AFX_ACGAPPCONTROLLER_H__146CE6D6_8CAC_47D1_B632_D5C364852F4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommonDefinitions.h"

class CACGLDlg;

class ACGAppController  
{
public:
	int SendCloseToTools();
	int QueryActiveDisplayPage();
	bool IsAnyToolRunning();
	int UpdatedInvokedToolList(CString &csClosingtool);
	bool isExitClcked;
	int SendGLToolShowHide(CString &tool,bool flag);
    CACGLDlg * m_ACGLDlg; //pending shift this to public and add getter

	ACGAppController();
	virtual ~ACGAppController();

	static ACGAppController* GetInstance();
	static void DeleteInstance();

    ACGAppController& operator=(ACGAppController& f_ACGController);    

	int StartGUI();
	int QueryActivePage();
	int RegisterMsg(int branch);
	int SwitchPageScene(CString& f_PageScene);
    int GetPageScene(CString& f_csPageScene);
	int CloseTool();
	int InvokeApplication();
    int ReadToolConfigSettingsFromFile();
    int GetConfigToolSettingsForDisplay(vector<KeyValuePair>& f_KeyValuePairs);
    int SetConfigSettings( vector<KeyValuePair>& f_KeyValuePairs );
    int SetToolNumber(int f_nToolNum);
    int SetToolName(CString &f_csToolName);
    int ShowHideCardiacToolsByScene(CString &);
	void SetDlg( CACGLDlg *f_dlg )
{
    m_ACGLDlg = f_dlg;
}
    int SetButtonPressedStatus(int f_nStatus, int f_nIndex);
    int GetButtonPressedStatus(int &f_nStatus, int f_nIndex);

    void AddZOrderLayer();
    BOOL RegisterWindowZOrder(HWND hWnd_i);
    BOOL UnRegisterWindowZOrder(HWND hWnd_i);
    BOOL ChangeWindowZOrder(HWND hWnd_i);
private:
	static ACGAppController* m_ACGController;
	
    std::vector<KeyValuePair> m_KeyValuePairs;
    int m_nToolNum;
    CString m_csToolName;
    int m_ButtonPressedStatus[NUMBER_OF_TOOLS];//Max buttons
};

#endif // !defined(AFX_ACGAPPCONTROLLER_H__146CE6D6_8CAC_47D1_B632_D5C364852F4A__INCLUDED_)
///////////////////////////////// END OF FILE /////////////////////////////////