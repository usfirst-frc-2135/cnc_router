// CComboBoxScreen.cpp : implementation file
//
/*
	Unicode Button
	©2005 Robbert E. Peters

	History:
				Version 1.0 Date: 25/09/2005

	Usage:
		Create an owner-draw button control
		Add a member for it and change CButton 2 CCComboBoxScreen
		Set the Text/Styles

		Link with Usp10.lib

	Copyright:
				You may use this code anyway you like.
*/

#include "stdafx.h"
#include "CComboBoxScreen.h"

CList <LPCComboBoxScreen, LPCComboBoxScreen> CComboBoxScreen::ComboBoxScreens;


// CComboBoxScreen

//IMPLEMENT_DYNAMIC(CComboBoxScreen, CButton)
CComboBoxScreen::CComboBoxScreen()
{
	ToolTipText = "";
	CachedID = Var = -1;
	SetFont(L"MS Sans Serif", 10, false, false);

	CComboBoxScreen::ComboBoxScreens.AddTail(this);
}

void CComboBoxScreen::Reset()
{
	ToolTipText = "";
	CachedID = Var = -1;
	SetFont(L"MS Sans Serif", 10, false, false);
	GetPersistText();

	if (m_hWnd != nullptr)
	{
		// Add the WS_TABSTOP style to ensure it can be tabbed to
		ModifyStyle(0, WS_TABSTOP);
	}
}

void CComboBoxScreen::GetPersistText(void)
{
	CScreen *scr = &TheFrame->GCodeDlg.Screen;
	if (!m_hWnd || !scr->CheckIfOKtoChangeText(GetID())) return;
	SetWText(scr->GetPersistText(GetIDName()));
}


CComboBoxScreen::~CComboBoxScreen()
{
	m_font.DeleteObject();
}

CString CComboBoxScreen::GetIDName()
{
	CScreen *scr = &TheFrame->GCodeDlg.Screen;
	if (CachedIDName == "")
	{
		CachedIDName = scr->FindResourceName(GetID());
	}
	return CachedIDName;
}

int CComboBoxScreen::GetID()
{
	if (CachedID == -1 && m_hWnd)
	{
		CachedID = GetDlgCtrlID();
	}
	return CachedID;
}

BEGIN_MESSAGE_MAP(CComboBoxScreen, CComboBox)
END_MESSAGE_MAP()



// this function is called on a WM_MOUSELEAVE

LRESULT CComboBoxScreen::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	CString t;

	switch (message)
	{
	case WM_CTLCOLOREDIT:
	case WM_IME_NOTIFY:
	case WM_PAINT:
		// detect whenever the text changes to notify KFLOP something changed 
		// must use this direct call to avoid the conversion from Unicode to ANSI
		::CallWindowProcW(*GetSuperWndProcAddr(), m_hWnd, WM_GETTEXT, 255, (LPARAM)(LPWSTR)t.GetBufferSetLength(256));
		t.ReleaseBuffer();
		if (t != PrevWindowText)
		{
			PrevWindowText = t;
			TheFrame->GCodeDlg.Screen.EditScreenChangesCount++;
			CEditScreen::PersistDirty=true;
		}
		break;

	case WM_SETFOCUS:
		TheFrame->GCodeDlg.DisableKeyJog();
		break;


	case WM_PARENTNOTIFY:
		if (wParam == WM_LBUTTONDOWN || wParam == WM_RBUTTONDOWN)
		{
			TheFrame->GCodeDlg.DisableKeyJog();
		}
		break;
	}
	return CComboBox::WindowProc(message, wParam, lParam);
}

