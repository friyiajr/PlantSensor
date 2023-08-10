#include <string>
#include "WiFiS3.h"
#include "arduino_secrets.h"

#define POST_INTERVAL 1000

char ssid[] = WIFI_NETWORK_NAME;
char pass[] = WIFI_PASSWORD;
char server[] = WEB_ADDRESS;
char host[] = HOST;

int status = WL_IDLE_STATUS;

WiFiClient client;
unsigned long msecLst = 0;

void executeRequest() {
  if (client.connect(server, 8000)) {
    std::string payloadStart = "{\"moisture\": ";
    std::string payloadValue = std::to_string(72);
    std::string payloadEnd = "}";
    std::string payload = payloadStart + payloadValue + payloadEnd;

    std::string contentStart = "Content-Length: ";
    std::string contentEnd = std::to_string(payload.length());
    std::string content = contentStart + contentEnd;

    Serial.print("Sending Soil Moisture Reading: ");
    Serial.println(content.c_str());
    Serial.println(payload.c_str());
    
    client.println("POST / HTTP/1.1");
    client.println(host);
    client.println("Connection: keep-alive");
    client.println("Accept: application/json");
    client.println("Content-Type: application/json");
    client.println(content.c_str());
    client.println();
    client.println(payload.c_str());

    Serial.println("Moisture Reading Success");
  }
}

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  while (status != WL_CONNECTED) {
    Serial.println("Connecting to WIFI");
    status = WiFi.begin(ssid, pass);     
    // wait 10 seconds for connection:
    delay(10000);
  }
}

void loop() {
  unsigned long msec = millis();

  if ((msec - msecLst) > POST_INTERVAL)  {
      msecLst = msec;
      executeRequest();
  } else if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();

    // do nothing forevermore:
    while (true);
  }
}
