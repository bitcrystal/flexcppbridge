This is a translation of the javascript side of Ely Greenfield's FABridge to C++. The idea is that you can write flex code in C++ instead of simply making externalinterface calls to communicate with the flex side. It uses a slightly modified version of Ely's FABridge.as on the actionscript side.

For more details, see: http://anirudhs.chaosnet.org/blog/2008.03.13.html

Sample:
```
oRootObj.Call("getpanel1").Call("settitle", "Panel1");
pBridge->ClassRef("mx.controls.Alert").Call("show", "SampleAlert");
```

Currently, this is a windows only solution where the flash player ActiveX control is embedded onto a Visual C++ application. You can use externalinterface to talk to the flash player, but you'd have to handle passing of the XML back and forth yourself.

The bridge can have N worker threads processing requests from flash and vice versa. This frees the main MFC thread from getting blocked while some heavy CPU intensive operation is happening.

Multiple flash player instances are supported by the bridge.

Actionscript exceptions are serialized and passed across the bridge and thrown as C++ exceptions.

It uses [TinyXML](http://www.grinninglizard.com/tinyxml/) to parse XML.

Hopefully with the help from the opensource community, the Flex C++ bridge will be available on the Linux and Mac in the future. For more info regarding this, see FutureDirections.