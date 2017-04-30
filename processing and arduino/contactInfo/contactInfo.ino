int con0 = 49; //pc14
int con1 = 50; //pc13
int con2 = 51; //pc12
int val0 = 0;
int val1 = 0;
int val2 = 0;
int fVal = 0;
void setup() {
  // put your setup code here, to run once:
	pinMode(con0, INPUT);
	pinMode(con1, INPUT); 
	pinMode(con2, INPUT); 
	digitalWrite(con0, HIGH);       // turn on pullup resistors
	digitalWrite(con1, HIGH);       // turn on pullup resistors
	digitalWrite(con2, HIGH);       // turn on pullup resistors
	Serial.begin(9600);
	// Serial.println("");
	
	
}

void loop() {
// val0= PIOC->PC14;   
// !!(PIOD->PIO_PDSR & (1<<8))
// PIOB->PIO_SODR=1<<27; //lights the LED
// PIOB->PIO_CODR=1<<27; //clears it
// boolean p12=!!(PIOD->PIO_PDSR & (1<<8)); //read pin 12


val0 = digitalRead(con0)*100;   // read the input pin
val1 = digitalRead(con1)*10;   // read the input pin
val2 = digitalRead(con2)*1;   // read the input pin

fVal=val0+val1+val2;
// Serial.println(fVal);
//0 means contact
switch(fVal)
{
	case 1://state 1
		Serial.println("1");
		break;
	case 11://state 2
		Serial.println("2");
		break;
	case 100://state 3
		Serial.println("3");
		break;
	case 110://state 4
		Serial.println("4");
		break;
	case 101://state 5
		Serial.println("5");
		break;
	case 111://state 6
		Serial.println("6");
		break;
	case 10://state 7
		Serial.println("7");
		break;
	default:
		Serial.println("0");
		break;
	}
	delay(100);
}
