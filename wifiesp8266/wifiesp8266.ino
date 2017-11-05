/*
  Basic ESP8266 MQTT example


  It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off

  It will reconnect to the server if the connection is lost using a blocking
  reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
  achieve the same result without blocking the main loop.

  To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier

const char* ssid = "CSU-GeekSpace";
const char* password = "147258369";
const char* mqtt_server = "119.29.67.126";
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
int data;
String oneLineData = "";
void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  char signaltype;
  String port;
  String keyvalue;

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    if (i == 0) {
      signaltype = (char)payload[0];
      Serial.println(signaltype);
    } else if (i == 1) {
      port += (char)payload[1];
      Serial.println(port);
    } else if ( i == 2) {
      port += (char)payload[2];
      Serial.println(port);
    } else {
      keyvalue += (char)payload[i];
      Serial.println(keyvalue);
    }

  }

  // we can now get the signaltype and port and keyvalue
  int keyvalueInt = keyvalue.toInt();
  if (signaltype == '0') {
    // indicates this is turn-on-off value
    // now keyvalueInt is 0 or 1
    Serial.println("get signaltype");
    if (keyvalueInt == 0) {
      Serial.println("kv=0");
      digitalWrite(D4, LOW);
      colorBlink();

    } else {
      Serial.println("kv=1");
      turnOnLight();
      delay(1000);
    }
  } else {
    Serial.println("else");
    // now keyvalueInt is 21 or 32
    // do something
  }

}

// ************ turn on light with a link *******************
void turnOnLight() {
  analogWrite(D5, 1024);
  analogWrite(D6, 1024);
  analogWrite(D7, 1024 );

  for (int i = 0; i < 4; i++) {
    digitalWrite(D4, HIGH);
    delay(100);
    digitalWrite(D4, LOW);
    delay(100);
  }
  digitalWrite(D4, HIGH);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe("manager/0004000001");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
    }
  }
}


void setup() {

  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(D4, OUTPUT);// Initialize the BUILTIN_LED pin as an output
  // this is for RGB led
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);

  analogWrite(D5, 1024);
  analogWrite(D6, 1024);
  analogWrite(D7, 1024);


  digitalWrite(D4, LOW);    // turn the LED off by making the voltage LOW
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setColor(int red, int green, int blue)
{
  analogWrite(D5, 1024 - red);
  analogWrite(D6, 1024 - green);
  analogWrite(D7, 1024 - blue);
}
void colorBlink() {
  Serial.println("----------=================color blink, fuck!!!!!!!!!!!!!!!!!!!!----------------");
  int i, j;
  for (i = 0, j = 1024; i < 1025; i++)
  {
    setColor(i, j, 0);
    delay(4);
    j--;
  }
  delay(100);           //绿色向红色渐变
  for (i = 0, j = 1024; i < 1025; i++)
  {
    setColor(j, 0, i);
    delay(4);
    j--;
  }
  delay(100);           //红色向蓝色渐变
  for (i = 0, j = 1024; i < 1025; i++)
  {
    setColor(0, i, j);
    delay(4);
    j--;
  }
  delay(100);          //蓝色向绿色渐变
}


void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    while (Serial.available() > 0)
    {
      oneLineData += char(Serial.read());
    }
    int sensorValue = analogRead(A0);
    Serial.println(sensorValue);
    String tem = String(sensorValue);
    client.publish("chat/bob", tem.c_str());
    tem = "";
    // memset(ret, 0, 100);

    snprintf (msg, 75, "wocha", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("outTopic", msg);
  }

}
