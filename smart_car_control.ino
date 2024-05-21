#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include<Servo.h>
Servo servo;

// Define the motor pins

#define ENA D0
#define IN1 D1
#define IN2 D2
#define IN3 D3
#define IN4 D4
#define ENB D5

// Variables for the Blynk widget values
int x = 50;
int y = 50;
int Speed;

int sensorpin=A0;// soil pin
int sensorvalue=0;
int outputvalue=0;

int rainSensorPin = D6;// rain 

int servoOneRoler = D7;//servo
Servo servoTwo;

char auth[] = "V8TV7RxNP42-1KbXnhwCZ7aavnpF1ZPt"; //Enter your Blynk auth token
char ssid[] = "ASUS";           //WIFI Name
char pass[] = "12345678";           //WIFI Password

BlynkTimer timer;


//rain
void moisture() {
  int rainSensor = analogRead(rainSensorPin);// rain pin
  rainSensor = map(rainSensor, 100, 1023, 100, 0);
  Blynk.virtualWrite(V3, rainSensor);
  Serial.println(rainSensor);
}



void setup() {
  Serial.begin(9600);


  //Set the motor pins as output pins
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);


  //SERVO 
  servo.attach(servoOneRoler);
  servoTwo.attach(15);


  // Initialize the Blynk library
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  timer.setInterval(100L, moisture);
  



}

//servo
BLYNK_WRITE(V5){

   servo.write(param.asInt());

}
BLYNK_WRITE(V6){

   servoTwo.write(param.asInt());

}


// Get the joystick values
BLYNK_WRITE(V0) {
  x = param[0].asInt();
}
// Get the joystick values
BLYNK_WRITE(V1) {
  y = param[0].asInt();
}
//Get the slider values
BLYNK_WRITE(V2) {
  Speed = param.asInt();
}

// Check these values using the IF condition
void smartcar() {
  if (y > 70) {
    carForward();
    Serial.println("carForward");
  } else if (y < 30) {
    carBackward();
    Serial.println("carBackward");
  } else if (x < 30) {
    carLeft();
    Serial.println("carLeft");
  } else if (x > 70) {
    carRight();
    Serial.println("carRight");
  } else if (x < 70 && x > 30 && y < 70 && y > 30) {
    carStop();
    Serial.println("carstop");
  }
}


void loop() 
{
  Blynk.run();
  smartcar();// Call the main function
  timer.run();

  sensorvalue=analogRead(sensorpin);
  outputvalue=map(sensorvalue,0,1023,0,100); //100,0 pottupaaru
  //delay(1000);

  if(outputvalue>74)
  {
       //Serial.println("water your plant");
       Serial.print(outputvalue);
       Blynk.notify("water your plant");
       //delay(1000); 
  }
  else if(outputvalue<45)
  {
       //Serial.println("soil is wet enough to water");
       Serial.print(outputvalue);
       Blynk.notify("soil is wet enough to water");
       //delay(1000);
  }
  
  Blynk.virtualWrite(V4,outputvalue);


}


/**************Motor movement functions*****************/
void carForward() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void carBackward() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void carLeft() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void carRight() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void carStop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
