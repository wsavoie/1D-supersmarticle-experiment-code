/*
 * 1D-SS-Servo.c
 *
 * Created: 3/28/2017 10:02:40 AM
 * Author : root
 */ 
#define __DELAY_BACKWARD_COMPATIBLE__

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include <math.h>
#include "dynamixel.h"
#include "serial.h"
#include "AX12A.h"



void PrintCommStatus(int CommStatus);
void PrintErrorCode(void);

int servoId1=0;
int servoId2=1;
int oldV=300;
int inputV=0;
int currentSpeed=0;

int main(void)
{

	serial_initialize(57600);
	dxl_initialize( 0, 1 ); 
	sei();	// Interrupt Enable
	
	printf( "\n\nStarting 1D SuperSmarticle Servo\n\n" );
    

	
	//set to wheel mode
	dxl_write_word(servoId1, CW_ANGLE_LIMIT_L, 0);
	dxl_write_word(servoId1, CCW_ANGLE_LIMIT_L, 0 );
	dxl_write_word(servoId1, MOVING_SPEED_L, currentSpeed );
	
		dxl_write_word(servoId2, CW_ANGLE_LIMIT_L, 0);
		dxl_write_word(servoId2, CCW_ANGLE_LIMIT_L, 0 );
		dxl_write_word(servoId2, MOVING_SPEED_L, currentSpeed );
	
	printf("\n(W,S)= \t(+10,-10)\n(ESC,Q,E) = \t(0,512,1023)\n");
	scanf("%d",&inputV);
	//if(oldV!=inputV)
	//{
		////sanitize inputs
		//inputV=inputV<0 ? 0:inputV;
		//inputV=inputV>1023 ? 1023 : inputV;
		//
		//oldV=inputV;
		//dxl_write_word(servoId, MOVING_SPEED_L, inputV );
	//}
	

	
	
	
    while (1) 
    {
		
		unsigned char ReceivedData = getchar();
		if(ReceivedData ==  'w')
			currentSpeed=currentSpeed+10;
		else if(ReceivedData == 's')
			currentSpeed=currentSpeed-10;
		else if(ReceivedData == 0x1b) //esc
			currentSpeed=0;
		else if(ReceivedData == 'q') 
			currentSpeed=512;
		else if(ReceivedData == 'e') 
			currentSpeed=1023;
		//sanitize inputs
		currentSpeed=currentSpeed<0 ? 0:currentSpeed;
		currentSpeed=currentSpeed>1023 ? 1023 : currentSpeed;

		
		dxl_write_word(servoId1, MOVING_SPEED_L, currentSpeed );
		dxl_write_word(servoId1, MOVING_SPEED_L, currentSpeed );
		
		dxl_write_word(servoId2, MOVING_SPEED_L, currentSpeed );
		dxl_write_word(servoId2, MOVING_SPEED_L, currentSpeed );
		//currentSpeed= dxl_read_word( servoId, PRESENT_SPEED_L);
		printf("current speed: %d\r\n", currentSpeed);
			
    }
	return 0;
}

// Print communication result
void PrintCommStatus(int CommStatus)
{
	switch(CommStatus)
	{
		case COMM_TXFAIL:
		printf("COMM_TXFAIL: Failed transmit instruction packet!\n");
		break;

		case COMM_TXERROR:
		printf("COMM_TXERROR: Incorrect instruction packet!\n");
		break;

		case COMM_RXFAIL:
		printf("COMM_RXFAIL: Failed get status packet from device!\n");
		break;

		case COMM_RXWAITING:
		printf("COMM_RXWAITING: Now receiving status packet!\n");
		break;

		case COMM_RXTIMEOUT:
		printf("COMM_RXTIMEOUT: There is no status packet!\n");
		break;

		case COMM_RXCORRUPT:
		printf("COMM_RXCORRUPT: Incorrect status packet!\n");
		break;

		default:
		printf("This is unknown error code!\n");
		break;
	}
}

// Print error bit of status packet
void PrintErrorCode()
{
	if(dxl_get_rxpacket_error(ERRBIT_VOLTAGE) == 1)
	printf("Input voltage error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_ANGLE) == 1)
	printf("Angle limit error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_OVERHEAT) == 1)
	printf("Overheat error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_RANGE) == 1)
	printf("Out of range error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_CHECKSUM) == 1)
	printf("Checksum error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_OVERLOAD) == 1)
	printf("Overload error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_INSTRUCTION) == 1)
	printf("Instruction code error!\n");
}