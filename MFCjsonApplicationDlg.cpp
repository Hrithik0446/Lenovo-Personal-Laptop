
// MFCjsonApplicationDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MFCjsonApplication.h"
#include "MFCjsonApplicationDlg.h"
#include "afxdialogex.h"
#include <fstream>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCjsonApplicationDlg dialog



CMFCjsonApplicationDlg::CMFCjsonApplicationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCJSONAPPLICATION_DIALOG, pParent)
	, m_display(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCjsonApplicationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DISPLAY, m_display);
	DDX_Control(pDX, IDC_EDIT_DISPLAY, m_disp);
	DDX_Control(pDX, IDC_MFCEDITBROWSE, m_browsepath);
}

BEGIN_MESSAGE_MAP(CMFCjsonApplicationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT_DISPLAY, &CMFCjsonApplicationDlg::OnEnChangeEditDisplay)
	ON_EN_CHANGE(IDC_MFCEDITBROWSE, &CMFCjsonApplicationDlg::OnEnChangeMfceditbrowse)
	ON_BN_CLICKED(IDC_EXPORT, &CMFCjsonApplicationDlg::OnBnClickedExport)
END_MESSAGE_MAP()


// CMFCjsonApplicationDlg message handlers

BOOL CMFCjsonApplicationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCjsonApplicationDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCjsonApplicationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCjsonApplicationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void searchAndReplace(std::string& value, std::string const& search, std::string const& replace)
{
	std::string::size_type  next;
	for (next = value.find(search);        // Try and find the first match
		next != std::string::npos;        // next is npos if nothing was found
		next = value.find(search, next)    // search for the next match starting after
		// the last match that was found.
		)
	{
		// Inside the loop. So we found a match.
		value.replace(next, search.length(), replace);   // Do the replacement.
		next += replace.length();                      // Move to just after the replace
		// This is the point were we start
		// the next search from. 
	}
}
string k;
string header = "ISOGEN-FILES        ISOGEN.FLS\nUNITS-BORE          INCH\nUNITS-CO-ORDS       MM\nUNITS-BOLT-LENGTH   MM\nUNITS-BOLT-DIA      MM\nPIPELINE-REFERENCE  P-1\n";
std::string convertJsonToPcf(const json& jsonData) {
	std::string pcfFormat;

	for (auto it = jsonData.begin(); it != jsonData.end(); it++) {
		if (!pcfFormat.empty()){
			pcfFormat += "\n";
		}
		std::string value = it.value().get<std::string>();
		size_t commaPos = value.find(',');
		while (commaPos != std::string::npos) {
			value.replace(commaPos, 1, "   ");
			//if (value.find(".") < value.size()) {
			//	value.replace(commaPos, 1, "0000   ");
			//}
			//else {
				value.replace(commaPos, 1, "   ");
			//}
			commaPos = value.find(',', commaPos + 4); 
		}
		if (it.key() == "CP1") {
			k = "center-point";
			pcfFormat += "\t" + k + "\t" + value;
		}
		else if (it.key() == "WP1" || it.key() == "WP2") {
			k = it.key();
			k = "end-point";
			pcfFormat += "\t" + k + "\t\t" + value;
		}
		else {
			if (it.key() == "Angle") {
				pcfFormat += "\t" + it.key() + "\t\t\t" + value;
			}
			if (it.key() == "Material") {
				pcfFormat += "\t" + it.key() + "\t    " + value;
			}
			if (it.key() == "pcf") {
				pcfFormat += "\t" + it.key() + "\t\t\t\t" + value;
			}
		}
	}
	return pcfFormat;
}

void CMFCjsonApplicationDlg::OnEnChangeEditDisplay()
{
}

void CMFCjsonApplicationDlg::OnEnChangeMfceditbrowse()
{
	CString fileNameInC;
	m_browsepath.GetWindowTextW(fileNameInC);
	std::wstring s((LPCTSTR)fileNameInC);
	std::string fileName(s.begin(), s.end());
	searchAndReplace(fileName, "\\", "\\\\");
	std::ifstream file(fileName);
	nlohmann::json data = nlohmann::json::parse(file);
	int count = 0;
	while (count < data["components"].size() - 2) {
		if (data["components"][count]["name"] != "ELBOW") {
		}
		else {
			std::string nameelbow = data["components"][count].value("name", "not found");
			std::string pcfFormat = nameelbow;
			//+"\r\n\r\n" + convertJsonToPcf(data["components"][0]["iProperties"][0]);
			wstring pcfFormat2;
			pcfFormat2.assign(pcfFormat.begin(), pcfFormat.end());
			LPCTSTR data3 = pcfFormat2.c_str();
			m_disp.SetWindowTextW(data3);
		}
		count++;
	}
	//std::string nameelbow = data["components"][count].value("name", "not found");
	//std::string pcfFormat = nameelbow;
	////+"\r\n\r\n" + convertJsonToPcf(data["components"][0]["iProperties"][0]);
	//wstring pcfFormat2;
	//pcfFormat2.assign(pcfFormat.begin(), pcfFormat.end());
	//LPCTSTR data3 = pcfFormat2.c_str();
 //   m_disp.SetWindowTextW(data3);
	/*std::ifstream file("invFile3.json");
	json data = json::parse(file);*/
	/*if (!file) {
		AfxMessageBox(L"not read");
	}*/
	//int count = 0;
	//while (count <= 5) {
		//string pcf1 = data["components"][0]["iProperties"][0]["pcf"].value("pcf", "not found");
		//if (pcf1 == "yes") {
			//cout << data["components"][count]["iProperties"][0] << endl;
			/*string data1 = data["components"][0]["iProperties"][0]["pcf"];
			wstring data2;
			data2.assign(data1.begin(), data1.end());
			LPCTSTR data3 = data2.c_str();
			m_disp.SetWindowTextW(data3);*/
		//}
		// //count++;
		//}
	/*}*/
	//int countsa1 = 0;
	//int countsa2 = 0;
	//while (countsa1 <= 2) {
	//	string pcf2 = data["components"][8]["Components"][countsa1]["iProperties"][0].value("pcf", "not found");
	//	if (pcf2 == "yes") {
	//		string data1 = data["components"][8]["Components"][countsa1]["iProperties"][0] + "\n";
	//		wstring data2;
	//		data2.assign(data1.begin(), data1.end());
	//		LPCTSTR data3 = data2.c_str();
	//		m_disp.SetWindowTextW(data3);
	//	}
	//	countsa1++;
	//}
	//while (countsa2 <= 4) {
	//	string pcf3 = data["components"][9]["Components"][countsa2]["iProperties"][0].value("pcf", "not found");
	//	if (pcf3 == "yes") {
	//		string data1 = data["components"][9]["Components"][countsa2]["iProperties"][0];
	//		wstring data2;
	//		data2.assign(data1.begin(), data1.end());
	//		LPCTSTR data3 = data2.c_str();
	//		m_disp.SetWindowTextW(data3);
	//	}
	//	countsa2++;
	//}	
}


void CMFCjsonApplicationDlg::OnBnClickedExport()
{
	string pcfFormat;
	CString fileNameInC;
	m_browsepath.GetWindowTextW(fileNameInC);
	std::wstring s((LPCTSTR)fileNameInC);
	std::string fileName(s.begin(), s.end());
	searchAndReplace(fileName, "\\", "\\\\");
	std::ifstream inputFile(fileName);
	nlohmann::json data = nlohmann::json::parse(inputFile);
	if (!inputFile) {
		AfxMessageBox(L"cant read file");
	}
	//json jsonData = json::parse(jsonStr)
	/*string nameelbow = data["components"][].value("name", "not found");*/
	//cout << nameelbow << endl;
	string atrbs = "\tskey\t\t\t"+data["components"][0].value("skey", "none");
	string ppspec = "\tpiping-spec\t\t"+data["components"][0].value("piping-spec","none");
	int count = 0;
		while (count < data["components"].size() - 2) {
			if (data["components"][count]["name"] != "ELBOW") {
			}
			else {
				string nameelbow = data["components"][count].value("name", "not found");
				std::string pcfFormat = header + ppspec + "\n" + nameelbow + "\n" + convertJsonToPcf(data["components"][count]["iProperties"][0]) + "\n" + atrbs;
				transform(pcfFormat.begin(), pcfFormat.end(), pcfFormat.begin(), ::toupper);
				std::ofstream outputFile("ELBOW.pcf");
				if (!outputFile) {
					AfxMessageBox(L"cant read file");
				}
				outputFile << pcfFormat;
				outputFile.close();
			}
			count++;
		}
	// Write to the piping component file
	/*std::ofstream outputFile("ELBOW.pcf");
	if (!outputFile) {
		AfxMessageBox(L"cant read file");
	}
	outputFile << pcfFormat;
	outputFile.close();*/
}

