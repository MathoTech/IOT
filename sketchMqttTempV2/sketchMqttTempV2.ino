#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <time.h>
#include <DHT.h>

// Numéro de série / Unique par carte
const char* SERIAL_NUMBER = "TempModulIOT8888";

// Delay pour envoi temp au client
const long regularIntervalSeconds = 10; // 10 secondes
unsigned long previousRegularMillis = 0;
const long regularInterval = regularIntervalSeconds * 1000;

// Delay pour envoi temp au server pour stocker sur Firebase
const long saveIntervalMinutes = 1; // 1min
unsigned long previousSaveMillis = 0;
const long saveInterval = saveIntervalMinutes * 60 * 1000;

// Paramètres du capteur DHT11
#define DHTPIN 0
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const char* mqtt_server = "u9v0uc.stackhero-network.com";
const int mqtt_port = 8883;
const char* mqtt_user = "admin";
const char* mqtt_password = "CRM6aqZbmRiCZYTiGA8CYGoIlV6xBL4j";

WiFiClientSecure espClient;
PubSubClient mqttClient(espClient);

const char* caCert = 
"-----BEGIN CERTIFICATE-----\n" \
"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\n" \
"-----END CERTIFICATE-----\n";

void setup() {
    Serial.begin(9600);

    Serial.print("Serial Number: ");
    Serial.println(SERIAL_NUMBER); 

    // Connect to WiFi using WiFiManager for ease of use
    WiFiManager wifiManager;
    wifiManager.setConnectTimeout(10);
    if (!wifiManager.autoConnect("D1MiniProAP")) {
        Serial.println("Failed to connect to WiFi. Restarting...");
        delay(3000);
        ESP.restart();
    }

    Serial.println("Connected to WiFi");

    // Initialize NTP
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");

    // Wait for time to be set
    time_t now = time(nullptr);
    while (now < 8 * 3600 * 2) {
        delay(500);
        now = time(nullptr);
    }

    // Print the time
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
    Serial.print("Current time: ");
    Serial.println(asctime(&timeinfo));
  
    // Load the CA certificate
    BearSSL::X509List x509(caCert);
    espClient.setTrustAnchors(&x509);

    // Set up the MQTT client
    mqttClient.setServer(mqtt_server, mqtt_port);
    mqttClient.setCallback(mqttCallback);

    // Connect to the MQTT broker
    connectToMqtt();

    // Initialise le capteur DHT11
    dht.begin();

    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();
  if (!mqttClient.connected()) {
    connectToMqtt();
  }
  mqttClient.loop();

  // Publication régulière toutes les 2 secondes
  if (currentMillis - previousRegularMillis >= regularInterval) {
    previousRegularMillis = currentMillis;
    publishTemperature();
  }

  // Envoi à /saveTemperature toutes les 60 secondes
  if (currentMillis - previousSaveMillis >= saveInterval) {
    previousSaveMillis = currentMillis;
    saveTemperature();
  }
}

void publishTemperature() {
  float temperature = dht.readTemperature();
  if (!isnan(temperature)) {
    char tempString[8];
    dtostrf(temperature, 1, 2, tempString);
    char topic[50];
    sprintf(topic, "device/%s/temperature", SERIAL_NUMBER);
    mqttClient.publish(topic, tempString);
      // Serial.print("Message send to ");
      // Serial.print(topic);
      // Serial.print("        ");
      // Serial.print("Température: ");
      // Serial.print(temperature);
      // Serial.println(" °C");
  }
}

void saveTemperature() {
  float temperature = dht.readTemperature();
  if (!isnan(temperature)) {
      // char saveTopic[50];
      // sprintf(topic, "/saveTemperature/%s", SERIAL_NUMBER);
      // mqttClient.publish(saveTopic, tempString);
      char saveString[50];
      sprintf(saveString, "%s: %.2f", SERIAL_NUMBER, temperature);
      mqttClient.publish("/saveTemperature", saveString);
      // Serial.print("Message send to ");
      // Serial.print(saveTopic);
      // Serial.print("        ");
      // Serial.println(saveString);
  }
}

void connectToMqtt() {
    while (!mqttClient.connected()) {
        // Generate a random 8-byte hexadecimal string
        String clientId = "ESPClient-";
        for (int i = 0; i < 8; i++) {
            clientId += String(random(0x10), HEX);  // Generates a single hexadecimal digit
        }

        Serial.print("Connecting to MQTT server with client ID: ");
        Serial.println(clientId);

        if (mqttClient.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
            Serial.println("Connected to MQTT server");
            // Subscribe to topics here if needed
            char topic[50];
            sprintf(topic, "device/%s/settings", SERIAL_NUMBER);
            mqttClient.subscribe(topic);
        } else {
            Serial.print("Failed to connect, error code: ");
            Serial.println(mqttClient.state());
            delay(10000);
        }
    }
}


void mqttCallback(char* topic, byte* payload, unsigned int length) {
    char message[length + 1];
    memcpy(message, payload, length);
    message[length] = '\0';

    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("]: ");
    Serial.println(message);

    // Handle the incoming message
    // Gérer le message pour la lumière
    // char topicLight[50];
    // sprintf(topicLight, "device/%s/light", SERIAL_NUMBER);
    // if (strcmp(topicLight, topic) == 0) {
    //     if (strcmp(message, "ON") == 0) {
    //         digitalWrite(LED_BUILTIN, LOW); // Allumer la LED
    //     } else if (strcmp(message, "OFF") == 0) {
    //         digitalWrite(LED_BUILTIN, HIGH); // Éteindre la LED
    //     }
    // }
    char settingsTopic[50];
    sprintf(settingsTopic, "device/%s/settings", SERIAL_NUMBER);
    if (strcmp(topic, settingsTopic) == 0) {
        if (strcmp(message, "DISCONNECT_WIFI") == 0) {
            Serial.println("Déconnexion du WiFi...");
            WiFi.disconnect(true);
        } else if (strcmp(message, "RESET_WIFI") == 0) {
            Serial.println("Réinitialisation des paramètres WiFi...");
            WiFi.disconnect();
            WiFiManager wifiManager;
            wifiManager.resetSettings();
            ESP.restart();
        } else if (strcmp(message, "RESTART") == 0) {
            Serial.println("Restart...");
            ESP.restart();
        }
    }
}