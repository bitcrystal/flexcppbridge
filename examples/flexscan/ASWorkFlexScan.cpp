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

#include "ASWorkFlexScan.h"
#include "Logger.h"
#include "FlexBridge.h"
#include "ASObject.h"
#include <sstream>
#include <windows.h>
#include "BridgeException.h"
#include "TwainCpp.h"
#include "FlexCall.h"
#define WM_FLEXSCAN (WM_USER + 104)
#define WM_FLEXSCAN_COMPLETE (WM_USER + 106)

static CTwain* g_pTwain = NULL;

CASWorkFlexScan::CASWorkFlexScan(void)
{
	m_pTwain = NULL;
}

CASWorkFlexScan::CASWorkFlexScan(void* pTwain)
{
	m_pTwain = pTwain;
}

CASWorkFlexScan::~CASWorkFlexScan(void)
{
	//delete g_pTwain;
}

static void ScanClick(CASObject& obj, CFlexBridge* pBridge)
{
    if ( pBridge )
    {
        CASObject oRootObj;
        
        pBridge->Root(oRootObj);
        
        CASObject oRunText;
        CASValue oRunValue;
        
        oRootObj.Call("getscanButton").Call("setenabled", false);
		SendMessage((HWND)CFlexCall::m_pDlg, WM_FLEXSCAN, (WPARAM)0, (LPARAM)0);	
    }
} 


void CASWorkFlexScan::Worker()
{
    CFlexBridge* pBridge = (CFlexBridge*)m_pBridge;    
    CASObject oRootObj;
    
    pBridge->Root(oRootObj);
    
    if ( oRootObj.GetInstance() )
    {
        
        try
        {
			//g_pTwain = (CTwain*)m_pTwain;
		    /* add a mouseClick event listener for the button */
			oRootObj.Call("getscanButton").Call("addEventListener", "click", ScanClick); 
            
        }
        catch ( CBridgeException& oErrBridge )
        {
            LOG("Bridge Exception Caught: \"" << oErrBridge.what() << "\"");            
        }
    }
    
}

CASWork* CASWorkFlexScan::Clone()
{
    return (CASWork*)(new CASWorkFlexScan(m_pTwain));
}
