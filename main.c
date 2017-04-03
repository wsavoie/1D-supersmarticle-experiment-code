/*
 * 1D-SS-Servo.c
 *
 * Created: 3/28/2017 10:02:40 AM
 * Author : root
 */ 
#define __DELAY_BACKWARD_COMPATIBLE__

#include <stdio.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL

#include <math.h>
#include "dynamixel.h"
#include "serial.h"
#include "AX12A.h"
#include <util/delay.h>
#include <avr/interrupt.h>


void PrintCommStatus(int CommStatus);
void PrintErrorCode(void);
int keepWithinBounds(int input);
int servoId1=0;
int servoId2=1;
int oldV=300;
int inputV=0;
int currentSpeed=0;
int timer=0;
int randSpeedChange = .2*1024; //change by 10%
int delayTime = 200; 
int cs1=0;
int cs2=0;
int timerSwitch=9;
int main(void)
{

	serial_initialize(57600);
	dxl_initialize( 0, 1 ); 
	sei();	// Interrupt Enable
	
	printf( "\n\nStarting 1D SuperSmarticle Servo\n\n" );
    
		srand(time(NULL));
	
	//set to wheel mode
	dxl_write_word(servoId1, CW_ANGLE_LIMIT_L, 0);
	dxl_write_word(servoId1, CCW_ANGLE_LIMIT_L, 0 );
	dxl_write_word(servoId1, MOVING_SPEED_L, currentSpeed );
	dxl_write_word(servoId1, MAX_TORQUE_L, 1023);
	dxl_write_word(servoId1, TORQUE_LIMIT_L, 1023);
	
	dxl_write_word(servoId2, CW_ANGLE_LIMIT_L, 0);
	dxl_write_word(servoId2, CCW_ANGLE_LIMIT_L, 0 );
	dxl_write_word(servoId2, MOVING_SPEED_L, currentSpeed );
	dxl_write_word(servoId2, MAX_TORQUE_L, 1023);
	dxl_write_word(servoId2, TORQUE_LIMIT_L, 1023);
	
	
	
	printf("\n(W,S)= \t(+10,-10)\n(ESC,Q,E) = \t(0,512,1023)\n");
	//scanf("%d",&inputV);
	//if(oldV!=inputV)
	//{
		////sanitize inputs
		//inputV=inputV<0 ? 0:inputV;
		//inputV=inputV>1023 ? 1023 : inputV;
		//
		//oldV=inputV;
		//dxl_write_word(servoId, MOVING_SPEED_L, inputV );
	//}
	

	
		currentSpeed=1023;
	printf("Set speed: S1=%d\tS2=%d\r\n", currentSpeed,currentSpeed);
    while (1) 
    {
		currentSpeed=1023;
		//unsigned char ReceivedData = getchar();
		//if(ReceivedData ==  'w')
			//currentSpeed=currentSpeed+10;
		//else if(ReceivedData == 's')
			//currentSpeed=currentSpeed-10;
		//else if(ReceivedData == 0x1b) //esc
			//currentSpeed=0;
		//else if(ReceivedData == 'q') 
			//currentSpeed=512;
		//else if(ReceivedData == 'e') 
			//currentSpeed=1023;
		//else
			//currentSpeed=currentSpeed;
		timer++;
		if (timer>timerSwitch)
		{
			cs1=currentSpeed+(rand())%randSpeedChange-randSpeedChange/2;
			cs2=currentSpeed+(rand())%randSpeedChange-randSpeedChange/2;
			timer=0;
			
					cs1=keepWithinBounds(cs1);
					cs2=keepWithinBounds(cs2);
					
			printf("current speed: S1=%d\t S2=%d\r\n", cs1,cs2);
		}
		else
		{
			cs1=currentSpeed;
			cs2=currentSpeed;
		}
		cs1=keepWithinBounds(cs1);
		cs2=keepWithinBounds(cs2);
		
		
		dxl_write_word(servoId1, MOVING_SPEED_L, cs1 );
		dxl_write_word(servoId2, MOVING_SPEED_L, cs2 );
		//printf("current speed: S1=%d\t S2=%d\n", cs1,cs2);


		_delay_ms(delayTime);

		
    }
	return 1;
}

int keepWithinBounds(int input)
{
		input=input<0 ? 0:input;
		input=input>1023 ? 1023 : input;
		return input;
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