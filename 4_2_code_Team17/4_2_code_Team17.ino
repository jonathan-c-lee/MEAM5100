/**
 * Name: 4_2_code_Team17.ino
 * Author: Jonathan Lee
 * Description: Code for Lab 4.2.
 */

#include "html510.h"          // use html510 header
#include "lab_4_2_website.h"  // use lab_4_2_website header

// front right motor parameters
#define FR_MOTOR_FWD 39
#define FR_MOTOR_BWD 40
#define FR_PWM_PIN   41
#define FR_PWM_CHAN  0

// front left motor parameters
#define FL_MOTOR_FWD 1
#define FL_MOTOR_BWD 2
#define FL_PWM_PIN   42
#define FL_PWM_CHAN  1

// rear right motor parameters
#define RR_MOTOR_FWD 19
#define RR_MOTOR_BWD 20
#define RR_PWM_PIN   21
#define RR_PWM_CHAN  2

// rear left motor parameters
#define RL_MOTOR_FWD 45
#define RL_MOTOR_BWD 34
#define RL_PWM_PIN   33
#define RL_PWM_CHAN  3

// PWM parameters
#define FREQUENCY  10000
#define RESOLUTION 10
#define MAX_DUTY   ((1 << RESOLUTION) - 1)

// WiFi
HTML510Server h(80);
const char* ssid_sta = "airportexpress";
const char* pwd_sta = "bluemango345";
const char* ssid_ap = "Team17";

// duty parameters
const int default_duty = 95;
const int rotation_duty = 70;

void setup() {
  pwm_setup();
  motors_setup();
  wifi_setup(0);
  server_setup();
}

void loop() {
  h.serve();
  delay(10);
}

// set up all necessary PWM channels
void pwm_setup() {
  // initialize front right motor PWM
  ledcSetup(FR_PWM_CHAN, FREQUENCY, RESOLUTION);
  ledcAttachPin(FR_PWM_PIN, FR_PWM_CHAN);
  ledcWrite(FR_PWM_CHAN, 0);

  // initialize front left motor PWM
  ledcSetup(FL_PWM_CHAN, FREQUENCY, RESOLUTION);
  ledcAttachPin(FL_PWM_PIN, FL_PWM_CHAN);
  ledcWrite(FL_PWM_CHAN, 0);
  
  // initialize rear right motor PWM
  ledcSetup(RR_PWM_CHAN, FREQUENCY, RESOLUTION);
  ledcAttachPin(RR_PWM_PIN, RR_PWM_CHAN);
  ledcWrite(RR_PWM_CHAN, 0);

  // initialize rear left motor PWM
  ledcSetup(RL_PWM_CHAN, FREQUENCY, RESOLUTION);
  ledcAttachPin(RL_PWM_PIN, RL_PWM_CHAN);
  ledcWrite(RL_PWM_CHAN, 0);
}

// set up all necessary motor pins
void motors_setup() {
  // set up front right motor
  pinMode(FR_MOTOR_FWD, OUTPUT);
  pinMode(FR_MOTOR_BWD, OUTPUT);

  // set up front left motor
  pinMode(FL_MOTOR_FWD, OUTPUT);
  pinMode(FL_MOTOR_BWD, OUTPUT);

  // set up rear right motor
  pinMode(RR_MOTOR_FWD, OUTPUT);
  pinMode(RR_MOTOR_BWD, OUTPUT);

  // setup rear left motor
  pinMode(RL_MOTOR_FWD, OUTPUT);
  pinMode(RL_MOTOR_BWD, OUTPUT);
}

// set up WiFi, 0 = AP, 1 = STA
void wifi_setup(unsigned int mode) {
  Serial.begin(115200);
  
  // 0 = AP Mode
  if (mode == 0) {
    Serial.print("Access Point: ");
    Serial.println(ssid_ap);
    WiFi.softAP(ssid_ap);
    IPAddress IP = WiFi.softAPIP();
    Serial.print("APIP Address: ");
    Serial.println(IP);
    return;
  }
  
  // 1 = STA Mode
  if (mode == 1) {
    WiFi.begin(ssid_sta, pwd_sta);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.print("Use this URL to connect: http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");
    return;
  }
}

