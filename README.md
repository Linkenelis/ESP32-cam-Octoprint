This creates 2 webservers, 1 for the stream (video) on port 81 and 1 for jpg (snapshot), led and ota
Just type the ip address of your cam in a browser, the links will be there.

example with ip 192.168.2.207:
Browser Stream Link: http://192.168.2.207:81/stream
Browser Single Picture Link: http://192.168.2.207/jpg
Switch LED ON: http://192.168.2.207/ledon
Switch LED OFF: http://192.168.2.207/ledoff
Start OTA mode: http://192.168.2.207/start_ota

If you happen to use the wrong webserver (eg 81 instead of the standard 80, it will be auto matically redirected).
Yes this makes it possible to take a snapshot, while watching the live stream ;-) but you will see a glitch in the stream as there is only 1 camera chip

first time setup needs to be done via serial port. After that you can use OTA. Just follow the link to put the ESP32-cam into OTA mode and change from com to ip in platform.ini

Manual set to OTA by connecting IO12 to ground for a sec, then disconnect

This repository is forked from the [Circuitrocks@Learn](https://learn.circuit.rocks/?p=2245) blog how to stream a video from the ESP32-CAM to a RTSP client or web browser.

The source code for PlatformIO is in the _**src**_ subfolder. There you can also set your credentials for the network.

# Configuration & Build
First you should configure your Wifi credentials. This can bea easily done in src/wifikeys.h.
After this, compile and upload your code.

## Compiling with PlatformIO
Just open the folder containing the files with PlatformIO and adjust your settings in platformio.ini. Required libraries are either automatically installed (OneButton) or in the lib folder (Micro-RTSP).

## Useful links

* [Octoprint Forum Post](https://community.octoprint.org/t/octoprint-with-esp32-with-camera/6996/20)
* [Tutorial Blog Post](https://randomnerdtutorials.com/esp32-cam-take-photo-display-web-server/)
* [Platform IO ESP32-CAM Board Info Page](https://docs.platformio.org/en/latest/boards/espressif32/esp32cam.html)
* [PlatformIO Thread on Serial Port Issues](https://community.platformio.org/t/solved-esp32-serial-monitor-not-working/2858/6)



