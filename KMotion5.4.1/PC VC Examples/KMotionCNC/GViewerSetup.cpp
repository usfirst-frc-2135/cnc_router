// GViewerSetup.cpp : implementation file
/*********************************************************************/
/*         Copyright (c) 2003-2006  DynoMotion Incorporated          */
/*********************************************************************/

#include "stdafx.h"
#include "GViewerSetup.h"
#include "MainFrm.h"
#include "OpenDlg.h"
#include "CPreviewFileDialog.h"
#include "DoPreview.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGViewerSetup dialog


CGViewerSetup::CGViewerSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CGViewerSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGViewerSetup)
	m_ToolShapeFile = _T("");
	m_BoxX = 10.0;
	m_BoxY = 10.0;
	m_BoxZ = 5.0;
	m_BoxOffsetX = 0.0;
	m_BoxOffsetY = 0.0;
	m_BoxOffsetZ = 0.0;
	m_AxisSize = 1.0f;
	m_ToolSize = 1.0f;
	m_ToolOffX = 0.0f;
	m_ToolOffY = 0.0f;
	m_ToolOffZ = 0.0f;
	m_IncludeA = FALSE;
	m_IncludeB = FALSE;
	m_IncludeC = FALSE;
	m_IncludeToolAngles = FALSE;
	//}}AFX_DATA_INIT
}


void CGViewerSetup::DoDataExchange(CDataExchange* pDX)
{
	
	CDialog::DoDataExchange(pDX);

	
	//{{AFX_DATA_MAP(CGViewerSetup)
	DDX_Text(pDX, IDC_ToolShapeFile, m_ToolShapeFile);
	DDX_TextMM(pDX, IDC_BoxX, m_BoxX, ConfigUnitsMM);
	DDV_MinMaxDouble(pDX, m_BoxX, 0., 1000000000.);
	DDX_TextMM(pDX, IDC_BoxY, m_BoxY, ConfigUnitsMM);
	DDV_MinMaxDouble(pDX, m_BoxY, 0., 1000000000.);
	DDX_TextMM(pDX, IDC_BoxZ, m_BoxZ, ConfigUnitsMM);
	DDV_MinMaxDouble(pDX, m_BoxZ, 0., 1000000000.);
	DDX_TextMM(pDX, IDC_BoxOffsetX, m_BoxOffsetX, ConfigUnitsMM);
	DDV_MinMaxDouble(pDX, m_BoxOffsetX, -1000000000., 1000000000.);
	DDX_TextMM(pDX, IDC_BoxOffsetY, m_BoxOffsetY, ConfigUnitsMM);
	DDV_MinMaxDouble(pDX, m_BoxOffsetY, -1000000000., 1000000000.);
	DDX_TextMM(pDX, IDC_BoxOffsetZ, m_BoxOffsetZ, ConfigUnitsMM);
	DDV_MinMaxDouble(pDX, m_BoxOffsetZ, -1000000000., 1000000000.);
	DDX_Text(pDX, IDC_AxisSize, m_AxisSize);
	DDV_MinMaxDouble(pDX, m_AxisSize, 0.f, 1.e+006);
	DDX_Text(pDX, IDC_ToolSize, m_ToolSize);
	DDV_MinMaxDouble(pDX, m_ToolSize, 0.f, 1.e+006);
	DDX_Text(pDX, IDC_ToolOffX, m_ToolOffX);
	DDV_MinMaxDouble(pDX, m_ToolOffX, -1e6, 1.e+006);
	DDX_Text(pDX, IDC_ToolOffY, m_ToolOffY);
	DDV_MinMaxDouble(pDX, m_ToolOffY, -1e6, 1.e+006);
	DDX_Text(pDX, IDC_ToolOffZ, m_ToolOffZ);
	DDV_MinMaxDouble(pDX, m_ToolOffZ, -1e6, 1.e+006);
	DDX_Check(pDX, IDC_IncludeA, m_IncludeA);
	DDX_Check(pDX, IDC_IncludeB, m_IncludeB);
	DDX_Check(pDX, IDC_IncludeC, m_IncludeC);
	DDX_Check(pDX, IDC_IncludeToolAngles, m_IncludeToolAngles);
	//}}AFX_DATA_MAP


}


