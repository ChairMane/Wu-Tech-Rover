//#define BLYNK_PRINT Serial 
#include <ESP8266_Lib.h>
#include <SimpleTimer.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <Wire.h>
#include <Servo.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"


Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *motor1  = AFMS.getMotor(1);
Adafruit_DCMotor *motor2  = AFMS.getMotor(2);
Adafruit_DCMotor *motor3  = AFMS.getMotor(3);
Adafruit_DCMotor *motor4  = AFMS.getMotor(4);
 
 
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "7e90c0b489454100a4b345e293d3f998";


//Servos for arm
Servo servo1;  //bottom servo
Servo servo2;  //shoulder
Servo servo3;  //elbow
Servo servo4;  //wrist
Servo seedBoxServo;  //servo that opens seed box
 
//Temperature sensor
//float temp;
//int tempPin = 8; //analog pin 1
   
//SimpleTimer timer;
 
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "MESA";
char pass[] = "A2+B2=C2";
 
 
// Hardware Serial on Mega, Leonardo, Micro...
#define EspSerial Serial1
 
 
// Your ESP8266 baud rate:
#define ESP8266_BAUD 115200


ESP8266 wifi(&EspSerial);

BLYNK_WRITE(V0) {                         //
    int x = param[0].asInt();             // LEFT WHEELS controlled by joystick
    int y = param[1].asInt();             //
    moveRover(motor2, motor4, y);         //
}
 
BLYNK_WRITE(V1) {                         //
    int x = param[0].asInt();             // RIGHT WHEELS controlled by joystick
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
    servo3.write(potentio);    
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
    //Serial.begin(9600);
    AFMS.begin();
 
    //Attaching servos to digital pins
    servo1.attach(22);
    servo2.attach(24);
    servo3.attach(26);
    servo4.attach(28);
    seedBoxServo.attach(30);
    
    seedBoxServo.write(20);
 
    // Set ESP8266 baud rate
    EspSerial.begin(ESP8266_BAUD);
    delay(10);
 
    Blynk.begin(auth, wifi, ssid, pass);
    // You can also specify server:
    //Blynk.begin(auth, wifi, ssid, pass, "blynk-cloud.com", 8442);
    //Blynk.begin(auth, wifi, ssid, pass, IPAddress(192,168,1,100), 8442);
    //timer.setInterval(1000L, sendUptime);
}


void sendUptime() {
    //Shows the value temp on virtual pin 10
    //Blynk.virtualWrite(10, temp);
}

 
void loop() {
    Blynk.run();
    timer.run();
    /*temp = analogRead(tempPin);
    temp = temp * 0.48828125;*/
}

