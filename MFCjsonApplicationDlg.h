
// MFCjsonApplicationDlg.h : header file
//

#pragma once


// CMFCjsonApplicationDlg dialog
class CMFCjsonApplicationDlg : public CDialogEx
{
// Construction
public:
	CMFCjsonApplicationDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCJSONAPPLICATION_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_display;
	afx_msg void OnEnChangeEditDisplay();
	CEdit m_disp;
	CMFCEditBrowseCtrl m_browsepath;
	afx_msg void OnEnChangeMfceditbrowse();
	afx_msg void OnBnClickedExport();
};
