You need visual studio.

Compile the code in src into a static lib. Link to that from the apps in examples. All apps in examples are MFC projects.

In each example, there will be a class inheriting from ASWork (like ASWorkSample, ASWorkFlexRun, etc.). This is the place to start coding in.

**Note**: If the swf doesn't load on startup of the App, you have to either change the "Movie" property of the flash player instance by opening up FlexVCBridgeAct.rc, Dialog and clicking on the black window.

Alternatively, you can call m\_Shockwave.LoadMovie("absolute path to swf") in CFlexVCBridgeDlg::OnInitDialog().

To compile the flex part, for each project you have to have the bridge folder with FABridge.as inside it.