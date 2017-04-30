/*
 * 1D-SS-Servo.c
 *
 * Created: 3/28/2017 10:02:40 AM
 * Author : root
 */ 
#define __DELAY_BACKWARD_COMPATIBLE__
#define MAX 1023
#define CCW 1023
#define CW 2047

typedef enum {WHEEL,JOINT, TRUESIN, WHEELRAND,TEST} m;
static const char *MODE_STRING[] = {"WHEEL", "JOINT", "TRUESIN", "WHEELRAND","TEST"};
m mode=TRUESIN;	
//#define F_CPU 1200000UL
#define F_CPU 16000000UL
#define DEFAULT_BAUDNUM 1 
#define NUM_ACTUATOR 2
#define STEP_THETA			(M_PI / 50.0f) // Large value is more fast
#define CONTROL_PERIOD		(10) // msec (Large value is more slow)

#include <stdio.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>
#define d2r(x) x*M_PI/180.0
#define r2d(x) x*180.0/M_PI

#include "dynamixel.h"
#include "serial.h"
#include "AX12A.h"

double t;
double dt;
int servoId1=0;
int servoId2=1;
int currentSpeed=0; //0-1023 = ccw direction 1024-2047= cw;
int timer=0;
//float percentChange=.10;
//int randSpeedChange = 2*percentChange*1023; //multiply by 2 to allow positive and negative change by percent change value

int randSpeedChange = .2*1023; //10% change
int delayTime;
double angfreq;
int cs1=0;
int cs2=0;
int timerSwitch=9;
//int centeredPos=488; //478



//wheel mode functions
void setWheelModeAddresses();
int changeSpeed(int speedIn,int changeAmount,int id);
int changeSpeedPercent(int speedIn,int changeAmount,int id);
int keepWithinBounds(int input,int minB, int maxB);
int keyboardInputSpeed(int speed);
void keyboardInputChangePhase();

//joint mode functions
void setJointModeAddresses();
void setPositionAng(int id, double ang);
void setPositionRad(int id, double rad);
void firstMove();

void PrintCommStatus(int CommStatus);
void PrintErrorCode(void);

