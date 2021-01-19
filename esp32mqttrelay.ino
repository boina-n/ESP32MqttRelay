#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"

// Update these with values suitable for your network.
const char* wifi_ssid = WIFI_SSID;
const char* wifi_password = WIFI_PASSWORD;
const char* mqtt_server = MQTT_SERVER;
const int   mqtt_port = MQTT_PORT;
const char* mqtt_user = MQTT_USER;
const char* mqtt_password = MQTT_PASSWORD;
const char* mqtt_serial_publisher_ch = MQTT_SERIAL_PUBLISHER_CH;
const char* mqtt_serial_receiver_ch = MQTT_SERIAL_RECEIVER_CH;
const int   id_map_length = ID_MAP_LENGTH;

WiFiClient wifiClient;
/*id - GPIO
   1 - 4
   2 - 5
   3 - 16
   4 - 17
*/
// relay pin
const int relayPin = 4;

int iomap[id_map_length] = {0, 4, 5, 16, 17};

PubSubClient client(wifiClient);

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);
  WiFi.begin(wifi_ssid, wifi_password);
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

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
      Serial.println("connected");
      //Once connected, publish an announcement...
      client.publish("/icircuit/presence/ESP32/", "hello world");
      // ... and resubscribe
      client.subscribe(mqtt_serial_receiver_ch);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void callback(char* topic, byte *payload, unsigned int length) {
  //print recevied messages on the serial console
  Serial.println("-------new message from broker-----");
  Serial.print("channel:");
  Serial.println(topic);
  Serial.print("data:");
  Serial.write(payload, length);
  Serial.println();
  String cmd;

  Serial.print("mqtt_serial_receiver_ch:");
  Serial.print(mqtt_serial_receiver_ch);
  Serial.println();
  Serial.print("Topic:");
  Serial.print(String(topic));
  Serial.println();

  //  Serial.print("Payload: ");
  //  for (int i = 0; i < length; i++) {
  //    Serial.print((char)payload[i]);
  //    cmd += (char)payload[i];
  //  }

  Serial.println();

  if (String(topic) == mqtt_serial_receiver_ch) {
    for (int i = 0; i < length; i++) {
      Serial.print("New Command: ");
      Serial.println((char)payload[i]);
      cmd += (char)payload[i];
    }

    if (String(cmd) == "u") {
      Serial.print("commande: ");
      Serial.print(cmd);

      Serial.println();

      Serial.print("state: ");
      digitalWrite(relayPin, LOW);
      int val = digitalRead(relayPin);
      Serial.print(val);
      delay(500);

      Serial.println();
      Serial.print("state: ");
      digitalWrite(relayPin, HIGH);
      int val2 = digitalRead(relayPin);
      Serial.print(val2);
      Serial.println();

    }
    else {
      Serial.println("Wrong command was used");
      Serial.print("command: ");
      Serial.print(cmd);

    }

    // Use the following structure if multiple condition.
    //    else if (String (cmd) == "d") {
    //    }

  }
}

void setup() {

  Serial.begin(115200);
  Serial.setTimeout(500);// Set time out for
  int i = 0;
  for (i = 0; i < ID_MAP_LENGTH; i++) {
    pinMode(iomap[i], OUTPUT);
  }
  digitalWrite(relayPin, HIGH);

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  reconnect();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
