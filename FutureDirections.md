# Flex C++ Bridge - The Future #

Fake a NPAPI host (firefox's plugin architecture) to load firefox plugin version of flash player. --- This is done, ~~just need to figure out the externalinterface bit in firefox~~.

In firefox, the serialization happens in javascript which means we have to parse the javascript call to build the XML that the bridge can understand.

This is being done to port the bridge to Linux. I don't have a MAC, so can't do anything about a mac port.

~~Support communication via sockets. This should make it platform agnostic. Instead of putting XML requests from On Flash Call to Flex Call Queue.~~