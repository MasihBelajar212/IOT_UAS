/*
This example shows how to connect to Cayenne using an Ethernet W5100 shield and send/receive sample data.

The CayenneMQTT Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.

Steps:
1. Set the Cayenne authentication info to match the authentication info from the Dashboard.
2. Compile and upload the sketch.
3. A temporary widget will be automatically generated in the Cayenne Dashboard. To make the widget permanent click the plus sign on the widget.
*/

//#define CAYENNE_DEBUG       // Uncomment to show debug messages
#define CAYENNE_PRINT Serial  // Comment this out to disable prints and save space
#include <CayenneMQTTEthernet.h>

#define SENSOR_PIN 0
#define VIRTUAL_CHANNEL 0

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "c427cf30-cc2d-11eb-8779-7d56e82df461";
char password[] = "998dad402f0d5b797c0e58e30599b76d9d81fc40c";
char clientID[] = "3f8a9220-cc2e-11eb-8779-7d56e82df461";

#define SENSOR_trig_PIN 9
#define SENSOR_echo_PIN 10
#define BUZZER_PIN 11
#define LED_PIN 13

// defines variables
long duration;
int distance;
int safetyDistance;

void setup() {
  Serial.begin(9600);
  Cayenne.begin(username, password, clientID);

pinMode(SENSOR_trig_PIN, OUTPUT); // Sets the trigPin as an Output
pinMode(SENSOR_echo_PIN, INPUT); // Sets the echoPin as an Input
pinMode(BUZZER_PIN, OUTPUT);
pinMode(LED_PIN, OUTPUT);
Serial.begin(9600); // Starts the serial communication
}

void loop() {
  Cayenne.loop();

  digitalWrite(SENSOR_trig_PIN, LOW);
delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(SENSOR_trig_PIN, HIGH);
delayMicroseconds(10);
digitalWrite(SENSOR_trig_PIN, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(SENSOR_echo_PIN, HIGH);

// Calculating the distance
distance= duration*0.034/2;

safetyDistance = distance;
if (safetyDistance <= 80){  // yang diganti - ganti jaraknya
  digitalWrite(BUZZER_PIN, HIGH);
  digitalWrite(LED_PIN, HIGH);
}
else{
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
}

// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.print(distance);
Serial.println(" cm");
}

// Default function for sending sensor data at intervals to Cayenne.
// You can also use functions for specific channels, e.g CAYENNE_OUT(1) for sending channel 1 data.
CAYENNE_OUT_DEFAULT()
{
  // Write data to Cayenne here. This example just sends the current uptime in milliseconds on virtual channel 0.
  Cayenne.virtualWrite(0, millis());
  // Some examples of other functions you can use to send data.
  //Cayenne.celsiusWrite(1, 22.0);
  //Cayenne.luxWrite(2, 700);
  //Cayenne.virtualWrite(3, 50, TYPE_PROXIMITY, UNIT_CENTIMETER);
}

// Default function for processing actuator commands from the Cayenne Dashboard.
// You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
CAYENNE_IN_DEFAULT()
{
  CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
  //Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}

CAYENNE_OUT(VIRTUAL_CHANNEL)
{
  Cayenne.virtualWrite(VIRTUAL_CHANNEL, analogRead(SENSOR_PIN), "analog_sensor", "null");
}