// set up the server
void server_setup() {
  h.begin();
  h.attachHandler("/ ", handleRoot);
  h.attachHandler("/forwards ", handleForwards);
  h.attachHandler("/backwards ", handleBackwards);
  h.attachHandler("/stop ", handleStop);
  h.attachHandler("/right ", handleRight);
  h.attachHandler("/left ", handleLeft);
  h.attachHandler("/rright ", handleRotateRight);
  h.attachHandler("/rleft ", handleRotateLeft);
}

// root handler
void handleRoot() {
  h.sendhtml(body);
}

// move forwards command handler
void handleForwards() {
  set_motor_speed(0, default_duty);
  set_motor_speed(1, default_duty);
  set_motor_speed(2, default_duty);
  set_motor_speed(3, default_duty);
}

// move backwards command handler
void handleBackwards() {
  set_motor_speed(0, -default_duty);
  set_motor_speed(1, -default_duty);
  set_motor_speed(2, -default_duty);
  set_motor_speed(3, -default_duty);
}

// stop command handler
void handleStop() {
  set_motor_speed(0, 0);
  set_motor_speed(1, 0);
  set_motor_speed(2, 0);
  set_motor_speed(3, 0);
}

// move right command handler
void handleRight() {
  set_motor_speed(0, -default_duty);
  set_motor_speed(1, default_duty);
  set_motor_speed(2, default_duty);
  set_motor_speed(3, -default_duty);
}

// move left command handler
void handleLeft() {
  set_motor_speed(0, default_duty);
  set_motor_speed(1, -default_duty);
  set_motor_speed(2, -default_duty);
  set_motor_speed(3, default_duty);
}

// rotate right command handler
void handleRotateRight() {
  set_motor_speed(0, -rotation_duty);
  set_motor_speed(1, rotation_duty);
  set_motor_speed(2, -rotation_duty);
  set_motor_speed(3, rotation_duty);
}

// rotate left command handler
void handleRotateLeft() {
  set_motor_speed(0, rotation_duty);
  set_motor_speed(1, -rotation_duty);
  set_motor_speed(2, rotation_duty);
  set_motor_speed(3, -rotation_duty);
}

/**
 * set the speed of a motor
 * :param motor: integer corresponding to motor, 0 = FR, 1 = FL, 2 = RR, 3 = RL
 * :param duty: duty to set
 */
 void set_motor_speed(unsigned int motor, int duty) {
   // assign forwards and packwards pins and PWM channel
   uint8_t fwd_pin, bwd_pin, pwm_chan;
   if (motor == 0) {
     fwd_pin = FR_MOTOR_FWD;
     bwd_pin = FR_MOTOR_BWD;
     pwm_chan = FR_PWM_CHAN;
   } else if (motor == 1) {
     fwd_pin = FL_MOTOR_FWD;
     bwd_pin = FL_MOTOR_BWD;
     pwm_chan = FL_PWM_CHAN;
   } else if (motor == 2) {
     fwd_pin = RR_MOTOR_FWD;
     bwd_pin = RR_MOTOR_BWD;
     pwm_chan = RR_PWM_CHAN;
   } else {
     fwd_pin = RL_MOTOR_FWD;
     bwd_pin = RL_MOTOR_BWD;
     pwm_chan = RL_PWM_CHAN;
   }

   // set speed and direction
   if (duty == 0) {
     digitalWrite(fwd_pin, LOW);
     digitalWrite(bwd_pin, LOW);
   } else if (duty > 0) {
     digitalWrite(fwd_pin, HIGH);
     digitalWrite(bwd_pin, LOW);
   } else {
     digitalWrite(fwd_pin, LOW);
     digitalWrite(bwd_pin, HIGH);
   }

   ledcWrite(pwm_chan, map(abs(duty), 0, 100, 0, MAX_DUTY));
 }