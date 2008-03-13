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

#include "ASWorkSample2.h"
#include "Logger.h"
#include "FlexBridge.h"
#include "ASObject.h"

CASWorkSample2::CASWorkSample2(void)
{
}

CASWorkSample2::~CASWorkSample2(void)
{
}

void CASWorkSample2::Worker()
{
    CFlexBridge* pBridge = (CFlexBridge*)m_pBridge;

    LOG("Sample worker 2 called");
    CASObject oRootObj;

    pBridge->Root(oRootObj);

    if ( oRootObj.GetInstance() )
    {
        CASObject oWidthObj;
        CASObject oActWidth;
        oActWidth.SetString(std::string("Test String2!"));
        std::vector<CASObject> vArgArgs;
        vArgArgs.push_back(oActWidth);
        oActWidth.SetString(std::string("Another message2"));
        vArgArgs.push_back(oActWidth);
        
        oActWidth.SetString(std::string("Custom Object2"));
        std::map<std::string, CASObject> vArgMap;
        
        vArgMap.insert(std::make_pair(std::string("Custom"), oActWidth));
        oActWidth.SetObject(vArgMap);
        vArgArgs.push_back(oActWidth);
        vArgArgs.push_back(oRootObj);
        //pBridge->Call(oRootObj, "getWidth", oWidthObj);
        pBridge->Call(oRootObj, "testAlert", vArgArgs,oWidthObj);
        LOG("Width obj is: " );
        oWidthObj.Dump();
        //oRootObj.GetInstance()->Call("setWidth", 0);
    }

}


CASWork* CASWorkSample2::Clone()
{
    return (CASWork*)(new CASWorkSample2());
}
