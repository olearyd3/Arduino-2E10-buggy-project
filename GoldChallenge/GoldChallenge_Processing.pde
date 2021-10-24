import controlP5.*;
import processing.net.*;
PWindow win;

//Declare Client
Client myClient = new Client(this, "192.168.1.13", 80);

//Declare String to be read from Arduino
String data;

char charIn;
PFont f;
  
float value;
String[] fontlist = PFont.list();
String Case;
String Casein;

//Create roll, pitch and yaw variables
float yaw = 0.0;
float pitch = 0.0;
float roll = 0.0;

//Declare GUI Description Strings
String obstacle_text = "Following Buggy";
String acceleration_string = "Holding Speed";

//Make first window
public void settings() {
  size(500, 500); 
  smooth();
}

void setup() {
  //PWindow Creates a second window to show 3D real-time buggy graphic
  win = new PWindow();
  f = createFont("Impact", 16, true);
}

void draw() {
  background(10, 20, 30);
  textFont(f, 32); //added font
  fill(0); //font colour
  
textSize(50);
  stroke(255);
  fill(255);
  text("GROUP Z06", 150, 100); //title

  //GUI Draw
  textSize(40);
  stroke(#FFFFFF);
  fill(#FFFFFF);
  text("Roll = ", 100, 325);
  text("Yaw = ", 100, 375);
  text("Pitch = ", 100, 425);
  text(roll, 250, 325);
  text(yaw, 250, 375);
  text(pitch, 250, 425);

//Read in data from arduino
  data = myClient.readString();
}

//class of PWindow created to display 3D graphic of buggy using object-oriented programming
class PWindow extends PApplet {
  PWindow() {
    super();
    PApplet.runSketch(new String[] {this.getClass().getSimpleName()}, this);
  }

  void settings() {
    //Creates second window in the 3D plane
    size(500, 500, P3D);
  }

  void setup() {
  }

  void draw() {
    lights();
    background(255);
    int newLine = 13; 
    if(myClient.available() > 0){
do {

    Case = myClient.readStringUntil(newLine); // read from port until new line

    if (Case != null) {

      String[] list = split(trim(Case), " ");

      if (list.length >= 4 && list[0].equals("Orientation:")) {

        yaw = float(list[1]); // convert to float yaw

        pitch = float(list[2]); // convert to float pitch

        roll = float(list[3]); // convert to float roll
      }
    }
  } while (Case != null);
}
  translate(width/2,height/2);
  
  //creating matrix to draw Buggy
  pushMatrix();
          
  float c1 = cos(radians(roll));
  float s1 = sin(radians(roll));
  float c2 = cos(radians(pitch));
  float s2 = sin(radians(pitch));
  float c3 = cos(radians(yaw));
  float s3 = sin(radians(yaw));
  applyMatrix( c2*c3, s1*s3+c1*c3*s2, c3*s1*s2-c1*s3, 0,
               -s2, c1*c2, c2*s1, 0,
               c2*s3, c1*s2*s3-c3*s1, c1*c3+s1*s2*s3, 0,
               0, 0, 0, 1);

  drawArduino();

  popMatrix(); // end of object

   //Print values to console
  print(roll);
  print("\t");
  print(pitch);
  print("\t");
  print(yaw);
  println();
  }
  
  /* function contains shape(s) that are to be rotated with the IMU */
  void drawArduino() {
   stroke(0, 90, 90); //main breadboard
    fill(255, 255, 204); 
    box(300, 10, 200); 
  
    stroke(0); //front breadboard
    fill(255,255,204); 
    translate(-155, 70, 0); 
    box(10, 150, 200); 
    
    stroke(0); //left motor
    fill(255,255,0); 
    translate(105, -27, 75); 
    box(200, 75, 50); 
    
    translate(0, 0, -150); //right motor
    box(200, 75, 50); 
    
    noFill();
    strokeWeight(10);
    translate(-40,0,180);//left wheel
    circle(0,0,240);
    fill(255,255,0);
    circle(0,0,50);
    strokeWeight(1);
    box(1,240,1);
    box(240,1,1);
    
    noFill();
    strokeWeight(10);
    translate(0,0,-210);//right wheel
    circle(0,0,240);
    fill(255,255,0);
    circle(0,0,50);
    strokeWeight(1);
    box(1,240,1);
    box(240,1,1);
    
    fill(0,0,204);//arduino nano
    translate(100,-50,105);
    box(20,5,100);
    
    translate(-130,0,0);//sonar
    box(10,76,100);
    
    translate(250,72,0);//back wheel holder
    fill(0,0,0);
    box(40,120,40);
    
    translate(0,60,0);//back wheel ball
    fill(224,224,224);
    sphere(25);
    
    translate(-130,-80,0);//battery
    fill(255,255,255);
    box(290,75,40);
    
    translate(-200,0,50);//infrared sensor
    fill(0,0,204);
    box(100,5,20);
    
    translate(0,0,-100);//infrared sensor
    box(100,5,20);
  }
}