BEGIN_MESSAGE_MAP(CGViewerSetup, CDialog)
	//{{AFX_MSG_MAP(CGViewerSetup)
	ON_BN_CLICKED(IDC_BrowseToolShapeFile, OnBrowseToolShapeFile)
	ON_BN_CLICKED(ID_HELP, OnHelp)
	ON_BN_CLICKED(IDC_mm, Onmm)
	ON_BN_CLICKED(IDC_inch, Oninch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGViewerSetup message handlers

void CGViewerSetup::OnBrowseToolShapeFile() 
{
	CString IDir=TheFrame->MainPathRoot + TOOL_IMAGE_SUB_DIR;

//tktk	CPersistOpenDlg FileDlg (TRUE, L".wrl", 
//tktk		TheFrame->GCodeDlg.InitialFile(m_ToolShapeFile, TOOL_IMAGE_SUB_DIR, L"Tool.wrl"),
//tktk		OFN_FILEMUSTEXIST | OFN_ENABLESIZING, 
//tktk		/*TRAN*/L"Tool Shape VRML Files (*.wrl)|*.wrl|All Files (*.*)|*.*||");

	CDoPreview preview;

	CPreviewFileDialog FileDlg(&preview, TRUE, L".wrl", m_ToolShapeFile, OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR | OFN_PATHMUSTEXIST,
		/*TRAN*/L"VRML Image Files (*.wrl)|*.wrl|All Files (*.*)|*.*||");


	FileDlg.m_ofn.lpstrInitialDir = IDir;
	FileDlg.m_ofn.lpstrTitle=/*TRAN*/L"Select Tool Shape VRML File";

	if (FileDlg.DoModal() == IDOK)
	{
		m_ToolShapeFile=TheFrame->GCodeDlg.StripPathMatch(FileDlg.GetPathName(), TOOL_IMAGE_SUB_DIR);
		UpdateData(FALSE);
	}
}


void CGViewerSetup::OnOK() 
{
	if (!UpdateData()) return;

	
	CDialog::OnOK();
}

BOOL CGViewerSetup::OnInitDialog() 
{
	CDialog::OnInitDialog();

#ifdef KMOTION_CNC
	TheFrame->GCodeDlg.DisableKeyJog();
#endif
	if (ConfigUnitsMM)
		CheckRadioButton(IDC_mm, IDC_inch, IDC_mm);
	else
		CheckRadioButton(IDC_mm, IDC_inch, IDC_inch);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CGViewerSetup::OnHelp() 
{
	TheFrame->HelpDlg.Show("KMotionCNC\\GViewerSetup.htm");
}

void CGViewerSetup::Onmm()
{
	UpdateData();
	ConfigUnitsMM = TRUE;
	CheckRadioButton(IDC_mm, IDC_inch, IDC_mm);
	UpdateData(FALSE);
	SetStatics();
}

void CGViewerSetup::Oninch()
{
	UpdateData();
	ConfigUnitsMM = FALSE;
	CheckRadioButton(IDC_mm, IDC_inch, IDC_inch);
	UpdateData(FALSE);
	SetStatics();
}

void CGViewerSetup::SetStatics()
{
	if (ConfigUnitsMM)
	{
		SetDlgItemText(IDC_StaticBoxSizeUnits, L"mm");
		SetDlgItemText(IDC_StaticBoxOffsetUnits, L"mm");
	}
	else
	{
		SetDlgItemText(IDC_StaticBoxSizeUnits, L"inches");
		SetDlgItemText(IDC_StaticBoxOffsetUnits, L"Inches");
	}
}


