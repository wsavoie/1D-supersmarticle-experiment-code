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


int servoId1=0;
int servoId2=1;
int currentSpeed=0; //0-1023 = ccw direction 1024-2047= cw;
int timer=0;
float percentChange=.10;
int randSpeedChange = 2*percentChange*1023; //multiply by 2 to allow positive and negative change by percent change value
int delayTime = 200;
int cs1=0;
int cs2=0;
int timerSwitch=9;

//wheel mode functions
void setWheelModeAddresses();
int changeSpeed(int speedIn,int changeAmount,int id);
int changeSpeedPercent(int speedIn,int changeAmount,int id);
int keepWithinBounds(int input,int minB, int maxB);
int keyboardInput(int speed);

//joint mode functions
void setWheelModeAddresses();


void PrintCommStatus(int CommStatus);
void PrintErrorCode(void);


int main(void)
{

	serial_initialize(57600);
	dxl_initialize( 0, 1 ); 
	sei();	// Interrupt Enable
	
	printf( "\n\nStarting 1D SuperSmarticle Servo\n\n" );
	srand(time(NULL));
	
	
	
	printf("\n(W,S)= \t(+10,-10)\n(ESC,Q,E) = \t(0,512,1023)\n");	
	printf("Set speed: S1=%d\tS2=%d\r\n", currentSpeed,currentSpeed);
    while (1) 
    {
		
		
		//////////////////////////////////////////
		//timer++;
		//if (timer>timerSwitch)
		//{
			//cs1=changeSpeedPercent(currentSpeed,rand()%randSpeedChange-randSpeedChange/2);
			//cs2=changeSpeedPercent(currentSpeed,rand()%randSpeedChange-randSpeedChange/2);
			//timer=0;
			//printf("current speed: S1=%d\t S2=%d\r\n", cs1,cs2);
		//}
		//else
		//{
			
			/////for keyboard update/////
			//currentSpeed=keyboardInput(currentSpeed);
			
			////for regular update//////
			//currentSpeed=MAX;
			//cs1=currentSpeed; cs2=currentSpeed;
			//dxl_write_word(servoId1, MOVING_SPEED_L, cs1 );
			//dxl_write_word(servoId2, MOVING_SPEED_L, cs2 );

		//}
			
			
			
		//printf("current speed: S1=%d\t S2=%d\n", cs1,cs2);


		_delay_ms(delayTime);

		
    }
	return 1;
}

//initialize wheel mode system addresses
void setWheelModeAddresses()
{
	currentSpeed=1023;
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
}

void setJointModeAddresses()
{
	currentSpeed=1023;
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
}

//change speed by 10 bit amount
int changeSpeed(int speedIn,int changeAmount, int id)
{
	int speed=speedIn+changeAmount;
	speed=keepWithinBounds(speed,0,MAX);
	dxl_write_word(id, MOVING_SPEED_L, speed );
	return speed;
}

//change speed by percent amount
int changeSpeedPercent(int speedIn,int changeAmount,int id)
{
	int speed = speedIn+changeAmount*MAX/100;
	speed=keepWithinBounds(speed,0,MAX);
	dxl_write_word(id, MOVING_SPEED_L, speed );
	return speed;
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
		speed=changeSpeedPercent(speed,5,servoId1);
		changeSpeedPercent(speed,5,servoId2);
		break;	
	case 's':
		speed=changeSpeedPercent(speed,-5,servoId1);
		changeSpeedPercent(speed,-5,servoId2);
		break;
	case 0x1b: //esc
		speed=0;
		dxl_write_word(servoId1, MOVING_SPEED_L, speed );
		dxl_write_word(servoId2, MOVING_SPEED_L, speed );
		break;
	case 'q':
		speed=MAX/2;
		dxl_write_word(servoId1, MOVING_SPEED_L, speed );
		dxl_write_word(servoId2, MOVING_SPEED_L, speed );
		break;
	case 'e':
		speed=MAX;
		dxl_write_word(servoId1, MOVING_SPEED_L, speed );
		dxl_write_word(servoId2, MOVING_SPEED_L, speed );
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