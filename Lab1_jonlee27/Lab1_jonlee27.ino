/*
Name: Lab1_jonlee27.ino
Author: Jonathan Lee
Description: Arduino file for MEAM3480 Lab 1
*/

// Pins
#define LED_PIN            13
#define THERMISTOR_PIN     A0
#define POSITIVE_RELAY_PIN 7
#define NEGATIVE_RELAY_PIN 8

// Thermistor Parameters
#define NOMINAL_RESISTANCE  10000
#define NOMINAL_TEMPERATURE 25
#define B_COEFFICIENT       3950
#define SAMPLES             5
#define EXT_RESISTANCE      10000
int samples[SAMPLES];

int desired_temperature = 80;

void setup() {
  Serial.begin(9600);
  analogReference(EXTERNAL);
  pinMode(LED_PIN, OUTPUT);
  pinMode(POSITIVE_RELAY_PIN, OUTPUT);
  pinMode(NEGATIVE_RELAY_PIN, OUTPUT);
  digitalWrite(NEGATIVE_RELAY_PIN, LOW);
}

void loop() {
  // get samples
  for (uint8_t i = 0; i < SAMPLES; i++) {
    samples[i] = analogRead(THERMISTOR_PIN);
    delay(10);
  }

  // calculate average reading
  float average = 0;
  for (uint8_t i = 0; i < SAMPLES; i++) {
    average += samples[i];
  }
  average /= SAMPLES;
  
  // calculate temperature in Fahrenheit
  float resistance = EXT_RESISTANCE / (1023 / average - 1);
  float steinhart = log(resistance / NOMINAL_RESISTANCE);
  steinhart /= B_COEFFICIENT;
  steinhart += 1.0 / (NOMINAL_TEMPERATURE + 273.15);
  steinhart = 1.0 / steinhart;
  steinhart -= 273.15;
  steinhart *= 1.8;
  steinhart += 32;

  if (steinhart >= desired_temperature) {
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(POSITIVE_RELAY_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
    digitalWrite(POSITIVE_RELAY_PIN, LOW);
  }
  Serial.println(steinhart);
  delay(1000);
}