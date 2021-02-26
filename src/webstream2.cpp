#include "main.h"

/** Web server class */
WebServer server2(80);

/** Forward dedclaration of the task handling browser requests */
void webTask2(void *pvParameters);

/** Task handle of the web task */
TaskHandle_t webTaskHandler2;

/** Flag to stop the web server2 */
boolean stopWeb2 = false;

/** Web request function forward declarations */
void handle_jpg_stream2(void);
void handle_jpg2(void);
void handle_ledon2(void);
void handle_ledoff2(void);
void handle_start_ota2(void);
void handleNotFound2();



/**
 * Initialize the web stream server2 by starting the handler task
 */
void initWebStream2(void)
{
#ifdef ENABLE_WEBSERVER2
	// Create the task for the web server2 and run it in core 0 (others tasks use core 1)
	xTaskCreatePinnedToCore(webTask2, "WEB", 4096, NULL, 1, &webTaskHandler2,1);

	if (webTaskHandler2 == NULL)
	{
		Serial.println("Create Webstream2 task failed");
	}
	else
	{
		Serial.println("Webstream2 task up and running");
	}
#endif
}

/**
 * Called to stop the web server2 task, needed for OTA
 * to avoid OTA timeout error
 */
void stopWebStream2(void)
{
	stopWeb2 = true;
}

/**
 * The task that handles web server2 connections
 * Starts the web server2
 * Handles requests in an endless loop
 * until a stop request is received because OTA
 * starts
 */
void webTask2(void *pvParameters)
{
	// Set the function to handle stream requests
	server2.on("/stream", HTTP_GET, handle_jpg_stream2);
	// Set the function to handle single picture requests
	server2.on("/jpg", HTTP_GET, handle_jpg2);
	server2.on("/ledon", HTTP_GET, handle_ledon2);
	server2.on("/ledoff", HTTP_GET, handle_ledoff2);
	server2.on("/start_ota", HTTP_GET, handle_start_ota2);
	// Set the function to handle other requests
	server2.onNotFound(handleNotFound2);
	// Start the web server2
	server2.begin();

	while (1)
	{
#ifdef ENABLE_WEBSERVER2
			// Check if the server2 has clients
			server2.handleClient();
#endif
		}
		if (stopWeb2)
		{
			// User requested web server2 stop
			server2.close();
			// Delete this task
			vTaskDelete(NULL);
		}
		delay(100);
}

#ifdef ENABLE_WEBSERVER2
/**
 * Handle web stream requests
 * redirect to ip:81/stream
 */

void handle_jpg_stream2(void)
{
	IPAddress ip = WiFi.localIP();
	server2.sendHeader("Location", String("http://")+ ip.toString()+ String(":81/stream"));
	server2.send(302, "text/plain", "");
	server2.client().stop();
}

/**
 * Handle single picture requests
 * Gets the latest picture from the camera
 * and sends it to the web client
 */
void handle_jpg2(void)
{

	WiFiClient thisClient = server2.client();

	cam.run();
	if (!thisClient.connected())
	{
		return;
	}
	String response = "HTTP/1.1 200 OK\r\n";
	response += "Content-disposition: inline; filename=capture.jpg\r\n";
	response += "Content-type: image/jpeg\r\n\r\n";
	server2.sendContent(response);
	thisClient.write((char *)cam.getfb(), cam.getSize());

}
void handle_ledon2(void)
{
	digitalWrite(LED_BUILTIN, true);
	IPAddress ip = WiFi.localIP();
	String message = "Done";
	message += "\n";
	server2.send(200, "text/plain", message);
}
void handle_ledoff2(void)
{
	digitalWrite(LED_BUILTIN, false);
	IPAddress ip = WiFi.localIP();
	String message = "Done";
	message += "\n";
	server2.send(200, "text/plain", message);
}
void handle_start_ota2(void)
{
	IPAddress ip = WiFi.localIP();
	String message = "Going into OTA mode";
	message += "\n";
	server2.send(200, "text/plain", message);
	// If OTA is not enabled
	if (!otaStarted)
	{
		// Stop the camera server2s
#ifdef ENABLE_WEBSERVER2
		stopWebStream2();
#endif

		delay(100);
		Serial.println("OTA enabled");
		// Start the OTA server2
		startOTA();
		otaStarted = true;
	}
	else
	{
		// If OTA was enabled
		otaStarted = false;
		// Stop the OTA server2
		stopOTA();
		// Restart the camera server2s
#ifdef ENABLE_WEBSERVER2
		initWebStream2();
#endif

	}
}
/**
 * Handle any other request from the web client
 */
void handleNotFound2()
{
	IPAddress ip = WiFi.localIP();
	String message = "Browser Stream Link: http://";
	message += ip.toString();
	message += ":81/stream\n";
	message += "Browser Single Picture Link: http://";
	message += ip.toString();
	message += "/jpg\n";
	message += "Switch LED ON: http://";
	message += ip.toString();
	message += "/ledon\n";
	message += "Switch LED OFF: http://";
	message += ip.toString();
	message += "/ledoff\n";
	message += "Start OTA mode: http://";
	message += ip.toString();
	message += "/start_ota\n";
	message += "\n";
	server2.send(200, "text/plain", message);
}
#endif
