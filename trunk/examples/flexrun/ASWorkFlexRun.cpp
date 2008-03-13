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

#include "ASWorkFlexRun.h"
#include "Logger.h"
#include "FlexBridge.h"
#include "ASObject.h"
#include <sstream>
#include <windows.h>
#include "BridgeException.h"
#include <shlobj.h>
#include <shlwapi.h>
#include <objbase.h>


CASWorkFlexRun::CASWorkFlexRun(void)
{
}

CASWorkFlexRun::~CASWorkFlexRun(void)
{
}

static void RunClick(CASObject& obj, CFlexBridge* pBridge)
{
    if ( pBridge )
    {
        SHELLEXECUTEINFO ShExecInfo;
        
        CASObject oRootObj;
        
        pBridge->Root(oRootObj);
        
        CASObject oRunText;
        CASValue oRunValue;
        
        oRootObj.Call("getrunButton").Call("setenabled", false);
        oRunText = oRootObj.Call("getrunText").Call("gettext");
        oRunText.GetValue(oRunValue);
        
        ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
        ShExecInfo.fMask = NULL;
        ShExecInfo.hwnd = NULL;
        ShExecInfo.lpVerb = NULL;
        ShExecInfo.lpFile = oRunValue.m_sVal->c_str();
        ShExecInfo.lpParameters = NULL;
        ShExecInfo.lpDirectory = NULL;
        ShExecInfo.nShow = SW_MAXIMIZE;
        ShExecInfo.hInstApp = NULL;
        
        ShellExecuteEx(&ShExecInfo);
        oRootObj.Call("getrunButton").Call("setenabled", true);
        
	//oRootObj.Call("getMainPanel").Call("setTitle", *oRunValue.m_sVal); 
    }
} 


void CASWorkFlexRun::Worker()
{
    CFlexBridge* pBridge = (CFlexBridge*)m_pBridge;    
    CASObject oRootObj;
    
    pBridge->Root(oRootObj);
    
    if ( oRootObj.GetInstance() )
    {
        
        try
        {
            /* add a mouseClick event listener for the button */
            oRootObj.Call("getrunButton").Call("addEventListener", "click", RunClick); 
            
        }
        catch ( CBridgeException& oErrBridge )
        {
            LOG("Bridge Exception Caught: \"" << oErrBridge.what() << "\"");            
        }
    }
    
}

CASWork* CASWorkFlexRun::Clone()
{
    return (CASWork*)(new CASWorkFlexRun());
}