void CComboBoxScreen::FixMyComboboxExTip(CString Text)
{
	AFX_MODULE_THREAD_STATE* pThreadState = AfxGetModuleThreadState();
	CToolTipCtrl* pToolTip = pThreadState->m_pToolTip;
	if (pToolTip == NULL || pToolTip->m_hWnd == NULL) return;

	CString sTipText=Text;

	CWnd *ChildControl = GetWindow(GW_CHILD);
	CEdit *EditControl = GetEditCtrl();

	CRect rc; GetWindowRect(&rc);
	ScreenToClient(&rc);

	TOOLINFOW rTI; memset(&rTI, 0, sizeof(TOOLINFOW));
	rTI.cbSize = sizeof(TOOLINFOW);

	rTI.hwnd = m_hWnd;
	rTI.uFlags |= TTF_IDISHWND;

	// set TTF_NOTBUTTON and TTF_CENTERTIP if it isn't a button
	rTI.uFlags |= TTF_NOTBUTTON | TTF_CENTERTIP;

	rTI.lpszText = Text.GetBuffer(); // finish setup

	if (EditControl)
	{
		rTI.uId = (WPARAM)EditControl->m_hWnd;
		pToolTip->SendMessage(TTM_ADDTOOLW, 0, (LPARAM)&rTI);
	}

	rTI.uId = (WPARAM)ChildControl->m_hWnd;
	pToolTip->SendMessage(TTM_ADDTOOLW, 0, (LPARAM)&rTI);
	pToolTip->SendMessage(TTM_ACTIVATE, TRUE);
	Text.ReleaseBuffer();
}

void CComboBoxScreen::SetFont(const wchar_t *szFaceName, int height, bool Bold, bool Italic)
{
	// Check if the font is already set with the same parameters
	LOGFONT lf = { 0 };
	if (m_font.GetSafeHandle() && m_font.GetLogFont(&lf)) {
		// Compare font attributes
		if (
			lf.lfHeight == height &&
			lf.lfWeight == (Bold ? FW_BOLD : FW_NORMAL) &&
			lf.lfItalic == (BYTE)Italic &&
			wcscmp(lf.lfFaceName, szFaceName) == 0
			)
		{
			// Font is already set, no need to change
			return;
		}
	}
	m_font.DeleteObject();
	m_font.CreateFont(height, 0, 0, 0, Bold ? FW_BOLD : FW_NORMAL, Italic, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, szFaceName);
	if (m_hWnd)	CComboBox::SetFont(&m_font);
}

void CComboBoxScreen::InsertItem(CString p)
{
	COMBOBOXEXITEMW cbei;
	cbei.mask = CBEIF_TEXT;

	cbei.iItem = -1;
	cbei.pszText = p.GetBuffer();
	::SendMessageW(m_hWnd, CBEM_INSERTITEMW, 0, LPARAM(&cbei));
}


void CComboBoxScreen::SetTextAndDropDown(CString s)
{
	// Delete every item from the combo box.
	ResetAll();

	// create list.  Strings are separated by '/' 
	// if first one is specified set as text

	int i = 0;
	bool Done;

	CString a = Part(i++, s, Done);

	if (a != "")
	{
		CString w = GetWText();
		if (w == "")
			SetWText(a);
	}

	while (!Done)
	{
		InsertItem(Part(i++, s, Done));
	}
}


void CComboBoxScreen::ResetAll()
{
	// Delete every item from the combo box.
	for (int i = GetCount() - 1; i >= 0; i--)
		DeleteString(i);
}


// Return control text as a wide string
CString CComboBoxScreen::GetWText()
{
	CString w;
	::CallWindowProcW(*GetSuperWndProcAddr(), m_hWnd, WM_GETTEXT, 2000, (LPARAM)(LPWSTR)w.GetBufferSetLength(2001));
	w.ReleaseBuffer();
	return w;
}

// Set control text as a wide string
void CComboBoxScreen::SetWText(CString w)
{
	::CallWindowProcW(*GetSuperWndProcAddr(), m_hWnd, WM_SETTEXT, 0, (LPARAM)(LPCWSTR)w);
}


// extract a string into parts separated by semicolons
CString CComboBoxScreen::Part(int n, CString p, bool &Done)
{
	CString s = "";
	for (int k = 0; k <= n; k++)
	{
		int i = p.Find(';');

		Done = i < 0;

		if (Done) i = p.GetLength();

		s = p.Mid(0, i);

		if (k<n && p.GetLength() > 0) p = p.Mid(i + 1, p.GetLength() - i - 1);
	}
	return s;
}



