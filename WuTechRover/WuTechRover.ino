//Uncomment line below to show debug message
//#define BLYNK_PRINT Serial
 
#include <ESP8266_Lib.h>
#include <SimpleTimer.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <Wire.h>
#include <Servo.h>
#include <Adafruit_MotorShield.h>
 
 
// You should get Auth Token in the Blynk App under the nut icon in the project settings
// Your WiFi credentials
// Set password to "" for open networks
char auth[] = "YOUR AUTH TOKEN";
char ssid[] = "SSID";
char pass[] = "PASSWORD";


//Motors 1 and 3 are on the right side of the rover, and motors 2 and 4 are on the left
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *motor1 = AFMS.getMotor(1);
Adafruit_DCMotor *motor2 = AFMS.getMotor(2);
Adafruit_DCMotor *motor3 = AFMS.getMotor(3);
Adafruit_DCMotor *motor4 = AFMS.getMotor(4);

 
//Servos for robotic arm
Servo servo1;             //bottom servo
Servo servo2;             //shoulder
Servo servo3;             //elbow
Servo servo4;             //wrist
Servo seedBoxServo;       //seed box servo

   
SimpleTimer timer;


//First function passes in servo to be controlled, and 
void moveArmServo(Servo, int);
void moveRover(Adafruit_DCMotor*, Adafruit_DCMotor*, int);

 
// Hardware Serial on Mega, Leonardo, Micro...
#define EspSerial Serial1
 
 
// Your ESP8266 baud rate:
#define ESP8266_BAUD 115200

 
ESP8266 wifi(&EspSerial);
 
 
void setup() {
    //Uncomment for debugging in serial monitor
    //Serial.begin(9600);
    AFMS.begin();
    
 
    //Attaching servos to digital pins
    servo1.attach(22);
    servo2.attach(24);
    servo3.attach(26);
    servo4.attach(28);
    seedBoxServo.attach(30);
    

    //Sets initial angle of seed box servo
    seedBoxServo.write(0);

    servo1.write(145);
    servo2.write(0);
    servo3.write(50);
    servo4.write(90);
    
 
    //Sets ESP8266 baud rate
    EspSerial.begin(ESP8266_BAUD);
    delay(10);
    
 
    Blynk.begin(auth, wifi, ssid, pass);
}

//Left joystick, for control of the left pair of wheels (motors 2 and 4)
BLYNK_WRITE(V0) {
    int x = param[0].asInt();
    int y = param[1].asInt();
    moveRover(motor2, motor4, y);
}


//Right joystick, for control of the left pair of wheels (motors 1 and 3)
BLYNK_WRITE(V1) {
    int x = param[0].asInt();
    int y = param[1].asInt();
    moveRover(motor1, motor3, y);
}


//Horizontal slider controls the base servo that rotates horizontally
BLYNK_WRITE(V2) {                         
    int potentio = param.asInt();
    moveArmServo(servo1, potentio);
}

//Vertcial slider controls the shoulder servo that rotates vertically 
BLYNK_WRITE(V3) {
    int potentio = param.asInt();
    moveArmServo(servo2, potentio);
}


//Vorizontal slider controls the elbow servo that rotates vertically
BLYNK_WRITE(V4) {
    int potentio = param.asInt();
    moveArmServo(servo3, potentio);
}


//Horizontal slider controls the excavator bucket that rotates vertically 
BLYNK_WRITE(V5) {
    int potentio = param.asInt();
    moveArmServo(servo4, potentio);
}

//Button controls servo that opens door for seed box
BLYNK_WRITE(V6) {
    int buttonState = param.asInt();
    if (buttonState == 1) {
        seedBoxServo.write(80);
    } else {
        seedBoxServo.write(0);
    }
}
 
void loop() {
    Blynk.run();
}

void moveArmServo(Servo servo, int potentiometer){
    servo.write(potentiometer);    
    delay(15);
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

