/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/**
 * @file  robot.c
 *
 * @brief  Create the robot object with its motors and sensors, make its wheels move.
 *
 * @author Joshua Montreuil
 * @date 26-01-2023
 * @version 1.2
 * @section License
 *
 * The MIT License
 *
 * Copyright (c) 2023, Joshua Montreuil
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

/* ----------------------  INCLUDES  ---------------------------------------- */
#include "robot.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
/* ----------------------  PRIVATE CONFIGURATIONS  -------------------------- */
#define LEFT_MOTOR MD
#define RIGHT_MOTOR MA
#define LIGHT_SENSOR S1
#define FRONT_BUMPER S3
#define FLOOR_SENSOR S2
/* ----------------------  PRIVATE TYPE DEFINITIONS  ------------------------ */
/* ----------------------  PRIVATE STRUCTURES  ------------------------------ */
struct Robot_t
{
	Motor * rightMotor;
	Motor * leftMotor;
	ContactSensor * FloorSensor;
	ContactSensor * FrontSensor;
	LightSensor * lightSensor;
};
/* ----------------------  PRIVATE ENUMERATIONS  ---------------------------- */
/* ----------------------  PRIVATE VARIABLES  ------------------------------- */
static const char adresse_infox[] = "127.0.0.1";
static const int port = 12345;
/* ----------------------  PRIVATE FUNCTIONS PROTOTYPES  -------------------- */
/* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */
void Robot_start(Robot* pRobot)
{
	if(ProSE_Intox_init(adresse_infox,port) == -1)
	{
		PProseError("The communication with Intox could not had been initialised.");
	}

	//Open the ports of the Sensors
	pRobot->FloorSensor = ContactSensor_open(FLOOR_SENSOR); //floor sensor
	if(pRobot->FloorSensor == NULL)
	{
		PProseError("Error with the instance floor sensor.");
	}
	pRobot->FrontSensor = ContactSensor_open(FRONT_BUMPER); //front bumper
	if(pRobot->FrontSensor == NULL)
	{
		PProseError("Error with the instance front bumper.");
	}
	pRobot->lightSensor = LightSensor_open(LIGHT_SENSOR); //light sensor
	if(pRobot->lightSensor == NULL)
	{
		PProseError("Error with the instance light sensor.");
	}

	//Open the ports of the motors
	pRobot->leftMotor = Motor_open(LEFT_MOTOR); //left motor
	if(pRobot->leftMotor == NULL)
	{
		PProseError("Error with the instance left motor.");
	}
	pRobot->rightMotor = Motor_open(RIGHT_MOTOR); //right motor
	if(pRobot->rightMotor == NULL)
	{
		PProseError("Error with the instance right motor.");
	}
}

void Robot_stop(Robot* pRobot)
{
	ProSE_Intox_close();

	//Closing the motors
	if(Motor_close(pRobot->leftMotor) == -1)
	{
		PProseError("Error while closing left motor.");
	}
	if(Motor_close(pRobot->rightMotor) == -1)
	{
		PProseError("Error while closing right motor.");
	}

	//Closing the sensors
	if(ContactSensor_close(pRobot->FrontSensor) == -1)
	{
		PProseError("Error while closing front sensor.");
	}
	if(ContactSensor_close(pRobot->FloorSensor) == -1)
	{
		PProseError("Error while closing contact sensor.");
	}
	if(LightSensor_close(pRobot->lightSensor) == -1)
	{
		PProseError("Error while closing light sensor.");
	}
}

Robot* Robot_new(void)
{
	Robot* pRobot = (Robot*) malloc(sizeof(Robot));
	if(pRobot == NULL)
	{
		printf("ERROR : pRobot is NULL /n");
		while(1);
	}
	return pRobot;
}

void Robot_free(Robot* pRobot)
{
	printf("Destruction pRobot");
	free(pRobot);
}

void Robot_setWheelsVelocity(Robot* pRobot,int mr,int ml)
{
	if(Motor_setCmd(pRobot->leftMotor,ml) == -1)
	{
		PProseError("The command has not been given to the left motor.");
	}
	if(Motor_setCmd(pRobot->rightMotor,mr) == -1)
	{
		PProseError("The command has not been given to the right motor.");
	}
}

int Robot_getRobotSpeed(Robot* pRobot)
{
	return ((abs(Motor_getCmd(pRobot->leftMotor)) + abs(Motor_getCmd(pRobot->rightMotor))) / 2);
}

SensorState Robot_getSensorState(Robot* pRobot)
{
	SensorState sensorStatus;
	sensorStatus.collision = (ContactSensor_getStatus(pRobot->FloorSensor) || ContactSensor_getStatus(pRobot->FrontSensor))? BUMPED : NO_BUMP;
	sensorStatus.luminosity = LightSensor_getStatus(pRobot->lightSensor);

	return sensorStatus;
}
/* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */



