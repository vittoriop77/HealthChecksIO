/**************************************************************************
 * 
 * Interfacing ESP8266 NodeMCU with healthchecks.io
 *
 *************************************************************************/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

/* Set these to your desired credentials. */
const char *ssid = "REPLACEWITHYOURSSID"; //Enter your WIFI ssid
const char *password = "REPLACEWITHYOURPASSWORD"; //Enter your WIFI password


// Define the hostname, the port number and the fingerprint.
const char *host = "hc-ping.com";  
const int httpsPort = 443;


int timeSinceLastRead = 0;


void setup(void) {
  Serial.begin(115200);;
  Serial.setTimeout(2000);
  while(!Serial) { }
  Serial.println("Device Started");
  Serial.println("-------------------------------------");
  
  Serial.print("Configuring access point");
  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  // Halt the code until connected to WiFi.
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("*");
  }
  Serial.println("");
   Serial.println("WiFi connected");
   Serial.println("IP address: ");
   Serial.println(WiFi.localIP());
   WiFi.setAutoReconnect(true);
   WiFi.persistent(true);
}


void loop() {
  // Create a WiFiClientSecure object.
  WiFiClientSecure client;
  // Set the fingerprint to connect the server.
  client.setFingerprint(fingerprint);
  client.setInsecure();
  // If the host is not responding,return.
  if(!client.connect(host, httpsPort)){
    Serial.println("Connection Failed!");
    delay(2000);
    return;
  }
  
  // Send a GET request to a web page hosted by the server.
  client.print(String("GET ") + "/REPLACEWITHYOURURL" + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
  
  // Detect whether client is responding properly or not.
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  Serial.println("OK");
  delay(60000);
}
