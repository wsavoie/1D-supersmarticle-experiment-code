    import processing.serial.*;
    Serial arduinoPort;
    Serial robotisPort;
    int time;
    PrintWriter output;
    void setup() {
    
       printArray(Serial.list());
       //arduinoPort = new Serial( this, Serial.list()[1], 9600 );
   
       robotisPort= new Serial( this, "COM4", 57600 );
       

       robotisPort.write(65);
       delay(300);
       robotisPort.write(13);
       arduinoPort = new Serial( this, "COM9", 9600 );
         time = millis();//store the current time
       output = createWriter( "A:\\1DSmartData\\ContactData\\data.txt" );
  
    }
    void draw() {
      float t;
        if (arduinoPort.available() > 0 ) {
             String value = arduinoPort.readString();
             if ( value != null ) 
             {
                t=(millis()-time)/1000.0;    
                output.println( value+"\t"+t );
             }
        }
    }
    void keyPressed() {
        output.flush();  // Writes the remaining data to the file
        output.close();  // Finishes the file
        exit();  // Stops the program
    }