int id[NUM_ACTUATOR]={0, 1};
float phase[NUM_ACTUATOR]={-M_PI/2,0};
int centeredPos[NUM_ACTUATOR]={483, 488};
int GoalPos;
float theta=0;
int AmpPos = 310;
int main(void)
{

	//int centeredPos=488; //478
	delayTime=200;
	
	int i;
	int CommStatus;
	t=0;
	dt=0.0005;
	//joint = true, wheel = false
	char str[20];
	serial_initialize(57600);
	dxl_initialize( 0, DEFAULT_BAUDNUM ); 
	sei();	// Interrupt Enable
	
	printf( "\n\nStarting 1D SuperSmarticle Servo\n\n" );
	srand(time(NULL));
	
	//go to an initial position, all styles do this!
	setJointModeAddresses();
	//true position of center is 150, do everything about that point but I offset in function by 60 degrees so middle = 90 deg
	//setPositionRad(servoId1,phase[servoId1]);
	//setPositionRad(servoId2,phase[servoId2]);
		////rad=rad+M_PI/3;
		//M_PI/180/0.005061
			//dxl_write_word(servoId2, GOAL_POSITION_L, centeredPos+phase[servoId2]/.005061);
				printf("Currently running %s mode\n\n",MODE_STRING[mode]);
				_delay_ms(300);
				printf( "\n temps:(%d,%d)",dxl_read_word( servoId1, PRESENT_TEMP ), dxl_read_word( servoId2, PRESENT_TEMP ) );
				printf( "\n phase:(%e,%e)\n",phase[servoId1],phase[servoId2]);
	firstMove();
	//dxl_write_word(servoId1, GOAL_POSITION_L, centeredPos[servoId1]+((int)(r2d(phase[servoId1]))%180/0.29));
	//dxl_write_word(servoId2, GOAL_POSITION_L, centeredPos[servoId2]+((int)(r2d(phase[servoId2]))%180/0.29));
	
	_delay_ms(600);

	int switchPhase=0;
	if(switchPhase==0)
	{
			printf("Press any key then enter to continue:\n\n");
			scanf("%s", str);
	}
	
	switch(mode)
	{
		case JOINT: case TRUESIN:
		{//joint
			currentSpeed=1011; //= 11.7518 rad/s   .111 rpm per unit
			angfreq= 11.7518;
			//angfreq= 10;
			
			if(switchPhase)
			{
				keyboardInputChangePhase();
				dxl_write_word(servoId1, GOAL_POSITION_L, centeredPos[servoId1]+((int)(r2d(phase[servoId1]))%180/0.293));
				dxl_write_word(servoId2, GOAL_POSITION_L, centeredPos[servoId2]+((int)(r2d(phase[servoId2]))%180/0.293));
				_delay_ms(500);
			}
			break;
		}
		case WHEEL: case WHEELRAND:
		{//wheel
			currentSpeed=CW;
			setWheelModeAddresses();
			printf("\n(W,S)= \t(+10,-10)\n(ESC,Q,E) = \t(0,512,1023)\n");
			printf("Set speed: S1=%d\tS2=%d\r\n", currentSpeed,currentSpeed);	
			break;
		}
	
		case TEST:
		{//wheel
			currentSpeed=1011; //= 11.7518 rad/s   .111 rpm per unit
			angfreq= 11.7518;
			//float period = 2*M_PI/angfreq;
			break;
		}
    }
					//printf("Press enter to continue:\n\n");
					//scanf("%s", str);
	
	
	//loop
	while (1)
	{	
		switch(mode)
		{
			case WHEEL:
			{		
				///for keyboard update/////
				//currentSpeed=keyboardInputSpeed(currentSpeed);
			
				//for regular update//////
				//currentSpeed=MAX;
				
				cs1=currentSpeed; cs2=currentSpeed;
				dxl_write_word(servoId1, MOVING_SPEED_L, cs1 );
				dxl_write_word(servoId2, MOVING_SPEED_L, 0 );
				break;
			}
			case WHEELRAND:
			{			
				timer++;
				if (timer>timerSwitch)
				{
					cs1=changeSpeedPercent(currentSpeed,rand()%randSpeedChange-randSpeedChange/2,servoId1);
					cs2=changeSpeedPercent(currentSpeed,rand()%randSpeedChange-randSpeedChange/2,servoId2);
					timer=0;
					printf("current speed: S1=%d\t S2=%d\r\n", cs1,cs2);
				}
				else
				{
				
					///for keyboard update/////
					//currentSpeed=keyboardInput(currentSpeed);
				
					//for regular update//////
					currentSpeed=MAX;
				
					cs1=currentSpeed; cs2=currentSpeed;
					dxl_write_word(servoId1, MOVING_SPEED_L, cs1 );
					dxl_write_word(servoId2, MOVING_SPEED_L, cs2 );

				}
				//_delay_ms(1000);
				break;
			}
			case JOINT:
			{
				dt=0.0005;
				
				//old way of doing joint
				//double rad = M_PI/2*sin(angfreq*t+phase[servoId1])+M_PI/2;
				//double rad2 = M_PI/2*sin(angfreq*t+phase[servoId2])+M_PI/2;
				//setPositionRad(servoId1,rad);
				//setPositionRad(servoId2,rad2);
				//t = t+dt;
				//_delay_ms(dt*1000);
				
				dxl_set_txpacket_id(BROADCAST_ID);
				dxl_set_txpacket_instruction(INST_SYNC_WRITE);
				dxl_set_txpacket_parameter(0, GOAL_POSITION_L);
				dxl_set_txpacket_parameter(1, 2);
				for( i=0; i<NUM_ACTUATOR; i++ )
				{
					//-1 to +1 saw tooth wave
					
					dxl_set_txpacket_parameter(2+3*i, id[i]);
					//goes between +AmpPos and -AmpPos
					
					GoalPos = (int)(AmpPos*sin(angfreq*t+ phase[i]))+centeredPos[i]; 
					
					//printf( "%d  ", GoalPos );
					dxl_set_txpacket_parameter(2+3*i+1, dxl_get_lowbyte(GoalPos));
					dxl_set_txpacket_parameter(2+3*i+2, dxl_get_highbyte(GoalPos));
					
				}
				dxl_set_txpacket_length((2+1)*NUM_ACTUATOR+4);
				
				//printf( "\n" );
				
				dxl_txrx_packet();
				//CommStatus = dxl_get_result();
				//int moving =1;
				//while(moving==1)
				//{
				//moving =dxl_read_byte( servoId1, MOVING );
				//}
				
				//PrintErrorCode();
				//else
				//PrintCommStatus(CommStatus);
				t = t+dt;
				_delay_ms(dt*1000);


				break;
			}
			case TRUESIN: //NOT DONE!
			{
				//unit of speed = .111 rpm or .01162 rad/s
				//max= 11.89 rad/s
				

				dxl_set_txpacket_id(BROADCAST_ID);
				dxl_set_txpacket_instruction(INST_SYNC_WRITE);
				dxl_set_txpacket_parameter(0, GOAL_POSITION_L);
				dxl_set_txpacket_parameter(1, 2);
				for( i=0; i<NUM_ACTUATOR; i++ )
				{
					//-1 to +1 saw tooth wave
					
					dxl_set_txpacket_parameter(2+3*i, id[i]);
					//goes between +AmpPos and -AmpPos
					//https://en.wikipedia.org/wiki/Triangle_wave
					GoalPos = (int)(2*AmpPos/M_PI*asin(sin(angfreq*t-phase[i]))) +centeredPos[i];
					//printf( "%d  ", GoalPos );
							//dxl_write_word(id[i], GOAL_POSITION_L, GoalPos );
							//dxl_write_word(servoId2, GOAL_POSITION_L, speed );
							
					dxl_set_txpacket_parameter(2+3*i+1, dxl_get_lowbyte(GoalPos));
					dxl_set_txpacket_parameter(2+3*i+2, dxl_get_highbyte(GoalPos));
					
				}
			//
				//dxl_set_txpacket_parameter(2, id[0]);
				//GoalPos = (int)(2*AmpPos/M_PI*abs(asin(sin(angfreq*t/2-phase[0]))) +centeredPos[0]);
				//dxl_set_txpacket_parameter(3, dxl_get_lowbyte(GoalPos));
				//dxl_set_txpacket_parameter(4, dxl_get_highbyte(GoalPos));
				//dxl_set_txpacket_parameter(5, id[1]);
				//GoalPos = (int)(2*AmpPos/M_PI*asin(sin(angfreq*t-phase[1]))) +centeredPos[1];
				//dxl_set_txpacket_parameter(6, dxl_get_lowbyte(GoalPos));
				//dxl_set_txpacket_parameter(7, dxl_get_highbyte(GoalPos));
				
				
				dxl_set_txpacket_length((2+1)*NUM_ACTUATOR+4);
				dxl_txrx_packet();
				
				t = t+dt;
				_delay_ms(dt*1000);

				break;
			}
			case TEST://TRY DOING REGWRITE NEXT
			{
				
				dxl_set_txpacket_id(BROADCAST_ID);
				dxl_set_txpacket_instruction(INST_SYNC_WRITE);
				dxl_set_txpacket_parameter(0, GOAL_POSITION_L);
				dxl_set_txpacket_parameter(1, 2);
				for( i=NUM_ACTUATOR-1; i<=0; i-- )
				{
					dxl_set_txpacket_parameter(2+3*i, id[i]);
					//goes between +AmpPos and -AmpPos
					GoalPos = (int)(sin(theta-phase[i]) * (float)AmpPos+(float)centeredPos[i]);
					printf( "%d  ", GoalPos );
					dxl_set_txpacket_parameter(2+3*i+1, dxl_get_lowbyte(GoalPos));
					dxl_set_txpacket_parameter(2+3*i+2, dxl_get_highbyte(GoalPos));
				}
				dxl_set_txpacket_length((2+1)*NUM_ACTUATOR+4);
				
				printf( "\n" );
				
				dxl_txrx_packet();
				CommStatus = dxl_get_result();
				if( CommStatus == COMM_RXSUCCESS )
				PrintErrorCode();
				else
				PrintCommStatus(CommStatus);
				
				theta += STEP_THETA;

				if( theta > 2*M_PI )
				theta -= 2*M_PI;
				
				_delay_ms(CONTROL_PERIOD);
				
				break;
			}
		}
		
}
	return 1;
}
void firstMove()
{
dxl_set_txpacket_id(BROADCAST_ID);
dxl_set_txpacket_instruction(INST_SYNC_WRITE);
dxl_set_txpacket_parameter(0, GOAL_POSITION_L);
dxl_set_txpacket_parameter(1, 2);
for( int i=0; i<NUM_ACTUATOR; i++ )
{
	//-1 to +1 saw tooth wave
	
	dxl_set_txpacket_parameter(2+3*i, id[i]);
	//goes between +AmpPos and -AmpPos
	//https://en.wikipedia.org/wiki/Triangle_wave
	GoalPos = (int)(2*AmpPos/M_PI*asin(sin(angfreq*t-phase[i]))) +centeredPos[i];
	//printf( "%d  ", GoalPos );
	//dxl_write_word(id[i], GOAL_POSITION_L, GoalPos );
	//dxl_write_word(servoId2, GOAL_POSITION_L, speed );
	
	dxl_set_txpacket_parameter(2+3*i+1, dxl_get_lowbyte(GoalPos));
	dxl_set_txpacket_parameter(2+3*i+2, dxl_get_highbyte(GoalPos));
	
}
dxl_set_txpacket_length((2+1)*NUM_ACTUATOR+4);
dxl_txrx_packet();


}
//initialize wheel mode system addresses
void setWheelModeAddresses()
{
	//set to wheel mode
	dxl_write_word(servoId1, ID,servoId1);
	dxl_write_word(servoId1, BAUDRATE,DEFAULT_BAUDNUM);
	dxl_write_word(servoId1, CW_ANGLE_LIMIT_L, 0);
	dxl_write_word(servoId1, CCW_ANGLE_LIMIT_L, 0 );
	dxl_write_word(servoId1, MOVING_SPEED_L, currentSpeed );
	dxl_write_word(servoId1, MAX_TORQUE_L, MAX);
	dxl_write_word(servoId1, TORQUE_LIMIT_L, MAX);
	dxl_write_word(servoId1, PUNCH_L, 32);
	dxl_write_word(servoId1, CW_COMPLIANCE_SLOPE, 2);
	dxl_write_word(servoId1, CCW_COMPLIANCE_SLOPE, 2);
	dxl_write_word(servoId1, CW_COMPLIANCE_MARGIN, 1);
	dxl_write_word(servoId1, CCW_COMPLIANCE_MARGIN, 1);
	
	dxl_write_word(servoId2, ID, servoId2);
	dxl_write_word(servoId2, BAUDRATE,DEFAULT_BAUDNUM);
	dxl_write_word(servoId2, CW_ANGLE_LIMIT_L, 0);
	dxl_write_word(servoId2, CCW_ANGLE_LIMIT_L, 0 );
	dxl_write_word(servoId2, MOVING_SPEED_L, currentSpeed );
	dxl_write_word(servoId2, MAX_TORQUE_L, MAX);
	dxl_write_word(servoId2, TORQUE_LIMIT_L, MAX);
	dxl_write_word(servoId2, PUNCH_L, 32);
	dxl_write_word(servoId2, CW_COMPLIANCE_SLOPE, 2);
	dxl_write_word(servoId2, CCW_COMPLIANCE_SLOPE, 2);
	dxl_write_word(servoId2, CW_COMPLIANCE_MARGIN, 1);
	dxl_write_word(servoId2, CCW_COMPLIANCE_MARGIN, 1);
		
}

