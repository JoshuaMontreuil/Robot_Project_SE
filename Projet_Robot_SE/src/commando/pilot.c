/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/**
 * @file  pilot.c
 *
 * @brief  Create the pilot object, sets the wheel velocity from the user command and Stop/Run the robot.
 *
 * @author Joshua Montreuil
 * @date 26-01-2023
 * @version 1.1
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
#include "pilot.h"
#include "robot.h"
#include <stdlib.h>

//---- GLOBAL VARIABLES --------------------------------------------------------
PilotState *pState;
State_e currentState =IDLE;

//---- PRIVATE FUNCTION DECLARATIONS -------------------------------------------
/**
 * \fn static void Pilot_run(event ev, VelocityVector vel)
 * \brief State machine of the Pilot, run from different events.
 *
 * \param event ev: Gives the event to call.
 * \param VelocityVector vel: Data with the velocity vector -> speed and direction.
 */
static void Pilot_run(event ev, VelocityVector vel);

/**
 * \fn static void Pilot_sendMvt(VelocityVector vel)
 * \brief Sends the movement order to Robot_setWheelsVelocity.
 *
 * \param VelocityVector vel: Data with the velocity vector -> speed and direction.
 */
static void Pilot_sendMvt(VelocityVector vel);

/**
 * \fn static bool_e Pilot_hasBumped()
 * \brief check if the robot's collision sensors has bumped of not.
 *
 * \return bool_e : TRUE if bumped FALSE if not_bumped
 */
static bool_e Pilot_hasBumped();

//---- PUBLIC FUNCTIONS --------------------------------------------------------
void Pilot_new(void)
{
	pState = (PilotState*) malloc(sizeof(PilotState));
	Robot_new();
}

void Pilot_start(void)
{
	Robot_start();
}

void Pilot_setVelocity(VelocityVector vel)
{
	Pilot_run(SETVELOCITY, vel);
}

PilotState Pilot_getState(void)
{
	return *pState;
}

void Pilot_check(void)
{
	VelocityVector vel;
	pState->collision = Robot_getSensorState().collision;
	pState->luminosity = Robot_getSensorState().luminosity;
	pState->speed = Robot_getRobotSpeed();
	Pilot_run(CHECK, vel);
}

//---- PRIVATE FUNCTIONS -------------------------------------------------------
static void Pilot_run(event ev, VelocityVector vel)
{
	switch(currentState)
	{
		case IDLE:
			if(ev == SETVELOCITY)
			{
				if(vel.dir != STOP)
				{
					currentState = RUNNING;
					Pilot_sendMvt(vel);
				}
			}
			else if(ev == STOP_EVENT)
			{
				vel.dir = STOP;
				Pilot_sendMvt(vel);
			}
			break;
		case RUNNING:
			if(ev == SETVELOCITY)
			{
				if(vel.dir == STOP)
				{
					currentState = IDLE;
					Pilot_sendMvt(vel);
				}
				else
				{
					Pilot_sendMvt(vel);
				}
			}
			else if(ev == CHECK)
			{
				if(Pilot_hasBumped())
				{
					vel.dir = STOP;
					currentState = IDLE;
					Pilot_sendMvt(vel);
				}
			}
			else if(ev == STOP_EVENT)
			{
				vel.dir = STOP;
				Pilot_sendMvt(vel);
			}
			break;
		default:
			break;
	}
}

static void Pilot_sendMvt(VelocityVector vel)
{
	switch(vel.dir)
	{
		case LEFT:
			Robot_setWheelsVelocity(-vel.power, vel.power);
			break;
		case RIGHT:
			Robot_setWheelsVelocity(vel.power,-vel.power);
			break;
		case FORWARD:
			Robot_setWheelsVelocity(vel.power, vel.power);
			break;
		case BACKWARD:
			Robot_setWheelsVelocity(-vel.power, -vel.power);
			break;
		case STOP:
			Robot_setWheelsVelocity(0, 0);
			break;
		default:
			break;
	}
}

static bool_e Pilot_hasBumped()
{
	return (pState->collision == BUMPED)? TRUE : FALSE;
}

void Pilot_stop(void)
{
	VelocityVector vel;
	Pilot_run(STOP_EVENT, vel);
	Robot_stop();
}

void Pilot_free()
{
	free(pState);
	Robot_free();
}
