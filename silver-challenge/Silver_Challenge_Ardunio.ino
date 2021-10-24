#include <WiFiNINA.h>

char ssid[] = "SKYB0D2E_EXT";
char pass[] = "RQWPSNBYFB";
WiFiServer server(80);
//PID constants
double kp = 10;
double ki = 0;
double kd = 3;

int duration;
float distance;
const float sound = 34300.0; // speed of sound in cm/s

const int T_PIN = A0;
const int E_PIN = A1;
const int MCR_1 = 3;
const int MCR_2 = 10; //forward
const int MCL_1 = 5;
const int MCL_2 = 6;
const int LEYE = A4;
const int REYE = A5;
const int rightPWM = A7;
const int leftPWM = A3;

int rightPWMSpeed = 180;
int leftPWMSpeed = 180;

unsigned long currentTime, previousTime;
double elapsedTime;
double error;
double lastError;
double input, output, setPoint = 10;
double cumError, rateError;
int speedout;

bool start = false;
bool obstacle = false;
bool pid = false;

void setup() {

  Serial.begin(9600);
  pinMode( LEYE, INPUT );
  pinMode( REYE, INPUT );
  pinMode ( MCR_1, OUTPUT );
  pinMode ( MCR_2, OUTPUT );
  pinMode ( MCL_1, OUTPUT );
  pinMode ( MCL_2, OUTPUT );
  pinMode( T_PIN, OUTPUT );
  pinMode( E_PIN, INPUT );
  pinMode(rightPWM, OUTPUT);
  pinMode(leftPWM, OUTPUT);

  WiFi.begin(ssid, pass);
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address:");
  Serial.print(ip);
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  while (client.connected()) {
    char c = client.read();
    if (c == 'w') {
      start = true;
      PIDimp();
      server.write("b");
    }
    if (c == 's') {
      start = false;
      brake();
      server.write("c");
    }

    if ( distance < 20) {
      if (obstacle == false) {
        server.write(20);
      }
      PIDimp();
      obstacle = true;

    } else if (start == true) {
      if (obstacle == true) {
        obstacle = false;
      }

      PIDimp();
    }
  }
}
void PIDimp() {
  digitalWrite(T_PIN, LOW);
  delayMicroseconds(100);
  digitalWrite(T_PIN, HIGH);
  delayMicroseconds(150);
  digitalWrite(T_PIN, LOW);

  duration = pulseIn(E_PIN, HIGH);
  distance = (duration / 2) / 29.1; //calculates the distance from taking the time it takes for a ping from the trig pin to bounce off an object and return to the echo pin and dividing it by 2 (this gets the time for ping to bounce off the object and reach the echo pin). The duration/2 is then multiplied by the speed of sound in microseconds/cm to get the distance in cm. 

  input = distance;

  if (input != 0) {
    output = computePID(input);
    server.write(input); //sends distance values to client so that distance can be shown on GUI
    speedout = map(output, -110, 100, 255, -255);

    if (output <= 0) { //includes line following and PID control
      if (pid == false) {
        if (digitalRead( REYE ) == LOW && digitalRead( LEYE ) == LOW) {
          digitalWrite(MCR_1, LOW);
          digitalWrite(MCL_1, LOW);
          digitalWrite(MCR_2, HIGH);
          digitalWrite(MCL_2, HIGH);
          analogWrite(rightPWM, speedout);
          analogWrite(leftPWM, speedout);
        }
        if (digitalRead( REYE ) == HIGH && digitalRead( LEYE ) == HIGH) {
          digitalWrite(MCR_2, LOW);
          digitalWrite(MCL_2, LOW);
          analogWrite(rightPWM, speedout + 10); //+ 10 was added for one buggy in particular as the left and right motors speed cut off was uneven.
          analogWrite(leftPWM, speedout);
        }
        if (digitalRead( REYE ) == HIGH && digitalRead( LEYE ) == LOW ) { // right turn
          digitalWrite(MCR_2, LOW);
          digitalWrite(MCL_2, HIGH);
          analogWrite(rightPWM, speedout + 10);
          analogWrite(leftPWM, speedout);
        }
        if (digitalRead( REYE ) == LOW && digitalRead( LEYE ) == HIGH ) { // left turn
          digitalWrite(MCR_2, HIGH);
          digitalWrite(MCL_2, LOW);
          analogWrite(rightPWM, speedout + 10);
          analogWrite(leftPWM, speedout);
        }
        delay(50);
      }
      pid = false;
      Serial.println(speedout);
      Serial.println(distance);
      Serial.println(output);
    }

    else { //when the object is close enough so that the calculated output is > 0 the buggy reverse to avoid collision.
      digitalWrite(MCR_1, HIGH);
      digitalWrite(MCL_1, HIGH);
      digitalWrite(MCR_2, LOW);
      digitalWrite(MCL_2, LOW);

      analogWrite(rightPWM, rightPWMSpeed);
      analogWrite(leftPWM, leftPWMSpeed);
      delay(50);
    }
  }
}
void brake() { //this function stops the buggy when stop button is pressed on GUI
  digitalWrite(MCR_2, LOW);
  digitalWrite(MCL_2, LOW);
}

double computePID(double inp) {
  currentTime = millis();                //get current time
  elapsedTime = (double)(currentTime - previousTime);        //compute time elapsed from previous computation

  error = setPoint - inp;                                // determine error
  cumError += error * elapsedTime;                // compute integral
  rateError = (error - lastError) / elapsedTime; // compute derivative

  double out = kp * error + ki * cumError + kd * rateError;          //PID output

  lastError = error;                                //remember current error
  previousTime = currentTime;                        //remember current time

  return out;                                        //have function return the PID output
}
