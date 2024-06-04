#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>
#include <stddef.h>

// WiFi and MQTT configuration
const char* WIFI_SSID = "ABBgym_2.4"; // WiFi SSID
const char* WIFI_PASSWORD = "mittwifiarsabra"; // WiFi Password
const char* MQTT_SERVER = "maqiatto.com"; // MQTT Server address
const int MQTT_PORT = 1883; // MQTT Server port
const char* MQTT_USERNAME = "edvin.vikstrom@hitachigymnasiet.se"; // MQTT Username
const char* MQTT_PASSWORD = "EdvinV070615"; // MQTT Password
const char* CONTROL_TOPIC = "edvin.vikstrom@hitachigymnasiet.se/Driverbot"; // MQTT Topic
String clientId = String(ESP.getChipId()); // Unique client ID based on ESP8266 Chip ID

// Motor and servo configuration
#define motorPinRightSpeed 5 // D1, Connect to A+ on the motor shield
#define motorPinRightDir 0 // D2, Connect to A- on the motor shield
#define SERVO_PIN D5 // Pin for the servo

// WiFi and MQTT clients
WiFiClient espClient;
PubSubClient client(espClient);
Servo steeringServo; // Servo for steering

// Function to connect to WiFi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // Wait until WiFi is connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// Function to compare strings
short unsigned int str_eq(const char* a, char* b, size_t b_len) {
  size_t a_len = strlen(a);
  if (a_len != b_len)
    return 0;
  return (strncmp(a, b, b_len) == 0);
}

// Callback function for MQTT messages
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.printf("%.*s\n", length, payload);

  // Check payload and call appropriate function
  if (str_eq("up", (char*) payload, length)) {
    moveForward();
  }
  if (str_eq("down", (char*) payload, length)) {
    moveBackward();
  }
  if (str_eq("left", (char*) payload, length)) {
    turnLeft();
  }
  if (str_eq("right", (char*) payload, length)) {
    turnRight();
  }
  if (str_eq("stop", (char*) payload, length)) {
    StopCar();
  }
  if (str_eq("straight", (char*) payload, length)) {
    StraightCar();
  }
}

// Function to reconnect to MQTT server if connection is lost
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(clientId.c_str(), MQTT_USERNAME, MQTT_PASSWORD)) {
      Serial.println("connected");
      client.subscribe(CONTROL_TOPIC); // Subscribe to control topic
      break;
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000); // Wait 5 seconds before retrying
      break;
    }
  }
}

// Setup function
void setup() {
  Serial.begin(9600); // Initialize serial communication
  setup_wifi(); // Connect to WiFi
  client.setServer(MQTT_SERVER, MQTT_PORT); // Set MQTT server and port
  client.setCallback(callback); // Set callback function for MQTT messages
  steeringServo.attach(SERVO_PIN); // Attach servo to pin
  pinMode(motorPinRightSpeed, OUTPUT); // Set motor speed pin as output
  pinMode(motorPinRightDir, OUTPUT); // Set motor direction pin as output
}

// Main loop function
void loop() {
  if (!client.connected()) {
    reconnect(); // Reconnect to MQTT server if connection is lost
  }
  client.loop(); // Process MQTT messages
}

// Function to move the car forward
void moveForward() {
  digitalWrite(motorPinRightDir, LOW); // Set motor direction forwards
  analogWrite(motorPinRightSpeed, 1024); // Set motor speed
}

// Function to move the car backward
void moveBackward() {
  digitalWrite(motorPinRightDir, HIGH); // Set motor direction backwards
  analogWrite(motorPinRightSpeed, 1024); // Set motor speed
}

// Function to turn the car left
void turnLeft() {
  steeringServo.write(180); // Turn front wheels left
}

// Function to turn the car right
void turnRight() {
  steeringServo.write(0); // Turn front wheels right
}

// Function to point the car straight
void StraightCar() {
  steeringServo.write(90); // Point the car forward
}

// Function to stop the car
void StopCar() {
  analogWrite(motorPinRightSpeed, 0); // Set speed to 0
}
