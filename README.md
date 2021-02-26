This creates 2 webservers, 1 for the stream (video) on port 81 and 1 for jpg (snapshot), led and ota
Just type the ip address of your cam in a browser, the links will be there.
If you happen to use the wrong webserver (eg 81 instead of the standard 80, it will be auto matically redirected).
Yes this makes it possible to take a snapshot, while watching the live stream ;-) but you will see a glitch in the stream as there is only 1 camera chip

first time setup needs to be done via serial port. After that you can use OTA. Just follow the link to put the ESP32-cam into OTA mode and change from com to ip in platform.ini

Manual set to OTA by connecting IO12 to ground for a sec, then disconnect

This repository is forked from the [Circuitrocks@Learn](https://learn.circuit.rocks/?p=2245) blog how to stream a video from the ESP32-CAM to a RTSP client or web browser.

The source code for PlatformIO is in the _**src**_ subfolder.

## Compiling with PlatformIO
Just open the folder containing the files with PlatformIO and adjust your settings in platformio.ini. Required libraries are either automatically installed (OneButton) or in the lib folder (Micro-RTSP).



