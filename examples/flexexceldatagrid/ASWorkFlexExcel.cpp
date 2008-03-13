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

#include "ASWorkFlexExcel.h"
#include "Logger.h"
#include "FlexBridge.h"
#include "ASObject.h"
#include <sstream>
#include <windows.h>
#include "BridgeException.h"
#include <shlobj.h>
#include <shlwapi.h>
#include <objbase.h>

//for excel integration, copied from MSDN docs

#include <ole2.h> // OLE2 Definitions

// AutoWrap() - Automation helper function...
HRESULT AutoWrap(int autoType, VARIANT *pvResult, IDispatch *pDisp, LPOLESTR ptName, int cArgs...) {
    // Begin variable-argument list...
    va_list marker;
    va_start(marker, cArgs);
    
    if(!pDisp) {
        MessageBox(NULL, "NULL IDispatch passed to AutoWrap()", "Error", 0x10010);
        _exit(0);
    }
    
    // Variables used...
    DISPPARAMS dp = { NULL, NULL, 0, 0 };
    DISPID dispidNamed = DISPID_PROPERTYPUT;
    DISPID dispID;
    HRESULT hr;
    char buf[200];
    char szName[200];
    
    
    // Convert down to ANSI
    WideCharToMultiByte(CP_ACP, 0, ptName, -1, szName, 256, NULL, NULL);
    
    // Get DISPID for name passed...
    hr = pDisp->GetIDsOfNames(IID_NULL, &ptName, 1, LOCALE_USER_DEFAULT, &dispID);
    if(FAILED(hr)) {
        sprintf_s(buf, "IDispatch::GetIDsOfNames(\"%s\") failed w/err 0x%08lx", szName, hr);
        MessageBox(NULL, buf, "AutoWrap()", 0x10010);
        _exit(0);
        return hr;
    }
    
    // Allocate memory for arguments...
    VARIANT *pArgs = new VARIANT[cArgs+1];
    // Extract arguments...
    for(int i=0; i<cArgs; i++) {
        pArgs[i] = va_arg(marker, VARIANT);
    }
    
    // Build DISPPARAMS
    dp.cArgs = cArgs;
    dp.rgvarg = pArgs;
    
    // Handle special-case for property-puts!
    if(autoType & DISPATCH_PROPERTYPUT) {
        dp.cNamedArgs = 1;
        dp.rgdispidNamedArgs = &dispidNamed;
    }
    
    // Make the call!
    hr = pDisp->Invoke(dispID, IID_NULL, LOCALE_SYSTEM_DEFAULT, autoType, &dp, pvResult, NULL, NULL);
    if(FAILED(hr)) {
        sprintf_s(buf, "IDispatch::Invoke(\"%s\"=%08lx) failed w/err 0x%08lx", szName, dispID, hr);
        MessageBox(NULL, buf, "AutoWrap()", 0x10010);
        _exit(0);
        return hr;
    }
    // End variable-argument section...
    va_end(marker);
    
    delete [] pArgs;
    
    return hr;
}


CASWorkFlexExcel::CASWorkFlexExcel(void)
{
}

CASWorkFlexExcel::~CASWorkFlexExcel(void)
{
}

