#include "main.h"


/** Put your WiFi credentials into this file **/
#include "wifikeys.h"

/** Camera class */
OV2640 cam;

/** GPIO for OTA request button */
int otaButton = 12;
/** Button class */
OneButton pushBt(otaButton, true, true);

/** Function declarations */
void enableOTA(void);
void resetDevice(void);

/** 
 * Called once after reboot/powerup
 */
void setup()
{
	// Start the serial connection
	Serial.begin(115200);

	Serial.println("\n\n##################################");
	Serial.printf("Internal Total heap %d, internal Free Heap %d\n", ESP.getHeapSize(), ESP.getFreeHeap());
	Serial.printf("SPIRam Total heap %d, SPIRam Free Heap %d\n", ESP.getPsramSize(), ESP.getFreePsram());
	Serial.printf("ChipRevision %d, Cpu Freq %d, SDK Version %s\n", ESP.getChipRevision(), ESP.getCpuFreqMHz(), ESP.getSdkVersion());
	Serial.printf("Flash Size %d, Flash Speed %d\n", ESP.getFlashChipSize(), ESP.getFlashChipSpeed());
	Serial.println("##################################\n\n");

	// Initialize the ESP32 CAM, here we use the AIthinker ESP32 CAM
	delay(100);
	cam.init(esp32cam_aithinker_config);
	delay(100);

	//LED settings
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, false);

	// Connect the WiFi
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}

	// Print information how to contact the camera server
	IPAddress ip = WiFi.localIP();
	Serial.print("\nWiFi connected with IP ");
	Serial.println(ip);

#ifdef ENABLE_WEBSERVER1
	Serial.print("Browser Stream Link: http://");
	Serial.print(ip);
	Serial.println(":81/stream\n");
	Serial.print("Browser Single Picture Link: http//");
	Serial.print(ip);
	Serial.println("/jpg\n");
	Serial.print("Switch LED ON: http//");
	Serial.print(ip);
	Serial.println("/ledon\n");
	Serial.print("Switch LED OFF: http//");
	Serial.print(ip);
	Serial.println("/ledoff\n");
	Serial.print("Start OTA: http//");
	Serial.print(ip);
	Serial.println("/start_ota\n");
#endif
#ifdef ENABLE_WEBSERVER1
	// Initialize the HTTP web stream server
	initWebStream1();
#endif
#ifdef ENABLE_WEBSERVER2
	// Initialize the HTTP web stream server
	initWebStream2();
#endif
	// Attach the button functions
	pushBt.attachClick(enableOTA);
	
}

void loop()
{
	// Check the button
	pushBt.tick();

	if (otaStarted)
	{
		ArduinoOTA.handle();
	}
	//Nothing else to do here
	delay(100);
}

/**
 * Handle OTA
 */
void enableOTA(void)
{
	// If OTA is not enabled
	if (!otaStarted)
	{
		// Stop the camera servers
#ifdef ENABLE_WEBSERVER
		stopWebStream();
#endif

		delay(100);
		Serial.println("OTA enabled");
		// Start the OTA server
		startOTA();
		otaStarted = true;
	}
	else
	{
		// If OTA was enabled
		otaStarted = false;
		// Stop the OTA server
		stopOTA();
		// Restart the camera servers
#ifdef ENABLE_WEBSERVER1
		initWebStream1();
#endif
#ifdef ENABLE_WEBSERVER2
		initWebStream2();
#endif

	}
}

/** 
 * Handle resetDevice
 */
void resetDevice(void)
{
	delay(100);
	WiFi.disconnect();
	esp_restart();
}