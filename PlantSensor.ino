#include "WiFiS3.h"
#include "arduino_secrets.h"

char ssid[] = WIFI_NETWORK_NAME;
char pass[] = WIFI_PASSWORD;
char server[] = WEB_ADDRESS;
char host[] = HOST;

int status = WL_IDLE_STATUS;

WiFiClient client;

void setup() 
{
  Serial.begin(9600);
  while (!Serial);
  
  while (status != WL_CONNECTED) {
    Serial.println("Connecting to WIFI");
    status = WiFi.begin(ssid, pass);     
    // wait 10 seconds for connection:
    delay(10000);
  }
 
  Serial.println("\nStarting connection to server...");
  if (client.connect(server, 8000)) {
    Serial.println("connected to server");
    client.println("GET / HTTP/1.1");
    client.println(host);
    client.println("Connection: keep-alive");
    client.println();
    client.println();
  }
}

/* just wrap the received data up to 80 columns in the serial print*/
/* -------------------------------------------------------------------------- */
void read_response() {
/* -------------------------------------------------------------------------- */  
  uint32_t received_data_num = 0;
  while (client.available()) {
    /* actual data reception */
    char c = client.read();
    /* print data to serial port */
    Serial.print(c);
    /* wrap data to 80 columns*/
    received_data_num++;
    if(received_data_num % 80 == 0) { 
      Serial.println();
    }
  }  
}

/* -------------------------------------------------------------------------- */
void loop() {
/* -------------------------------------------------------------------------- */  
  read_response();

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();

    // do nothing forevermore:
    while (true);
  }
}