static void RunClick(CASObject& obj, CFlexBridge* pBridge)
{
    if ( pBridge )
    {
        CASObject oRootObj;
        
        pBridge->Root(oRootObj);
        
        // Initialize COM for this thread...
        CoInitialize(NULL);
        
        // Get CLSID for our server...
        CLSID clsid;
        HRESULT hr = CLSIDFromProgID(L"Excel.Application", &clsid);
        
        if(FAILED(hr)) {
            
            ::MessageBox(NULL, "CLSIDFromProgID() failed", "Error", 0x10010);
            return ;
        }
        
        // Start server and get IDispatch...
        IDispatch *pXlApp;
        hr = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, IID_IDispatch, (void **)&pXlApp);
        if(FAILED(hr)) {
            ::MessageBox(NULL, "Excel not registered properly", "Error", 0x10010);
            return;
        }
        
        // Make it visible (i.e. app.visible = 1)
        /* { */
            
        /*     VARIANT x; */
        /*     x.vt = VT_I4; */
        /*     x.lVal = 1; */
        /*     AutoWrap(DISPATCH_PROPERTYPUT, NULL, pXlApp, L"Visible", 1, x); */
        /* } */
        
        // Get Workbooks collection
        IDispatch *pXlBooks;
        {
            VARIANT result;
            VariantInit(&result);
            AutoWrap(DISPATCH_PROPERTYGET, &result, pXlApp, L"Workbooks", 0);
            pXlBooks = result.pdispVal;
        }
        
        // Call Workbooks.Add() to get a new workbook...
        IDispatch *pXlBook;
        {
            VARIANT result;
            VariantInit(&result);
            AutoWrap(DISPATCH_PROPERTYGET, &result, pXlBooks, L"Add", 0);
            pXlBook = result.pdispVal;
        }
        CASObject oADG = oRootObj.Call("getadgMain");
        CASObject oDataProvider = oADG.Call("getdataProvider");
        unsigned int iRowMax=1, iColMax=1;
        // Create a 15x15 safearray of variants...
        VARIANT arr;
        arr.vt = VT_ARRAY | VT_VARIANT;
        {
            SAFEARRAYBOUND sab[2];
            sab[0].lLbound = 1; 
            
            CASObject oColRowSize;// = oDataProvider.Call("getlength");
            CASValue oSizeVal;
            
            oColRowSize = oDataProvider.Call("getlength");
            oColRowSize.GetValue(oSizeVal);
            
            if ( oSizeVal.m_nVal )
            {
                sab[0].cElements = (unsigned long)*oSizeVal.m_nVal;
                iRowMax = (unsigned int)*oSizeVal.m_nVal;
            }
            else
            {
                sab[0].cElements = 1;
            }
            
            //oColRowSize = oADG.Call("getColumns");
            //oColRowSize.GetValue(oSizeVal);
            
            oColRowSize = oADG.Call("getcolumns");
            oColRowSize.GetValue(oSizeVal);
            /* sab[0].cElements = 6; */
            sab[1].lLbound = 1; 
            
            if ( oSizeVal.m_aVal )
            {
                sab[1].cElements = (unsigned long)oSizeVal.m_aVal->size() + 1;
                iColMax = (unsigned int)oSizeVal.m_aVal->size() +1;
            }
            else
            {
                sab[1].cElements = 1;
            }
            
            /* sab[1].cElements = 3; */
            arr.parray = SafeArrayCreate(VT_VARIANT, 2, sab);
        }
       
        
        // Fill safearray with some values...
        for(unsigned int i=1; i<=iRowMax; i++) {
            for(unsigned int j=1; j<=iColMax; j++) {
                // Create entry value for (i,j)
                VARIANT tmp;
                tmp.vt = VT_R8;
                
                CASObject oObj = oDataProvider.Call("getItemAt", (int)i-1);
                CASValue oVal;
                oObj.GetValue(oVal);
                const char* col1[3] = {"col1", "col2", "col3"};
                char *currcol = NULL;
                
                if ( j - 1 < 3 )
                {
                    currcol = (char*)col1[j-1];
                }
                else
                    currcol = (char*)col1[0];
                
                CASObject oMapVal = oObj.GetObjectMap()[currcol];
                CASValue oVal1;
                oMapVal.GetValue(oVal1);
                if ( oVal1.m_nVal )
                {
                    tmp.dblVal = *oVal1.m_nVal;
                }
                else
                {                 
                    tmp.vt = VT_BSTR;
                    std::wstring wstr(oVal1.m_sVal->length(), L'');
                    std::copy(oVal1.m_sVal->begin(), oVal1.m_sVal->end(), wstr.begin());
                    tmp.bstrVal = ::SysAllocString(wstr.c_str());
                }
                
                //tmp.lVal = i*j;
                // Add to safearray...
                long indices[] = {i,j};
                SafeArrayPutElement(arr.parray, indices, (void *)&tmp);
            }
        }
        
        // Get ActiveSheet object
        IDispatch *pXlSheet;
        {
            VARIANT result;
            VariantInit(&result);
            AutoWrap(DISPATCH_PROPERTYGET, &result, pXlApp, L"ActiveSheet", 0);
            pXlSheet = result.pdispVal;
        }
        
        // Get Range object for the Range A1:O15...
        IDispatch *pXlRange;
        {
            VARIANT parm;
            parm.vt = VT_BSTR;
            parm.bstrVal = ::SysAllocString(L"A1:C7");
            
            VARIANT result;
            VariantInit(&result);
            AutoWrap(DISPATCH_PROPERTYGET, &result, pXlSheet, L"Range", 1, parm);
            VariantClear(&parm);
            
            pXlRange = result.pdispVal;
        }
        
        // Set range with our safearray...
        AutoWrap(DISPATCH_PROPERTYPUT, NULL, pXlRange, L"Value", 1, arr);
        
	
        {
            //VARIANT parm;
            //parm.vt = VT_BSTR;
            //parm.bstrVal = ::SysAllocString(L"A1:C7");
            
            VARIANT result;
            VariantInit(&result);
            AutoWrap(DISPATCH_PROPERTYGET, &result, pXlRange, L"Value", 1, NULL);
            //VariantClear(&parm);
            LOG("Result vt is " << result.vt);
            result.parray->cbElements;
            
            for(unsigned int i1=1; i1<=result.parray->rgsabound[1].cElements; i1++) 
            {
                CASObject oObj = oDataProvider.Call("getItemAt", (int)i1-1);
                for(unsigned int j1=1; j1<=result.parray->rgsabound[0].cElements; j1++) 
                {
                    // Create entry value for (i,j)
                    VARIANT tmp;
                    tmp.vt = VT_I4;
                    
                    long indices[] = {i1,j1};
                    SafeArrayGetElement(result.parray, indices, (void *)&tmp);
                    
                    const char* col1[3] = {"col1", "col2", "col3"};
                    char *currcol = NULL;
                    
                    if ( j1 - 1 < 3 )
                    {
                        currcol = (char*)col1[j1-1];
                    }
                    else
                        currcol = (char*)col1[0];
                    
                    //if ( tmp.
                    oObj.GetObjectMap()[currcol] = tmp.dblVal;
                    CASValue oItemVal;
                    oObj.GetValue(oItemVal);
                    oDataProvider.Call("setItemAt", oObj,(int)i1-1);
                    
                    
                    //pXlRange = result.pdispVal;
                }
                
            }
        
            VariantClear(&result);
        }
        // Wait for user...
        //::MessageBox(NULL, "All done.", "Notice", 0x10000);
        
        // Set .Saved property of workbook to TRUE so we aren't prompted
        // to save when we tell Excel to quit...
        {
            VARIANT x;
            x.vt = VT_I4;
            x.lVal = 1;
            AutoWrap(DISPATCH_PROPERTYPUT, NULL, pXlBook, L"Saved", 1, x);
        }
        
        // Tell Excel to quit (i.e. App.Quit)
        AutoWrap(DISPATCH_METHOD, NULL, pXlApp, L"Quit", 0);
        
        // Release references...
        pXlRange->Release();
        pXlSheet->Release();
        pXlBook->Release();
        pXlBooks->Release();
        pXlApp->Release();
        VariantClear(&arr);
        
        // Uninitialize COM for this thread...
        CoUninitialize();
        
    }
} 


void CASWorkFlexExcel::Worker()
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

CASWork* CASWorkFlexExcel::Clone()
{
    return (CASWork*)(new CASWorkFlexExcel());
}
