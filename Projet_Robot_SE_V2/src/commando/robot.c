/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/**
 * @file  robot.c
 *
 * @brief  Create the robot object with its motors and sensors, make its wheels move.
 *
 * @author Joshua Montreuil
 * @date 26-01-2023
 * @version 2.0
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

//----INCLUDES -----------------------------------------------------------------
#include "robot.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

//---- CONSTANT DEFINITIONS ----------------------------------------------------
#define LEFT_MOTOR MD
#define RIGHT_MOTOR MA
#define LIGHT_SENSOR S1
#define FRONT_BUMPER S3
#define FLOOR_SENSOR S2

//---- GLOBAL VARIABLES --------------------------------------------------------
const char adresse_infox[] = "127.0.0.1";
const int port = 12345;
Robot *probot;

//---- PUBLIC FUNCTIONS --------------------------------------------------------
void Robot_start(void)
{
	if(ProSE_Intox_init(adresse_infox,port) == -1)
	{
		PProseError("The communication with Intox could not had been initialised.");
	}

	//Open the ports of the Sensors
	probot->FloorSensor = ContactSensor_open(FLOOR_SENSOR); //floor sensor
	if(probot->FloorSensor == NULL)
	{
		PProseError("Error with the instance floor sensor.");
	}
	probot->FrontSensor = ContactSensor_open(FRONT_BUMPER); //front bumper
	if(probot->FrontSensor == NULL)
	{
		PProseError("Error with the instance front bumper.");
	}
	probot->lightSensor = LightSensor_open(LIGHT_SENSOR); //light sensor
	if(probot->lightSensor == NULL)
	{
		PProseError("Error with the instance light sensor.");
	}

	//Open the ports of the motors
	probot->leftMotor = Motor_open(LEFT_MOTOR); //left motor
	if(probot->leftMotor == NULL)
	{
		PProseError("Error with the instance left motor.");
	}
	probot->rightMotor = Motor_open(RIGHT_MOTOR); //right motor
	if(probot->rightMotor == NULL)
	{
		PProseError("Error with the instance right motor.");
	}
}

void Robot_stop(void)
{
	ProSE_Intox_close();

	//Closing the motors
	if(Motor_close(probot->leftMotor) == -1)
	{
		PProseError("Error while closing left motor.");
	}
	if(Motor_close(probot->rightMotor) == -1)
	{
		PProseError("Error while closing right motor.");
	}

	//Closing the sensors
	if(ContactSensor_close(probot->FrontSensor) == -1)
	{
		PProseError("Error while closing front sensor.");
	}
	if(ContactSensor_close(probot->FloorSensor) == -1)
	{
		PProseError("Error while closing contact sensor.");
	}
	if(LightSensor_close(probot->lightSensor) == -1)
	{
		PProseError("Error while closing light sensor.");
	}
}

void Robot_new(void)
{
	probot = (Robot *) malloc(sizeof(Robot));
}

void Robot_free(void)
{
	free(probot);
}

void Robot_setWheelsVelocity(int mr,int ml)
{
	if(Motor_setCmd(probot->leftMotor,ml) == -1)
	{
		PProseError("The command has not been given to the left motor.");
	}
	if(Motor_setCmd(probot->rightMotor,mr) == -1)
	{
		PProseError("The command has not been given to the right motor.");
	}
}

int Robot_getRobotSpeed(void)
{
	return ((abs(Motor_getCmd(probot->leftMotor)) + abs(Motor_getCmd(probot->rightMotor))) / 2);
}

SensorState Robot_getSensorState(void)
{
	SensorState sensorStatus;
	sensorStatus.collision = (ContactSensor_getStatus(probot->FloorSensor) || ContactSensor_getStatus(probot->FrontSensor))? BUMPED : NO_BUMP;
	sensorStatus.luminosity = LightSensor_getStatus(probot->lightSensor);

	return sensorStatus;
}
