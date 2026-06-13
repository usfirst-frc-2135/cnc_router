/*
	CEditScreen

	CEdit extended for custom Fonts

	Copyright:Dynomotion, Inc. 2016
*/

#pragma once


class CEditScreen;
typedef CEditScreen *LPCEditScreen;


class CEditScreen : public CEdit
{
	//	DECLARE_DYNAMIC(CEditScreen)

public:

	CEditScreen();
	virtual ~CEditScreen();
	void SetFont(const wchar_t *szFaceName, int height, bool Bold, bool Italic);
	CString ToolTipText;
	CString PrevWindowText;
	int Var;
	void Reset();
	void GetPersistText(void);
	static int SavePersists(void);
	CString GetWText();
	void SetWText(CString w);
	static bool PersistDirty;
	int GetID();
	CString GetIDName();
	static CList <LPCEditScreen, LPCEditScreen> EditScreens;




protected:
	CFont m_font;
	int CachedID;
	CString CachedIDName;
	// Overrides
	 // ClassWizard generated virtual function overrides
	 //{{AFX_VIRTUAL(CEditScreen)
public:
	//}}AFX_VIRTUAL
  // Generated message map functions
protected:
	//{{AFX_MSG(CEditScreen)
	//}}AFX_MSG
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};


