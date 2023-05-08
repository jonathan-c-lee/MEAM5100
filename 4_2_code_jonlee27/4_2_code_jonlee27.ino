#include "html510.h"          // use html510 package
#include "lab_4_2_website.h"  // use lab_4_2_website package

// right motor parameters
#define R_MOTOR_FWD 1
#define R_MOTOR_BWD 1
#define R_ENCODER   1
#define R_PWM_PIN   1
#define R_PWM_CHAN  1

// left motor parameters
#define L_MOTOR_FWD 1
#define L_MOTOR_BWD 1
#define L_ENCODER   1
#define L_PWM_PIN   1
#define L_PWM_CHAN  1

// PWM parameters
#define FREQUENCY  10000
#define RESOLUTION 10
#define MAX_DUTY   (1 << RESOLUTION) - 1

// speed parameters
#define MAX_RPM MAX_DUTY / 4 * 3
#define MIN_RPM -(MAX_DUTY) / 4 * 3

// interrupt variables
volatile int r_counter = 0;
volatile int l_counter = 0;

// PID variables
unsigned int kp = 5;
unsigned int ki = 10;
float error_i[2] = {0, 0};

// global variables for motors
long prev_time[2] = {0, 0};
float rpm_filter[2] = {0, 0};
float prev_rpm[2] = {0, 0};
int target_rpm[2] = {MAX_DUTY, MAX_DUTY};

// WiFi
HTML510Server h(80);
const char* ssid = "Team17";

void setup() {
  pwm_setup();
  motors_setup();
  // interrupts_setup();
  wifi_setup();
  server_setup();
}

void loop() {
  h.serve();
  motor_pid(0);
  motor_pid(1);
  delay(5);
}

// set up all necessary PWM channels
void pwm_setup() {
  // initialize right motor PWM
  ledcSetup(R_PWM_CHAN, FREQUENCY, RESOLUTION);
  ledcAttachPin(R_PWM_PIN, R_PWM_CHAN);
  ledcWrite(R_PWM_CHAN, 0);

  // initialize left motor PWM
  ledcSetup(L_PWM_CHAN, FREQUENCY, RESOLUTION);
  ledcAttachPin(L_PWM_PIN, L_PWM_CHAN);
  ledcWrite(L_PWM_CHAN, 0);
}

// set up all necessary motor related pins
void motors_setup() {
  // set up right motor
  pinMode(R_MOTOR_FWD, OUTPUT);
  pinMode(R_MOTOR_BWD, OUTPUT);
  set_motor_speed(0, 0);
  pinMode(R_ENCODER, INPUT);

  // set up left motor
  pinMode(L_MOTOR_FWD, OUTPUT);
  pinMode(L_MOTOR_BWD, OUTPUT);
  set_motor_speed(1, 0);
  pinMode(L_ENCODER, INPUT);
}

// right motor encoder interrupt function
void IRAM_ATTR r_increment() {
  r_counter++;
}

// left motor eoncider interrupt function
void IRAM_ATTR l_increment() {
  l_counter++;
}

// set up all encoder interrupts
void interrupts_setup() {
  attachInterrupt(R_ENCODER, r_increment, RISING);  // right motor encoder interrupt
  attachInterrupt(L_ENCODER, l_increment, RISING);  // left motor encoder interrupt
}

// set up WiFi with Access Point using DHCP
void wifi_setup() {
  Serial.begin(115200);
  Serial.print("Access Point: ");
  Serial.println(ssid);
  WiFi.softAP(ssid);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("APIP Address: ");
  Serial.println(IP);
}

// set up the server
void server_setup() {
  h.begin();
  h.attachHandler("/ ", handleRoot);
  h.attachHandler("/stop ", handleStop);
  h.attachHandler("/forwards ", handleForwards);
  h.attachHandler("/backwards ", handleBackwards);
  h.attachHandler("/right ", handleRight);
  h.attachHandler("/left ", handleLeft);
}

// root handler
void handleRoot() {
  h.sendhtml(body);
}

// stop button handler
void handleStop() {
  target_rpm[0] = 0;
  target_rpm[1] = 0;
}

// forwards button handler
void handleForwards() {
  target_rpm[0] = MAX_RPM;
  target_rpm[1] = MAX_RPM;
}

// backwards button handler
void handleBackwards() {
  target_rpm[0] = MIN_RPM;
  target_rpm[1] = MIN_RPM;
}

// right button handler
void handleRight() {
  target_rpm[0] = MIN_RPM;
  target_rpm[1] = MAX_RPM;
}

// left button handler
void handleLeft() {
  target_rpm[0] = MAX_RPM;
  target_rpm[1] = MIN_RPM;
}

/**
 * control motor speed using PID
 * :param motor: integer corresponding to motor, 0 = right motor
 */
void motor_pid(int motor) {
  // // get encoder count
  // unsigned int count;
  // portDISABLE_INTERRUPTS();
  // if (motor == 0) {
  //   count = r_counter;
  // } else {
  //   count = l_counter;
  // }
  // portENABLE_INTERRUPTS();

  // // calculate rpm
  // unsigned long curr_time = micros();
  // float time_elapsed = (float) (curr_time - prev_time[motor]) / 0.000001;
  // float rpm = count / time_elapsed * 3.0;
  // prev_time[motor] = curr_time;

  // // filter signal
  // rpm_filter[motor] = 0.0 * rpm_filter[motor] + 1.0 * rpm + 0.0 * prev_rpm[motor];
  // prev_rpm[motor] = rpm;

  // // calculate feedback
  // float error_p = target_rpm[motor] - rpm_filter[motor];
  // error_i[motor] = error_i[motor] + error_p * time_elapsed;
  // float duty = kp * error_p + ki * error_i[motor];
  // if (duty > MAX_DUTY) {
  //   duty = MAX_DUTY;
  // }

  // // set motor
  // set_motor_speed(motor, duty);
  set_motor_speed(motor, target_rpm[motor]);
}

/**
 * set the speed of a motor
 * :param motor: integer corresponding to motor, 0 = right motor
 * :param speed: speed to set
 */
void set_motor_speed(unsigned int motor, int speed) {
  // assign forwards and backwards pins and PWM channel
  unsigned int fwd_pin, bwd_pin, pwm_chan;
  if (motor == 0) {
    fwd_pin = R_MOTOR_FWD;
    bwd_pin = R_MOTOR_BWD;
    pwm_chan = R_PWM_CHAN;
  } else {
    fwd_pin = L_MOTOR_FWD;
    bwd_pin = L_MOTOR_BWD;
    pwm_chan = L_PWM_CHAN;
  }

  // set speed and direction
  if (speed == 0) {
    digitalWrite(fwd_pin, LOW);
    digitalWrite(bwd_pin, LOW);
  } else if (speed > 0) {
    digitalWrite(fwd_pin, HIGH);
    digitalWrite(bwd_pin, LOW);
  } else {
    digitalWrite(fwd_pin, LOW);
    digitalWrite(bwd_pin, HIGH);
  }
  ledcWrite(pwm_chan, abs(speed));
}