#define BLYNK_PRINT Serial
 
#include <ESP8266_Lib.h>
#include <SimpleTimer.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <Wire.h>
#include <Servo.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
 
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *motor1 = AFMS.getMotor(1);
Adafruit_DCMotor *motor2 = AFMS.getMotor(2);
Adafruit_DCMotor *motor3 = AFMS.getMotor(3);
Adafruit_DCMotor *motor4 = AFMS.getMotor(4);
 
 
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "51638ee72eda4e9c89012e71ee42385d";
 
//Servos for arm
Servo servo1;  //bottom servo
Servo servo2;  //shoulder
Servo servo3;  //elbow
Servo servo4;  //wrist
Servo seedBoxServo;
 
//Temperature sensor
float temp;
int tempPin = 8; //analog pin 1
   
SimpleTimer timer;
 
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "";
char pass[] = "";
 
void moveRover(Adafruit_DCMotor*, Adafruit_DCMotor*, int);
 
// Hardware Serial on Mega, Leonardo, Micro...
#define EspSerial Serial1
 
// or Software Serial on Uno, Nano...
//#include <SoftwareSerial.h>
//SoftwareSerial EspSerial(2, 3); // RX, TX
 
// Your ESP8266 baud rate:
#define ESP8266_BAUD 115200
 
ESP8266 wifi(&EspSerial);
 
BLYNK_WRITE(V0) {                         //
    int x = param[0].asInt();             // LEFT WHEELS
    int y = param[1].asInt();             //
    moveRover(motor2, motor4, y);         //
}


BLYNK_WRITE(V1) {                         //
    int x = param[0].asInt();             // RIGHT WHEELS
    int y = param[1].asInt();             //
    moveRover(motor1, motor3, y);         //
}

 
BLYNK_WRITE(V2) {                         // Slider for bottom rotation servo
 
    int potentio = param.asInt();
    //Serial.println(potentio);
    potentio = map(potentio, 0, 1023, 0, 179);
    servo1.write(potentio);    
    delay(15);
}

 
BLYNK_WRITE(V3) {                         // Slider for shoulder servo
 
    int potentio = param.asInt();
    //Serial.println(potentio);
    potentio = map(potentio, 0, 1023, 0, 179);
    servo2.write(potentio);    
    delay(15);
}

 
BLYNK_WRITE(V4) {                         // Slider for elbow servo
 
    int potentio = param.asInt();
    //Serial.println(potentio);
    potentio = map(potentio, 0, 1023, 0, 179);
    servo3.write(potentio);    
    delay(15);
}

 
BLYNK_WRITE(V5) {                         // Slider for wrist servo
 
    int potentio = param.asInt();
    //Serial.println(potentio);
    potentio = map(potentio, 0, 1023, 0, 179);
    servo4.write(potentio);    
    delay(15);
}


BLYNK_WRITE(V6) {                         // Button for seed box servo
    int buttonState = param.asInt();
    if (buttonState == 1) {
        seedBoxServo.write(80);
    } else {
        seedBoxServo.write(0);
    }
}
 
 
void setup() {
    // Debug console
    Serial.begin(9600);
    AFMS.begin();
 
    //Attaching servos to digital pins
    servo1.attach(22);
    servo2.attach(24);
    servo3.attach(26);
    servo4.attach(28);
    seedBoxServo.attach(30);

    seedBoxServo.write(0);
 
    // Set ESP8266 baud rate
    EspSerial.begin(ESP8266_BAUD);
    delay(10);
 
    Blynk.begin(auth, wifi, ssid, pass);
}

 
void sendUptime() {
    //Shows the value temp on virtual pin 10
    Blynk.virtualWrite(10, temp);
}

 
void loop() {
    Blynk.run();
}
 
 
void moveRover(Adafruit_DCMotor *motor_a, Adafruit_DCMotor *motor_b, int y_speed) {
    if (y_speed == 128){
        motor_a->run(RELEASE);
        motor_b->run(RELEASE);
    } else if (y_speed > 128) {
        y_speed = (y_speed * 2) - 255;
        motor_a->setSpeed(y_speed);
        motor_b->setSpeed(y_speed);
        motor_a->run(FORWARD);
        motor_b->run(FORWARD);
    } else {
        y_speed = 255 - y_speed;
        y_speed = (y_speed * 2) - 255;
        motor_a->setSpeed(y_speed);
        motor_b->setSpeed(y_speed);
        motor_a->run(BACKWARD);
        motor_b->run(BACKWARD);
    }
}

