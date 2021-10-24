#include <WiFiNINA.h>

char ssid[] = "vodafone-ADC9";
char pass[] = "LJWJSUVH4V7NCS";
WiFiServer server(80);
const int LEYE = 4;
const int REYE = 7;
const int MCR_1 = 9;
const int MCR_2 = 10; //black right motor
const int MCL_1 = 6;
const int MCL_2 = 5; //black left motor
int duration;
float distance;
const float sound = 34300.0; // speed of sound in cm/s
const float threshold = 10.0;
const int T_PIN = A2;
const int E_PIN = A3;
bool start = false;

void setup() {
  Serial.begin(9600);
  pinMode (MCR_1, OUTPUT);
  pinMode (MCR_2, OUTPUT);
  pinMode (MCL_1, OUTPUT);
  pinMode (MCL_2, OUTPUT);
  pinMode(LEYE, INPUT);
  pinMode(REYE, INPUT);
  pinMode(T_PIN, OUTPUT);
  pinMode(E_PIN, INPUT);
  WiFi.begin(ssid, pass);
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address:");
  Serial.print(ip);
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
   while(client.connected()) { 
    Serial.println("Client Connected");
    char c = client.read();
    if (c == 'w'){
      start = true;
      moveforward();
      Serial.println("Go");
    }
    if(c == 's'){
      start = false; 
      brake();
      Serial.println("Stop");
    }
      
    digitalWrite(T_PIN, LOW);
    delayMicroseconds(5);
    digitalWrite(T_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(T_PIN, LOW);
    duration = pulseIn(E_PIN, HIGH);
    distance = (duration/2) / 29.1;
    
   if (distance < 10) {
    Serial.print("obstacle seen");
    brake();
   } 
   else if (start == true){
    moveforward();
    right_turn();
    left_turn();
   }
  }
 }
void moveforward() { 
 if (digitalRead(REYE) == LOW && digitalRead(LEYE) == LOW){
        //digitalWrite(EN, LOW);
        digitalWrite(MCR_2, HIGH);
        digitalWrite(MCL_2, HIGH);
  } 
}

void right_turn(){
   if(digitalRead( REYE ) == HIGH && digitalRead(LEYE) == LOW ){ // right turn
      digitalWrite(MCR_2, LOW);
      digitalWrite(MCL_2, HIGH);   
      Serial.println("Right Turn");
   }
}
void left_turn(){
   if (digitalRead(REYE) == LOW && digitalRead(LEYE) == HIGH ) { // left turn
      digitalWrite(MCR_2, HIGH);
      digitalWrite(MCL_2, LOW); 
      Serial.print("Left Turn");   
   }   
}
void brake() {
  digitalWrite(MCR_2, LOW);
  digitalWrite(MCL_2, LOW);
} 
