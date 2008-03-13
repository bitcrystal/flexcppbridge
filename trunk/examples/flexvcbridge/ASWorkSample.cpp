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

#include "ASWorkSample.h"
#include "Logger.h"
#include "FlexBridge.h"
#include "ASObject.h"
#include <sstream>
#include <windows.h>
#include "BridgeException.h"

CASWorkSample::CASWorkSample(void)
{
}

CASWorkSample::~CASWorkSample(void)
{
}

void SampleCallback(CASObject& obj, CFlexBridge* pBridge)
{
    if ( pBridge )
    {
        CASObject oRootObj;
        pBridge->Root(oRootObj);
	oRootObj.Call("getpanel1").Call("settitle", "Callback from VC"); 
    }
} 

void SampleCallback2(CASObject& obj, CFlexBridge* pBridge)
{
    if ( pBridge )
    {
        CASObject oRootObj;
        
        pBridge->Root(oRootObj);
	std::ostringstream ostr;
	ostr << "CPP Func called at ";
	ostr << GetTickCount();
	oRootObj.Call("getpanel1").Call("settitle", ostr.str()); 
    }
} 


void CASWorkSample::Worker()
{
    CFlexBridge* pBridge = (CFlexBridge*)m_pBridge;
    
    LOG("Sample worker called");
    CASObject oRootObj;
    
    pBridge->Root(oRootObj);
    
    if ( oRootObj.GetInstance() )
    {
        
        try
        {
            
            CASObject oAlertObj = pBridge->ClassRef("mx.controls.Alert");
            
            oAlertObj.Call("show", "StaticClassCalled");
            
            CASObject oFlashEventObj = pBridge->ClassRef("flash.events.MouseEvent");
            
            /* Call our test function in FlexVCBridge with 2 strings, a
             * custom anonymous object, the root object and a cpp function
             * callback. The function returns the root object so that it's
             * value can be used to chain the function call. */
            
            /* Way 1 - Pass args using overloaded call. But that has a
             * limitation of upto 5 args (5 can be increased to any bumber
             * though) */
            
            /* building an anonymous object */
            std::map<std::string, CASObject> vArgMap1;
            vArgMap1["Custom"] =  "String member of a custom object";
            
            oRootObj.Call("testAlert1", "String message 1", "Another String message", vArgMap1, oRootObj, SampleCallback).Call("getheight").Dump();
            
            /* add a mouseDown event listener for the piechart */
            oRootObj.Call("getpie1").Call("addEventListener", oFlashEventObj.Call("getMOUSE_DOWN"), SampleCallback2); 
            //oRootObj.Call("getPie1").Call("addEventListener", , SampleCallback2); 
            
            /* Way 2 - Pass args as vector */
            
            std::vector<CASObject> vArgArgs;
            
            vArgArgs.push_back("String message from way 2");
            vArgArgs.push_back("Another string message from way 2");
            
            std::map<std::string, CASObject> vArgMap;
            vArgMap["Custom"] =  "String member of custom object 2";
            
            vArgArgs.push_back(vArgMap);
            vArgArgs.push_back(oRootObj);
            
            LOG("Height obj is: " );
            oRootObj.CallArg("testAlert", &vArgArgs).Call("getheight").Dump();
            
            /* Remove first item in arraycollection and change the value
             * of the first item. */
            
            /* Here, the use of CallEx is shown. CallEx can take any
             * number of arguments. But the only condition is that the
             * last argument must be NULL (va_arg terminator). Otherwise
             * the results are unpredictable. */
            oRootObj.Call("getexpenses").CallEx("removeItemAt", &CASObject(2), NULL);
            oRootObj.Call("getexpenses").Call("refresh");
            
            CASObject oTaxes = oRootObj.Call("getexpenses").Call("getItemAt", 0);
            oTaxes.GetObjectMap()["Amount"]= 100;
            
            oRootObj.Call("getexpenses").Call("setItemAt", oTaxes, 0);
            
            /* Datagrid sample */
            
            /* create a custom class */
            CASObject oDGrid = pBridge->Create("mx.controls.DataGrid");
            
            /* add it as a child to our root object */
            oRootObj.Call("addChild", oDGrid);
            
            /* build columns and set that as the dataprovider */
            CASObject oDGCol1 = pBridge->Create("mx.controls.dataGridClasses.DataGridColumn");
            oDGCol1.Call("setdataField", "apples");
            
            CASObject oDGCol2 = pBridge->Create("mx.controls.dataGridClasses.DataGridColumn");
            oDGCol2.Call("setdataField", "oranges");
            
            /* DataGrid.columns is an Array. vArgArgs is used as that
             * array here. */
            vArgArgs.clear();
            
            vArgArgs.push_back(oDGCol1);
            vArgArgs.push_back(oDGCol2);
            
            oDGrid.Call("setcolumns", vArgArgs);
            oDGrid.Call("setwidth", 300);
            
            /* Here, the dataProvider Array is built */
            
            vArgMap.clear();
            vArgArgs.clear();
            
            vArgMap["apples"] = 12;
            vArgMap["oranges"] = 32;
            vArgArgs.push_back(vArgMap);
            
            vArgMap["apples"] = 7;
            vArgMap["oranges"] = 47;
            vArgArgs.push_back(vArgMap);
            
            vArgMap["apples"] = 14;
            vArgMap["oranges"] = 21;
            vArgArgs.push_back(vArgMap);
            
            oDGrid.Call("setdataProvider", vArgArgs);
        }
        catch ( CBridgeException& oErrBridge )
        {
            LOG("Bridge Exception Caught: \"" << oErrBridge.what() << "\"");            
        }
    }
    
}

CASWork* CASWorkSample::Clone()
{
    return (CASWork*)(new CASWorkSample());
}
