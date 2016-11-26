/*
 WiFiEsp example: WebClient

 This sketch connects to google website using an ESP8266 module to
 perform a simple web search.

 For more details see: http://yaab-arduino.blogspot.com/p/wifiesp-example-client.html
*/

#include "WiFiEsp.h"

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

#define trigPin1 51
#define echoPin1 53
#define trigPin2 50
#define echoPin2 52


//char ssid[] = "U-Hack";            // your network SSID (name)
//char pass[] = "H4C@2016";        // your network password

char ssid[] = "rkvl";     // your network SSID (name)
char pass[] = "00000000";  // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

char server[] = "192.168.43.217";

// Initialize the Ethernet client object
WiFiEspClient client;

void setup()
{
  // initialize serial for debugging
  Serial.begin(9600);
  // initialize serial for ESP module
  Serial1.begin(9600);
  // initialize ESP module
  WiFi.init(&Serial1);

  pinMode(trigPin1,OUTPUT);
  pinMode(echoPin1,INPUT);
  pinMode(trigPin2,OUTPUT);
  pinMode(echoPin2,INPUT);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  // you're connected now, so print out the data
  Serial.println("You're connected to the network");
  
  printWifiStatus();
}

void loop()
{
  // if there are incoming bytes available
  // from the server, read them and print them
  while (client.available()&& status == WL_CONNECTED) {
    char c = client.read();
    Serial.write(c);
  }

  sendData();
  
  // if the server's disconnected, stop the client
  if (!client.connected()) {
    Serial.println();
    Serial.println("Disconnecting from server...");
    client.stop();

    // do nothing forevermore
    while (true);
  }
  delay(1000);
  client.flush();
  client.stop();
}

void sendData()
{
    String holder1 = "";
    long duration1,distance1;
    long duration2,distance2;

   //sensor1 
    digitalWrite(trigPin1,LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin1,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin1,LOW);
    duration1 = pulseIn(echoPin1,HIGH);
    distance1 = (duration1/2)/29.1;

  //sensor2
    digitalWrite(trigPin2,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2,LOW);
  duration2 = pulseIn(echoPin2,HIGH);
  distance2 = (duration2/2)/29.1;

    holder1 = holder1 + distance1+"_"+distance2;
    String stringhi = String();
    
    Serial.println();
    Serial.println("Starting connection to server...");
    // if you get a connection, report back via serial
    if (client.connect(server, 8080)) {
      Serial.println("Connected to server");
      // Make a HTTP request

      stringhi = "GET /alpha/php/arduinodata.php?sensordata="+holder1+" HTTP/1.1";
      client.println(stringhi);
      client.println("Host: localhost");
      client.println("Connection: close");
      client.println();
      delay(1000);
    }
}

void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
