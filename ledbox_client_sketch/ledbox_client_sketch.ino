#include <WebSocketsClient.h>
#include <ESP8266WiFi.h>

WebSocketsClient webSocket;

// Temporarily hard-code the WiFi credentials for testing.
const char* ssid = "";
const char* password = "";

const char* ws_host = "ledbox-app-cqvyh.ondigitalocean.app"

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println('\n');

  ConnectToWiFi();
  ConnectToServer();
}

void ConnectToServer() {
  webSocket.beginSSL(ws_host, 443);
  webSocket.setExtraHeaders("");  // Remove the `Origin: file://` header just in case.
  webSocket.onEvent(WSEvent);
  webSocket.setReconnectInterval(5000);
}

void ConnectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.println(" ...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(++i);
    Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established.");
  Serial.print("IP Address:\t");
  Serial.println(WiFi.localIP());
}

void WSEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.println("[WSC] Disconnected");
      Serial.print(String((char *)payload));
      break;
    case WStype_CONNECTED:
      Serial.println("[WSC] Connected");
      break;
    case WStype_TEXT:
      Serial.printf("[WSC] Incoming Text: %s\n", payload);

      // send message to server
			// webSocket.sendTXT("message here");
      break;
    case WStype_BIN:
      Serial.printf("[WSC] Incoming Binary: %u\n", length);
      
      // send data to server
			// webSocket.sendBIN(payload, length)
      break;
  }
}

void loop() { 
  webSocket.loop(); 
}