void setJointModeAddresses()
{
	//.111 rads/s per index
	currentSpeed=1011; //= 11.7518 rad/s
	cs1=currentSpeed; cs2=currentSpeed;
	
	//set to wheel mode
	dxl_write_word(servoId1, ID,servoId1);
	dxl_write_word(servoId1, BAUDRATE,DEFAULT_BAUDNUM);
	dxl_write_word(servoId1, CW_ANGLE_LIMIT_L, 0);
	dxl_write_word(servoId1, CCW_ANGLE_LIMIT_L, MAX );
	dxl_write_word(servoId1, MOVING_SPEED_L, currentSpeed );
	dxl_write_word(servoId1, MAX_TORQUE_L, MAX);
	dxl_write_word(servoId1, TORQUE_LIMIT_L, MAX);
	dxl_write_word(servoId1, PUNCH_L, 32);
	dxl_write_word(servoId1, CW_COMPLIANCE_SLOPE, 2);
	dxl_write_word(servoId1, CCW_COMPLIANCE_SLOPE, 2);
	dxl_write_word(servoId1, CW_COMPLIANCE_MARGIN, 1);
	dxl_write_word(servoId1, CW_COMPLIANCE_MARGIN, 1);
	
	dxl_write_word(servoId2, ID, servoId2);
	dxl_write_word(servoId2, BAUDRATE,DEFAULT_BAUDNUM);
	dxl_write_word(servoId2, CW_ANGLE_LIMIT_L, 0);
	dxl_write_word(servoId2, CCW_ANGLE_LIMIT_L, MAX );
	dxl_write_word(servoId2, MOVING_SPEED_L, currentSpeed );
	dxl_write_word(servoId2, MAX_TORQUE_L, MAX);
	dxl_write_word(servoId2, TORQUE_LIMIT_L, MAX);
	dxl_write_word(servoId2, PUNCH_L, 32);
	dxl_write_word(servoId2, CW_COMPLIANCE_SLOPE, 2);
	dxl_write_word(servoId2, CCW_COMPLIANCE_SLOPE, 2);
	dxl_write_word(servoId2, CW_COMPLIANCE_MARGIN, 1);
	dxl_write_word(servoId2, CW_COMPLIANCE_MARGIN, 1);
}

