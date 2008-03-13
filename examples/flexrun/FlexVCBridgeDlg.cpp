/*
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is the Flex C++ Bridge.
 *
 * The Initial Developer of the Original Code is
 * Anirudh Sasikumar (http://anirudhs.chaosnet.org/).
 * Portions created by the Initial Developer are Copyright (C) 2008
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
*/

// FlexVCBridgeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FlexVCBridgeAct.h"
#include "FlexVCBridgeDlg.h"
#include "Logger.h"
#include "FlexCallQueue.h"
#include "FlashIDManager.h"
#include "BridgeManager.h"
#include "ASWorkFlexRun.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CFlexVCBridgeDlg dialog




CFlexVCBridgeDlg::CFlexVCBridgeDlg(CWnd* pParent /*=NULL*/)
: CDialog(CFlexVCBridgeDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    /* register the flash instances as well */
    CFlashIDManager::AddFlashPtr((void *)&m_Shockwave);
    //CFlashIDManager::AddFlashPtr((void *)&m_Shockwave2);
}

void CFlexVCBridgeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SHOCKWAVEFLASH1, m_Shockwave);
	//DDX_Control(pDX, IDC_SHOCKWAVEFLASH2, m_Shockwave2);
}

BEGIN_MESSAGE_MAP(CFlexVCBridgeDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
ON_MESSAGE(WM_FLEXCALLBACK, OnFlexCallback)
ON_MESSAGE(WM_FLEXCALLBACK_RETURN, OnFlexCallbackRet)
//ON_MESSAGE(DM_GETDEFID, OnGetDefID)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CFlexVCBridgeDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_MYOK, &CFlexVCBridgeDlg::OnBnClickedMyok1)
	ON_BN_CLICKED(IDCANCEL, &CFlexVCBridgeDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_MYCANCEL, &CFlexVCBridgeDlg::OnBnClickedMycancel)
END_MESSAGE_MAP()


// CFlexVCBridgeDlg message handlers

BOOL CFlexVCBridgeDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    
    // Add "About..." menu item to system menu.
    
    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);
    
    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
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
    CFlexCall::Init((void*)m_hWnd);
    CASWorkFlexRun* pFlexRun = new CASWorkFlexRun();
    CBridgeManager::RegisterDefaultCallbacks(pFlexRun);
        
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFlexVCBridgeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	LOG("SC_CLOSE " << SC_CLOSE);
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else if ((nID & 0xFFF0)== SC_CLOSE )
    {
        OnOK();
    }
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
	 
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFlexVCBridgeDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFlexVCBridgeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BEGIN_EVENTSINK_MAP(CFlexVCBridgeDlg, CDialog)
	ON_EVENT(CFlexVCBridgeDlg, IDC_SHOCKWAVEFLASH1, 197, CFlexVCBridgeDlg::FlashCallShockwaveflash1, VTS_BSTR)
//	ON_EVENT(CFlexVCBridgeDlg, IDC_SHOCKWAVEFLASH2, 197, CFlexVCBridgeDlg::FlashCallShockwaveflash2, VTS_BSTR)
END_EVENTSINK_MAP()

void CFlexVCBridgeDlg::FlashCallShockwaveflash1(LPCTSTR request)
{
    static int iFlashID = -1;
    
    if ( iFlashID == -1 )
        iFlashID = CFlashIDManager::GetIDFlashPtr((void*)&m_Shockwave);

    FlashCallHandler(iFlashID, request);
}


/* There are 2 methods in which communication happens b/w flash /
 * vc. In the first way, Flex initiates a call via ExternalInterface
 * and then gets an async callback from C++ on the registered
 * function. The other way is for C++ to initiate a communication to
 * Flex and wait for an async callback. This function handles the
 * second way.  */
LRESULT CFlexVCBridgeDlg::OnFlexCallback(WPARAM wParam, LPARAM lParam)
{
    std::string* msg = (std::string*)wParam;
    if ( msg == NULL )
    {
        LOG("FlexCallback got a msg with NULL wparam");
        return -1;
    }
    LOGDEBUG("FlexCallback got " << msg->c_str());

    /* std::string dmsg = "VC Replies: "; */
    /* dmsg += (*msg); */
    /* MessageBox(dmsg.c_str()); */

    CShockwaveflash* pShockwave = NULL;

    int iFlashID = (int)lParam;

    pShockwave = (CShockwaveflash*)CFlashIDManager::GetFlashPtr(iFlashID);

    if ( pShockwave == NULL )
        pShockwave = &m_Shockwave;

    try
    {
        /* make response to async call back into flex */
        pShockwave->CallFunction(msg->c_str());
    }
    catch(...)
    {
        LOG("m_Shockwave.CallFunction threw exception!");
    }

    delete msg;
    return 0;
}

/* There are 2 methods in which communication happens b/w flash /
 * vc. In the first way, Flex initiates a call via ExternalInterface
 * and then gets an async callback from C++ on the registered
 * function. The other way is for C++ to initiate a communication to
 * Flex and wait for an async callback. This function handles the
 * second way.  It initiates the call from the c++ side, the return
 * value of the function has the response. */
LRESULT CFlexVCBridgeDlg::OnFlexCallbackRet(WPARAM wParam, LPARAM lParam)
{
    std::string* msg = (std::string*)wParam;
    if ( msg == NULL )
    {
        LOG("FlexCallback got a msg with NULL wparam");
        return -1;
    }
    LOGDEBUG("FlexCallback got " << msg->c_str());

    CShockwaveflash* pShockwave = NULL;

    CASResponse* pASResp = (CASResponse*)lParam;
    

    pShockwave = (CShockwaveflash*)CFlashIDManager::GetFlashPtr(pASResp->m_iFlashID);

    if ( pShockwave == NULL )
        pShockwave = &m_Shockwave;

    try
    {
        std::string resFn = "<invoke name=\"dude\"><arguments>"+pShockwave->CallFunction(msg->c_str()) + "</arguments></invoke>";
        CFlexCallQueue::AddResponse(pASResp->m_uiID, resFn);

        /* std::string fnout; */
        /* std::vector<CASObject> objs; */
        /* CFlexCallQueue::ParseFlexXML(resFn, fnout, objs); */
		
        /* for ( unsigned int i = 0; i < objs.size(); i++ ) */
        /* { */
        /*     objs[i].Dump(); */
        /* } */

        /* LOG("Response from function: " << resFn); */
    }
    catch(...)
    {
        LOG("m_Shockwave.CallFunction threw exception!");
    }

    delete msg;
    return 0;
}

/* Handle case when multiple shockwave embeds are there. */
void CFlexVCBridgeDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	m_Shockwave.SendMessageA(WM_KEYDOWN, VK_RETURN, 0);
}

void CFlexVCBridgeDlg::OnBnClickedMyok1()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CFlexVCBridgeDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	m_Shockwave.SendMessageA(WM_KEYDOWN, VK_ESCAPE, 0);
}

void CFlexVCBridgeDlg::OnBnClickedMycancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CFlexVCBridgeDlg::FlashCallHandler(int iFlashID, LPCTSTR request)
{
    CASRequest oRequest;
    
    oRequest.m_iFlashID = iFlashID;
    
    // TODO: Add your message handler code here
    oRequest.m_sRequest = request;

    LOGDEBUG("Got request " << oRequest.m_sRequest );
    //std::cout << "The request is " << sRequest << std::endl;
    CFlexCallQueue::AddCall(oRequest);	
}
