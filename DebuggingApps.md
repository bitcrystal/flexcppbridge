# Introduction #

Debugging apps built on top of the bridge may be considered to be not debuggable by Flex Builder. But this is not so.


# Details #

1. Build a debug version of the swf being used in your flexcppbridge app.

2. Create a new Run/Debug configuration in Flex Builder which launches an invalid http url such as http://localhost/askdjlaskdj.html.

3. Hit debug in flex builder. The browser will load up and show an error due to the invalid URL.

4. Launch the C++ app from your IDE. The Flex Builder debugger will connect to it now and you can debug as normal.