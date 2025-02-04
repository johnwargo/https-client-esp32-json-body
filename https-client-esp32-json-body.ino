/**********************************************************
 * HTTPS Client ESP32 with JSON Request Body
 * By John M. Wargo
 * htttps://johnwargo.com
 * Created February 4, 2025
 ***********************************************************/

#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <WiFi.h>

#include "cert.h"
#include "config.h"

const char *ssid = WIFI_SSID;
const char *pswd = WIFI_PSWD;
const char *targetHost = "https://us-east1-jmw-demos.cloudfunctions.net/jsonbody";

int counter = 0;
int responseCode;

WiFiClientSecure *client = new WiFiClientSecure;
HTTPClient https;

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.print("\nConnecting to ");
  Serial.print(ssid);
  Serial.println(":");
  // Set Wi-Fi station mode, no incoming connections
  WiFi.mode(WIFI_STA);
  // connect to the Wi-Fi network
  WiFi.begin(ssid, pswd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    counter += 1;
    if (counter > 25) {
      counter = 0;
      Serial.println();
    }
  }
  Serial.println("\nWi-Fi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  if (client) {
    Serial.println("Client created successfully");
    // assign the Cert Authorith cert to the client object
    client->setCACert(caCert);
  } else {
    // this should never happen
    Serial.println("Unable to create client");
    // fatal error, so go into an infinite loop
    for (;;) {
    }
  }
}

void loop() {
  Serial.print("Connecting to ");
  Serial.println(targetHost);

  https.begin(*client, targetHost);
  https.addHeader("content-type", "application/json");
  
  Serial.println("Building POST body");
  String httpRequestData = buildRequestBodyJSON();
  Serial.print("Body: ");
  Serial.println(httpRequestData);
  
  Serial.println("\nExecuting POST request");
  responseCode = https.POST(httpRequestData);

  Serial.print("HTTPS response: ");
  Serial.println(responseCode);
  // will be -1 for errors
  if (responseCode > 0) {
    if (responseCode == HTTP_CODE_OK) {
      Serial.println("Success");
    } else {
      Serial.println("Failure");
    }
    Serial.printf("Content length: %u\n", https.getSize());
    String payload = https.getString();
    Serial.printf("Response Payload: %s\n", payload.c_str());
  } else {
    Serial.println("\nConnection failed");
    Serial.print("Message: ");
    Serial.println(https.errorToString(responseCode));
  }
  https.end();

  Serial.println("\nWaiting 30 seconds...\n");
  delay(30000);
}

String buildRequestBodyJSON() {
  String returnStr = "{ ";
  for (int row = 0; row < PARAM_ROWS; row++) {
    returnStr += "\"";
    returnStr += paramsArray[row][0];
    returnStr += "\": \"";
    returnStr += paramsArray[row][1];
    returnStr += "\"";
    if (row < PARAM_ROWS - 1) returnStr += ", ";
  }
  returnStr += " }";
  return returnStr;
}