//set position of servo in rads
void setPositionRad(int id, double rad)
{
	//rad=rad+M_PI/3;
	rad=rad+d2r(150);
	int add=rad/0.005061;
	dxl_write_word(id, GOAL_POSITION_L, add);
}

//set position of servo in ang
void setPositionAng(int id, double ang)
{	
	ang=ang+150;
	int add=ang/.29;
	dxl_write_word(id, GOAL_POSITION_L, add);
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
int keyboardInputSpeed(int speed)
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

void keyboardInputChangePhase()
{	
	//char str[20];
	printf("1=(0,0) 2=(p,0),3=(p2,0),4=(-p2,0)\n");
	unsigned char ReceivedData = getchar();
	
	switch(ReceivedData)
	{
		case '1':
			phase[servoId1]=0;
			phase[servoId2]=0;
		break;
		case '2':
			phase[servoId1]=M_PI;
			phase[servoId2]=0;
		break;
		case '3':
			phase[servoId1]=M_PI_2;
			phase[servoId2]=0;
		break;
		case '4':
			phase[servoId1]=-M_PI_2;
			phase[servoId2]=0;
		break;
		default:
			
		break;
	}
	printf("phase=%s",&ReceivedData);
	//phase[0]=a*M_PI;
	//printf ("Enter x for servo 2 phase (phi=x*pi):\n");
	//scanf("%f",&str);
	//phase[1]=a*M_PI;
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