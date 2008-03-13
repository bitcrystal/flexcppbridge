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

// FlexVCBridgeDlg.h : header file
//

#pragma once
#include "shockwaveflash.h"


// CFlexVCBridgeDlg dialog
class CFlexVCBridgeDlg : public CDialog
{
// Construction
public:
	CFlexVCBridgeDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_FLEXVCBRIDGEACT_DIALOG };

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
        afx_msg LRESULT OnFlexCallback(WPARAM wParam, LPARAM lParam);
        afx_msg LRESULT OnFlexCallbackRet(WPARAM wParam, LPARAM lParam);
		//afx_msg LRESULT CFlexVCBridgeDlg::OnGetDefID(WPARAM wp, LPARAM lp); 

	DECLARE_MESSAGE_MAP()
public:
	CShockwaveflash m_Shockwave;
	DECLARE_EVENTSINK_MAP()
	void FlashCallShockwaveflash1(LPCTSTR request);
	void FlashCallShockwaveflash2(LPCTSTR request);

        void FlashCallHandler(int iFlashID, LPCTSTR request);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedMyok1();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedMycancel();
	CShockwaveflash m_Shockwave2;
};
