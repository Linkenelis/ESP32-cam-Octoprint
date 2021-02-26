#include "main.h"

/** Web server class */
WebServer server1(81);

/** Forward dedclaration of the task handling browser requests */
void webTask1(void *pvParameters);

/** Task handle of the web task */
TaskHandle_t webTaskHandler;

/** Flag to stop the web server1 */
boolean stopWeb = false;

/** Web request function forward declarations */
void handle_jpg_stream(void);
void handle_jpg(void);
void handle_ledon(void);
void handle_ledoff(void);
void handle_start_ota(void);
void handleNotFound();



/**
 * Initialize the web stream server1 by starting the handler task
 */
void initWebStream1(void)
{
#ifdef ENABLE_WEBSERVER1
	// Create the task for the web server1 and run it in core 0 (others tasks use core 1)
	xTaskCreatePinnedToCore(webTask1, "WEB", 4096, NULL, 1, &webTaskHandler,0);

	if (webTaskHandler == NULL)
	{
		Serial.println("Create Webstream task failed");
	}
	else
	{
		Serial.println("Webstream task up and running");
	}
#endif
}

/**
 * Called to stop the web server1 task, needed for OTA
 * to avoid OTA timeout error
 */
void stopWebStream(void)
{
	stopWeb = true;
}

/**
 * The task that handles web server1 connections
 * Starts the web server1
 * Handles requests in an endless loop
 * until a stop request is received because OTA
 * starts
 */
void webTask1(void *pvParameters)
{
	// Set the function to handle stream requests
	server1.on("/stream", HTTP_GET, handle_jpg_stream);
	// Set the function to handle single picture requests
	server1.on("/jpg", HTTP_GET, handle_jpg);
	server1.on("/ledon", HTTP_GET, handle_ledon);
	server1.on("/ledoff", HTTP_GET, handle_ledoff);
	server1.on("/start_ota", HTTP_GET, handle_start_ota);
	// Set the function to handle other requests
	server1.onNotFound(handleNotFound);
	// Start the web server1
	server1.begin();

	while (1)
	{
#ifdef ENABLE_WEBSERVER1
			// Check if the server1 has clients
			server1.handleClient();
#endif
		}
		if (stopWeb)
		{
			// User requested web server1 stop
			server1.close();
			// Delete this task
			vTaskDelete(NULL);
		}
		delay(100);
}

#ifdef ENABLE_WEBSERVER1
/**
 * Handle web stream requests
 * Gives a first response to prepare the streaming
 * Then runs in a loop to update the web content
 * every time a new frame is available
 */
void handle_jpg_stream(void)
{
	WiFiClient thisClient = server1.client();
	String response = "HTTP/1.1 200 OK\r\n";
	response += "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n\r\n";
	server1.sendContent(response);

	while (1)
	{
		cam.run();
		if (!thisClient.connected())
		{
			break;
		}
		response = "--frame\r\n";
		response += "Content-Type: image/jpeg\r\n\r\n";
		server1.sendContent(response);

		thisClient.write((char *)cam.getfb(), cam.getSize());
		server1.sendContent("\r\n");
		delay(20);
	}
}

/**
 * Handle single picture requests
 * redirect to the other stream
 */
void handle_jpg(void)
{
	IPAddress ip = WiFi.localIP();
	server1.sendHeader("Location", String("http://")+ ip.toString()+ String(":80/jpg"));
	server1.send(302, "text/plain", "");
	server1.client().stop();

}
void handle_ledon(void)
{
	IPAddress ip = WiFi.localIP();
	server1.sendHeader("Location", String("http://")+ ip.toString()+ String(":80/ledon"));
	server1.send(302, "text/plain", "");
	server1.client().stop();
}
void handle_ledoff(void)
{
	IPAddress ip = WiFi.localIP();
	server1.sendHeader("Location", String("http://")+ ip.toString()+ String(":80/ledoff"));
	server1.send(302, "text/plain", "");
	server1.client().stop();
}
void handle_start_ota(void)
{
	IPAddress ip = WiFi.localIP();
	server1.sendHeader("Location", String("http://")+ ip.toString()+ String(":80/start_ota"));
	server1.send(302, "text/plain", "");
	server1.client().stop();
	// If OTA is not enabled
	if (!otaStarted)
	{
		// Stop the camera server1s
#ifdef ENABLE_WEBSERVER1
		stopWebStream();
#endif

		delay(100);
		Serial.println("OTA enabled");
		// Start the OTA server1
		startOTA();
		otaStarted = true;
	}
	else
	{
		// If OTA was enabled
		otaStarted = false;
		// Stop the OTA server1
		stopOTA();
		// Restart the camera server1s
#ifdef ENABLE_WEBSERVER1
		initWebStream1();
#endif

	}
}
/**
 * Handle any other request from the web client
 */
void handleNotFound()
{
	IPAddress ip = WiFi.localIP();
	String message = "Browser Stream Link: http://";
	message += ip.toString();
	message += "81/stream\n";
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
	server1.send(200, "text/plain", message);
}
#endif
