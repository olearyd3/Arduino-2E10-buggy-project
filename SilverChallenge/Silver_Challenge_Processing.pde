import controlP5.*;
  import processing.net.*;
  char charIn;
  
  Button myButton1;
  Button myButton2;
  ControlP5 cp1;
  ControlP5 cp2;
  Client myClient;
  PFont f;
  
  Textarea Telemetry;
  Textarea Distance;
  
  float value;
  String data;
  String[] fontlist = PFont.list();
  
  void setup(){
    size(600, 400);
    cp1 = new ControlP5(this);
     cp2 = new ControlP5(this);
    myClient = new Client(this,"192.168.0.106", 80);
    f = createFont("Impact", 16, true);
    
   myButton1 = cp1.addButton("GO")
    .setValue(0)
    .setPosition(200, 50)
    .setSize(200,50);
    myButton1.setColorBackground(color(66, 255, 87));
    
   myButton2 = cp1.addButton("STOP")
    .setValue(0)
    .setPosition(200, 110)
    .setSize(200,50);
    myButton2.setColorBackground(color(255, 66, 87));
    
    Telemetry = cp1.addTextarea("Obstacle messages") //text area for obstacle messages in GUI
                      .setPosition(50,175)
                      .setSize(500,50)
                      .setFont(createFont("Impact",20))
                      .setLineHeight(14)
                      .setColor(color(128))
                      .setColorBackground(color(255,255,255))
                      .setColorForeground(color(0,0));

      
      Distance = cp2.addTextarea("Distances") //text area for distance values in GUI
                       .setPosition(50, 275)
                       .setSize(500, 50)
                       .setFont(createFont("Impact",25))
                       .setLineHeight(14)
                       .setColor(color(128))
                       .setColorBackground(color(255,255,255))
                       .setColorForeground(color(0,0));
    }
      
      void draw() {
       background(20, 100, 250); //added colour to window
          textFont(f, 32); //added font
          fill(0); //font colour
          textAlign(CENTER); //aligned font
          text("Group Z06", width/2, 30); //font location
          int Case = myClient.read();
          
          if(myClient.available() > 0) {
              if(Case == 20){ 
                Telemetry.setText("Obstacle seen, adjusting speed to avoid collision");
              }
              if(Case > 20){
                Telemetry.setText("No obstacle, continuing at full speed.");
            } 
             if (Case >= 0){
                Distance.setText(String.valueOf(Case)+" cm"); //prints distance in the GUI
              }
          }
       }
      
      public void GO() {
        if (myClient.active()){
        myClient.write("w");
      }
    }
    
    public void STOP() {
      if (myClient.active()){
        myClient.write("s");
      }
    } 
