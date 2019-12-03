#include <Credentials.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <ESP8266SSDP.h>

const char* host = "Irrigation Controller";

#define pooltopup_topic "irrigation/controller/pool"
#define zone1_topic "irrigation/controller/zone1"
#define zone2_topic "irrigation/controller/zone2"
#define zone3_topic "irrigation/controller/zone3"

//This can be used to output the date the code was compiled
const char compile_date[] = __DATE__ " " __TIME__;

//Define Pins
const int SWIMMING_POOL = 5; //D1
const int ZONE_1 = 4; //D2
const int ZONE_2 = 14; //D5
const int ZONE_3 = 12; //D6

String strTopic;
String strPayload;

//Initialize Objects
WiFiClient espIrrigatonController;
PubSubClient client(espIrrigatonController);

void setup() {
  Serial.begin(115200);

  pinMode(SWIMMING_POOL, OUTPUT);
  digitalWrite(SWIMMING_POOL, HIGH);
  pinMode(ZONE_1, OUTPUT);
  digitalWrite(ZONE_1, HIGH);
  pinMode(ZONE_2, OUTPUT);
  digitalWrite(ZONE_2, HIGH);
  pinMode(ZONE_3, OUTPUT);
  digitalWrite(ZONE_3, HIGH);

  setup_wifi();

  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback); //callback is the function that gets called for a topic sub

  ArduinoOTA.setHostname("Irrigation Controller");
  ArduinoOTA.begin();
}

void loop() {
  //If MQTT client can't connect to broker, then reconnect
  if (!client.connected()) {
    reconnect();
  }
  client.loop(); //the mqtt function that processes MQTT messages
  ArduinoOTA.handle();
}

void callback(char* topic, byte* payload, unsigned int length) {
  //Handle mqtt messages received by this NodeMCU
  payload[length] = '\0';
  strTopic = String((char*)topic);
Serial.println("Topic " + strTopic);
  String command = String((char*)payload);
  Serial.println("Command " + command);
  if (strTopic == pooltopup_topic) {

    if (command == "ON") {
      digitalWrite(SWIMMING_POOL, LOW);
    } else {
      digitalWrite(SWIMMING_POOL, HIGH);
    }
  }
  else if ( strTopic ==  zone1_topic) {
    if (command == "ON") {
      digitalWrite(ZONE_1, LOW);
    } else {
      digitalWrite(ZONE_1, HIGH);
    }
  }
  else if ( strTopic ==  zone2_topic) {
    if (command == "ON") {
      digitalWrite(ZONE_2, LOW);
    } else {
      digitalWrite(ZONE_2, HIGH);
    }
  }
  else if ( strTopic ==  zone3_topic ) {
    if (command == "ON") {
      digitalWrite(ZONE_3, LOW);
    } else {
      digitalWrite(ZONE_3, HIGH);
    }
  }
}

void setup_wifi() {

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("MAC: ");
  Serial.println(WiFi.macAddress());
}

void reconnect() {
  //Reconnect to Wifi and to MQTT. If Wifi is already connected, then autoconnect doesn't do anything.
  Serial.print("Attempting MQTT connection...");
  if (client.connect(host, MQTT_USERNAME, MQTT_PASSWORD)) {
    Serial.println("connected");
    client.subscribe("irrigation/controller/#");
  } else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println(" try again in 5 seconds");
    // Wait 5 seconds before retrying
    delay(5000);
  }
}
