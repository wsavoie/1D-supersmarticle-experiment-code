    import processing.serial.*;
    Serial arduinoPort;
    Serial robotisPort;
    int time;
    float t;
    PrintWriter output;
  PImage[] state=new PImage[7];
  int[] locx= new int[7];
  int[] locy= new int[7];
  float pi=3.1415;
  int h=300; int w=640;
  int backCol=51;  
  int bufferOld=1;
    String phases="";
  int systemState=0;
  boolean cont=false;
  //  printf("49=(0,0) 50=(pi,0),51=(pi/2,0),52=(-pi/2,0)\n");
  
  int phase=51;
  String name="s=3";
  int version=2;
    void setup() {
      time=1000;
      switch(phase){
      case 49://(0,0)      
        phases="_pa=0_pb=0_";
      break;
      case 50://(pi,0)      
        phases="_pa=1_pb=0_";
      break;     
      case 51://(pi/2,0)   
        phases="_pa=0.5_pb=0_";
      break;      
      case 52://(-pi/2,0)        
       phases="_pa=-0.5_pb=0_";
      break;
      
      }
      name=name+phases+"v="+version;

      background(backCol);
      surface.setTitle("state visualizer");
     
      size(640,330);
    
      locx[0] = 1*w/10;  locx[1] = 1*w/10;
      locx[2] = 4*w/10;  locx[3] = 4*w/10;
      locx[4] = 7*w/10; locx[5] = 7*w/10; locx[6] = 7*w/10;
     
      locy[0] = 2*h/10; locy[2] = 2*h/10;  locy[4] = 2*h/10;
      locy[1] = 5*h/10; locy[3] = 5*h/10;  locy[5] = 5*h/10;
      locy[6] = 8*h/10;
  
  
      for(int i=1;i<=7;i++)
      {
        state[i-1]=loadImage(i+".png");
        image(state[i-1],locx[i-1],locy[i-1]);
      }


       printArray(Serial.list());  
       startRobotis();
       while(cont)
       {
         if(keyPressed)
           {cont=true;}
       }
       startArduino();
         

        
  
    }
    void draw() {
       byte[] inBuffer = new byte[7];
       noFill();
       
      strokeWeight(2);
      int count = 0;
       while (arduinoPort.available() > 0) 
       {
         
         inBuffer = arduinoPort.readBytes();
          t=(millis()-time)/1000.0;
          //system doesn't properly store first .1 seconds in file
          if(t>.1)
         { //<>//
            if(inBuffer!=null)
            {
              System.err.println(inBuffer[0]);
               stroke(backCol); strokeWeight(10);
                rect(locx[bufferOld-1]-10, locy[bufferOld-1]-10, 190,80);
                 stroke(255,0,0); strokeWeight(2);
                 bufferOld=inBuffer[0];
                rect(locx[inBuffer[0]-1]-10, locy[inBuffer[0]-1]-10, 190,80);
                //save("A:\\1DSmartData\\contactVid\\"+count+".tga");
                count++;
            }
            if(systemState==1){
            output.println( inBuffer[0]+"\t"+t );}
         }
       }
  }
       
    void startRobotis()
    {
      try
      {
      robotisPort= new Serial( this, "COM4", 57600 );
      }
      catch(RuntimeException a)
      {
        System.err.println("turn off Roboplus terminal!");
        System.exit(0);
      }
      robotisPort.write(phase);
      return;
    }
    void startArduino()
    {
      try
      {
         arduinoPort = new Serial( this, "COM9", 9600 );
      }
      catch(RuntimeException a)
      {
        System.err.println("plug in arduino!");
        System.exit(0);
      }

      return;
    }
    void keyPressed() {
      
       if (keyCode == 32 && systemState==0)
       {
                output = createWriter( "A:\\1DSmartData\\ContactData\\"+name+".txt" );
         robotisPort.write(65);
        delay(1);
        robotisPort.write(13);
        systemState=1;
        time = millis();//store the current time
        return;
       }
       
        output.flush();  // Writes the remaining data to the file
        output.close();  // Finishes the file
        arduinoPort.clear();
        arduinoPort.stop();
        robotisPort.clear();
        robotisPort.stop();
        exit();  // Stops the program
    }