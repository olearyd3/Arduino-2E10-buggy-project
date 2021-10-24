#include <Arduino_LSM6DS3.h>
#include <WiFiNINA.h>
#include <MadgwickAHRS.h>

//initialise a Madgwick variable
Madgwick filter;
char ssid[] = "vodafone-ADC9";
char pass[] = "LJWJSUVH4V7NCS";
WiFiServer server(80);

//Pins
const int LEYE = 4;
const int REYE = 7;
const int ML_RED = 5; //forward
const int ML_BLACK= 6; 
const int MR_RED = 10;//forward
const int MR_BLACK = 9;
const int TRIG = A2;
const int ECHO = A3;

int velocity = 170;
char data[50];  

//Initialise IMU variables
float aX, aY, aZ;
float gX, gY, gZ;
float roll, pitch, heading;
  
void setup() {
Serial.begin(9600);
Serial.println("here");

//Set pin modes
pinMode(LEYE, INPUT);
pinMode(REYE, INPUT);
pinMode(ML_RED, OUTPUT);
pinMode(ML_BLACK, OUTPUT);
pinMode(MR_RED, OUTPUT);
pinMode(MR_BLACK, OUTPUT);
pinMode(TRIG, OUTPUT);
pinMode(ECHO, INPUT);
  
IMU.begin();
filter.begin(104);

if(!IMU.begin()){
  Serial.println("Failed to initialize IMU");
}

Serial.println("IMU initialized");

//WiFi
WiFi.begin(ssid, pass);
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address:");
  Serial.print(ip);
  server.begin();
}

void loop() {
  //Make buggy go around the track using IR sensors
  WiFiClient client = server.available();
  if (digitalRead(REYE) == LOW && digitalRead(LEYE) == LOW) {
            analogWrite(ML_RED, velocity);
            analogWrite(MR_BLACK, 0);
            analogWrite(MR_RED, velocity);
            analogWrite(ML_BLACK, 0);
  } 
  else if (digitalRead(REYE) == HIGH && digitalRead(LEYE) == HIGH) {
            brake();
  } 
  else if (digitalRead(REYE) == HIGH && digitalRead(LEYE) == LOW) {
            analogWrite(MR_RED, 0);
            analogWrite(MR_BLACK, 0);
            analogWrite(ML_RED, velocity);
            analogWrite(ML_BLACK, 0);
  } 
  else if (digitalRead(REYE) == LOW && digitalRead(LEYE) == HIGH) {
            analogWrite(ML_BLACK, 0);
            analogWrite(ML_RED, 0);
            analogWrite(MR_BLACK, 0);
            analogWrite(MR_RED, velocity);
   }
   //Uses IMU data to calculate roll, yaw, pitch
    if(IMU.accelerationAvailable() && IMU.gyroscopeAvailable()){
    IMU.readAcceleration(aX,aY,aZ);
    IMU.readGyroscope(gX,gY,gZ);
    
    filter.updateIMU(gX, gY, gZ, aX, aY, aZ);

    roll = filter.getRoll();
    pitch = filter.getPitch();
    heading = filter.getYaw();
    server.print("Orientation: ");
    server.print(heading);
    server.print(" ");
    server.print(pitch);
    server.print(" ");
    server.println(roll);

    Serial.print("Orientation: ");
    Serial.print(heading);
    Serial.print(" ");
    Serial.print(pitch);
    Serial.print(" ");
    Serial.println(roll);
  }
 // delay(10);
}

//brake function to make buggy stop
void brake() {
  analogWrite(MR_RED, 0);
  analogWrite(ML_BLACK, 0);
  analogWrite(ML_RED, 0);
  analogWrite(MR_BLACK, 0);
}
