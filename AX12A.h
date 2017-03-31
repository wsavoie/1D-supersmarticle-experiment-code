/*
 * AX12A.h
 *
 * Created: 3/28/2017 10:28:33 AM
 *  Author: root
 */ 


#ifndef AX12A_H_
#define AX12A_H_

//H is for High bit L is for low bit, for 8(L) and 16(H) bit address
#define MODEL_NUMBER_L					0	//Lowest byte of model number
#define MODEL_NUMBER_H					1	//Highest byte of model number
#define FIRMWARE_VERSION				2	//Information on the version of firmware
#define ID								3	//ID of Dynamixel
#define BAUDRATE						4	//Baud Rate of Dynamixel
#define RETURN_DELAY_TIME				5	//Return Delay Time
#define CW_ANGLE_LIMIT_L				6	//Lowest byte of clockwise Angle Limit
#define CW_ANGLE_LIMIT_H				7	//Highest byte of clockwise Angle Limit	
#define CCW_ANGLE_LIMIT_L				8	//Lowest byte of counterclockwise Angle Limit
#define CCW_ANGLE_LIMIT_H				9	//Highest byte of counterclockwise Angle Limit

#define TEMP_LIMIT						11	//Internal Limit Temperature
#define MIN_VOLTAGE_LIMIT				12	//Lowest Limit Voltage
#define MAX_VOLTAGE_LIMIT				13	//Highest Limit Voltage
#define MAX_TORQUE_L					14	//Lowest byte of Max. Torque
#define MAX_TORQUE_H					15	//Highest byte of Max. Torque
#define STATUS_RETURN_LEVEL				16	//Status Return Level
#define ALARM_LED						17	//LED for Alarm
#define ALARM_SHUTDOWN					18	//Shutdown for Alarm

#define TORQUE_ENABLE					24	//Torque On/Off
#define LED								25	//LED On/Off
#define CW_COMPLIANCE_MARGIN			26	//CW Compliance margin
#define CCW_COMPLIANCE_MARGIN			27	//CCW Compliance margin
#define CW_COMPLIANCE_SLOPE				28	//CW Compliance slope
#define CCW_COMPLIANCE_SLOPE			29	//CCW Compliance slope
#define GOAL_POSITION_L					30	//Lowest byte of Goal Position
#define GOAL_POSITION_H					31	//Highest byte of Goal Position
#define MOVING_SPEED_L					32	//Lowest byte of Moving Speed (Moving Velocity)
#define MOVING_SPEED_H					33	//Highest byte of Moving Speed (Moving Velocity)
#define TORQUE_LIMIT_L					34	//Lowest byte of Torque Limit (Goal Torque)
#define TORQUE_LIMIT_H					35	//Highest byte of Torque Limit (Goal Torque)
#define PRESENT_POSTION_L				36	//Lowest byte of Current Position (Present Velocity)
#define PRESENT_POSTION_H				37	//Highest byte of Current Position (Present Velocity)
#define PRESENT_SPEED_L					38	//Lowest byte of Current Speed
#define PRESENT_SPEED_H					39	//Highest byte of Current Speed
#define PRESENT_LOAD_L					40	//Lowest byte of Current Load
#define PRESENT_LOAD_H					41	//Highest byte of Current Load
#define PRESENT_VOLTAGE					42	//Current Voltage

#define PRESENT_TEMP					43	//Current Temperature
#define REGISTERED						44	//Means if Instruction is registered
#define MOVING							46	//Means if there is any movement
#define LOCK							47	//Locking EEPROM
#define PUNCH_L							48	//Lowest byte of Punch
#define PUNCH_H							49	//Highest byte of Punch

#endif /* AX12A_H_ */