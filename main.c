/*
 * 1D-SS-Servo.c
 *
 * Created: 3/28/2017 10:02:40 AM
 * Author : root
 */ 
#define __DELAY_BACKWARD_COMPATIBLE__
#define MAX 1023

#define F_CPU 8000000UL
#include <stdio.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>


#include "dynamixel.h"
#include "serial.h"
#include "AX12A.h"



int changeSpeed(int speedIn,int changeAmount);
int changeSpeedPercent(int speedIn,int changeAmount);
int keepWithinBounds(int input,int minB, int maxB);
int keyboardInput(int speed);

void PrintCommStatus(int CommStatus);
void PrintErrorCode(void);

int servoId1=0;
int servoId2=1;
int currentSpeed=0; //0-1023 = ccw direction 1024-2047= cw;
int timer=0;
int randSpeedChange = 0.2*1023; //change by 10%    
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
		currentSpeed=950;
		cs1=currentSpeed; cs2=currentSpeed;
		
	//set to wheel mode
	dxl_write_word(servoId1, ID,servoId1);
	dxl_write_word(servoId1, BAUDRATE,1);
	dxl_write_word(servoId1, CW_ANGLE_LIMIT_L, 0);
	dxl_write_word(servoId1, CCW_ANGLE_LIMIT_L, 0 );
	dxl_write_word(servoId1, MOVING_SPEED_L, currentSpeed );
	dxl_write_word(servoId1, MAX_TORQUE_L, MAX);
	dxl_write_word(servoId1, TORQUE_LIMIT_L, MAX);
	
	dxl_write_word(servoId2, ID, servoId2);
	dxl_write_word(servoId2, BAUDRATE,1);
	dxl_write_word(servoId2, CW_ANGLE_LIMIT_L, 0);
	dxl_write_word(servoId2, CCW_ANGLE_LIMIT_L, 0 );
	dxl_write_word(servoId2, MOVING_SPEED_L, currentSpeed );
	dxl_write_word(servoId2, MAX_TORQUE_L, MAX);
	dxl_write_word(servoId2, TORQUE_LIMIT_L, MAX);
	
	
	
	printf("\n(W,S)= \t(+10,-10)\n(ESC,Q,E) = \t(0,512,1023)\n");	
	printf("Set speed: S1=%d\tS2=%d\r\n", currentSpeed,currentSpeed);
    while (1) 
    {
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
		
		//////////////////////////////////////////
		//timer++;
		//if (timer>timerSwitch)
		//{
			//cs1=currentSpeed+(rand())%randSpeedChange-randSpeedChange/2;
			//cs2=currentSpeed+(rand())%randSpeedChange-randSpeedChange/2;
			//timer=0;
			//
					//cs1=keepWithinBounds(cs1);
					//cs2=keepWithinBounds(cs2);
					//
			//printf("current speed: S1=%d\t S2=%d\r\n", cs1,cs2);
		//}
		//else
		//{
			//////////////////////////////////////////////////////
			cs1=currentSpeed;
			cs2=currentSpeed;
		//}
		cs1=keepWithinBounds(cs1);
		cs2=keepWithinBounds(cs2);
		
		
		dxl_write_word(servoId1, MOVING_SPEED_L, cs1 );
		dxl_write_word(servoId2, MOVING_SPEED_L, cs2 );
		//printf("current speed: S1=%d\t S2=%d\n", cs1,cs2);


		_delay_ms(delayTime);

		
    }
	return 1;
}

//change speed by 10 bit amount
int changeSpeed(int speedIn,int changeAmount)
{
return speedIn+changeAmount;
}

//change speed by percent amount
int changeSpeedPercent(int speedIn,int changeAmount)
{
	return speedIn+changeAmount*MAX;
}

//sanitize input value
int keepWithinBounds(int input,int minB,int maxB)
{
		input=input<minB ? minB:input;
		input=input>maxB ? maxB : input;
		return input;
}

//change speed with keyboard input
int keyboardInput(int speed)
{
	unsigned char ReceivedData = getchar();
	switch(ReceivedData)
	{
	case 'w':
		speed=changeSpeedPercent(speed,5);
		break;	
	case 's':
		speed=changeSpeedPercent(speed,-5);
		break;
	case 0x1b: //esc
		speed=0;
		break;
	case 'q':
		speed=MAX/2;
		break;
	case 'e':
		speed=MAX;
		break;
	default:
		speed=speed;
		break;
	}
	return speed;
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