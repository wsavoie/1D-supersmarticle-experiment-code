    import processing.serial.*;
    Serial arduinoPort;
    Serial robotisPort;
    int time;
    float t;
    PrintWriter output;
    void setup() {
        size(480, 120);

       printArray(Serial.list());
       //arduinoPort = new Serial( this, Serial.list()[1], 9600 );
   
       robotisPort= new Serial( this, "COM4", 57600 );
       

       robotisPort.write(65);
       delay(500);
       robotisPort.write(13);
       arduinoPort = new Serial( this, "COM9", 9600 );
         time = millis();//store the current time
       output = createWriter( "A:\\1DSmartData\\ContactData\\-pi2_s=3_v=3.txt" );
  
    }
    void draw() {
       byte[] inBuffer = new byte[7];
       while (arduinoPort.available() > 0) 
       {
         inBuffer = arduinoPort.readBytes();
          t=(millis()-time)/1000.0;
          if(t>.1)
         { //<>//
            if(inBuffer!=null)
            {
              output.println( inBuffer[0]+"\t"+t );
            }
         }
       }
  }
       
        //if (arduinoPort.available() > 0 ) {
        //     //String value = arduinoPort.readString();
        //     //if ( value != null ) 
        //     //{
        //     //   t=(millis()-time)/1000.0;    
        //     //   output.println( value+"\t"+t );
        //     //}
        //    byte[] value = arduinoPort.readBytes();
        //        t=(millis()-time)/1000.0;    
        //        output.println( value[1]+"\t"+t );
        //}
    //}
    void keyPressed() {
        output.flush();  // Writes the remaining data to the file
        output.close();  // Finishes the file
        exit();  // Stops the program
    }