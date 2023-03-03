/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/**
 * @file  pilot.c
 *
 * @brief  Create the pilot object, sets the wheel velocity from the user command and Stop/Run the robot.
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
#include "pilot.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
/* ----------------------  PRIVATE CONFIGURATIONS  -------------------------- */
/* ----------------------  PRIVATE TYPE DEFINITIONS  ------------------------ */
/**
 * \enum event_e
 * \brief event for the state machine.
 */
typedef enum
{
	SETVELOCITY_E,
	SETVELOCITY_CHANGE_E,
	SETVELOCITY_STOP_E,
	CHECK_E,
	CHECKED_E,
	STOP_E,
	NB_E
}event_e;

typedef struct
{
	State_e stateDestination;
	action_e action;
}Transition_s;

typedef void (*ActionPtr)();
/* ----------------------  PRIVATE STRUCTURES  ------------------------------ */
/* ----------------------  PRIVATE ENUMERATIONS  ---------------------------- */
/* ----------------------  PRIVATE FUNCTIONS PROTOTYPES  -------------------- */
/**
 * \fn static void Pilot_run(Pilot* pPilot, event ev, VelocityVector vel)
 * \brief State machine of the Pilot, run from different events.
 *
 * \param event ev: Gives the event to call.
 * \param VelocityVector vel: Data with the velocity vector -> speed and direction.
 */
static void Pilot_run(Pilot* pPilot, event_e ev);
/**
 * \fn static void Pilot_sendMvt(VelocityVector vel)
 * \brief Sends the movement order to Robot_setWheelsVelocity.
 *
 * \param VelocityVector vel: Data with the velocity vector -> speed and direction.
 */
static void Pilot_sendMvt(Pilot* pPilot);
/**
 * \fn static bool_e Pilot_hasBumped(Pilot* pPilot)
 * \brief check if the robot's collision sensors has bumped of not.
 *
 * \return bool_e : TRUE if bumped FALSE if not_bumped
 */
static bool_e Pilot_hasBumped(Pilot* pPilot);
/**
 * \fn static void Pilot_Bump_Check(Pilot* pPilot)
 * \brief Action to check a bump, calls run if it has.
 */
static void Pilot_Bump_Check(Pilot* pPilot);
/**
 * \fn static void Pilot_sendMVT_Stop(Pilot* pPilot)
 * \brief Action to put the direction to stop on the velocity vector.Then calls "Pilot_sendMvt"
 */
static void Pilot_sendMVT_Stop(Pilot* pPilot);
/**
 * \fn static void Pilot_actionNop(Pilot* pPilot)
 * \brief Action nop
 */
static void Pilot_actionNop(Pilot* pPilot);
/**
 * \fn static void Pilot_action_Vel_Change(Pilot* pPilot)
 * \brief Action to verify any changment into the direction of the vector.
 */
static void Pilot_action_Vel_Change(Pilot* pPilot);
/* ----------------------  PRIVATE VARIABLES  ------------------------------- */
static Transition_s stateMachine[NB_S][NB_E]=
{
		[IDLE][SETVELOCITY_E] = {IDLE,VELOCITY_CHANGE_A},
		[IDLE][SETVELOCITY_CHANGE_E] = {RUNNING,SEND_MVT_A},
		[IDLE][SETVELOCITY_STOP_E] = {IDLE,SEND_MVT_A},
		[RUNNING][SETVELOCITY_E] = {RUNNING,VELOCITY_CHANGE_A},
		[RUNNING][SETVELOCITY_CHANGE_E] = {RUNNING,SEND_MVT_A},
		[RUNNING][SETVELOCITY_STOP_E] = {IDLE,SEND_MVT_A},
		[RUNNING][CHECK_E] = {RUNNING,CHECK_A},
		[RUNNING][STOP_E] = {DEATH_S,SEND_MVT_STOP_A},
		[IDLE][STOP_E] = {DEATH_S,SEND_MVT_STOP_A},
		[RUNNING][CHECKED_E] = {IDLE,SEND_MVT_STOP_A}
};

static const ActionPtr actionsTab[NB_ACTION] = {&Pilot_actionNop,&Pilot_action_Vel_Change, &Pilot_sendMVT_Stop,&Pilot_sendMvt,&Pilot_Bump_Check};
/* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */
Pilot* Pilot_new(void)
{
	Pilot* pPilot = (Pilot*) malloc(sizeof(Pilot));
	pPilot->state = IDLE;
	pPilot->robot = Robot_new();
	if(pPilot == NULL)
	{
		printf("ERROR : pPilot is NULL /n");
		while(1);
	}
	return pPilot;
}

void Pilot_start(Pilot* pPilot)
{
	Robot_start(pPilot->robot);
}

void Pilot_setVelocity(Pilot* pPilot)
{
	Pilot_run(pPilot, SETVELOCITY_E);
}

PilotState Pilot_getState(Pilot* pPilot)
{
	return pPilot->PState;
}

void Pilot_check(Pilot* pPilot)
{
	VelocityVector vel;
	pPilot->vector = vel;
	pPilot->PState.collision = Robot_getSensorState(pPilot->robot).collision;
	pPilot->PState.luminosity = Robot_getSensorState(pPilot->robot).luminosity;
	pPilot->PState.speed = Robot_getRobotSpeed(pPilot->robot);
	if(pPilot->state == IDLE && pPilot->vector.dir != STOP)
	{
		pPilot->state = RUNNING;
	}
	Pilot_run(pPilot,CHECK_E);
}


void Pilot_stop(Pilot* pPilot)
{
	VelocityVector vel;
	pPilot->vector = vel;
	Pilot_run(pPilot,STOP_E);
	Robot_stop(pPilot->robot);
}

void Pilot_free(Pilot* pPilot)
{
	Robot_free(pPilot->robot);
	free(pPilot);
}
/* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */
static void Pilot_run(Pilot* pPilot,event_e ev)
{
	State_e tempState;
	assert(pPilot->state != DEATH_S);
	pPilot->action = stateMachine[pPilot->state][ev].action;
	tempState = stateMachine[pPilot->state][ev].stateDestination;
	if(tempState != FORGET_S)
	{
		actionsTab[pPilot->action](pPilot);
		pPilot->state = tempState;
	}
}

static void Pilot_sendMvt(Pilot* pPilot)
{
	switch(pPilot->vector.dir)
	{
		case LEFT:
			Robot_setWheelsVelocity(pPilot->robot, -(pPilot->vector.power), (pPilot->vector.power));
			break;
		case RIGHT:
			Robot_setWheelsVelocity(pPilot->robot, (pPilot->vector.power),-(pPilot->vector.power));
			break;
		case FORWARD:
			Robot_setWheelsVelocity(pPilot->robot, (pPilot->vector.power), (pPilot->vector.power));
			break;
		case BACKWARD:
			Robot_setWheelsVelocity(pPilot->robot, -(pPilot->vector.power), -(pPilot->vector.power));
			break;
		case STOP:
			Robot_setWheelsVelocity(pPilot->robot, 0, 0);
			break;
		default:
			break;
	}
}
static void Pilot_Bump_Check(Pilot* pPilot)
{
	if(Pilot_hasBumped(pPilot))
	{
		Pilot_run(pPilot, CHECKED_E);
	}
}
static void Pilot_action_Vel_Change(Pilot* pPilot)
{
	event_e tempEvent = SETVELOCITY_CHANGE_E;
	if(pPilot->vector.dir == STOP)
	{
		tempEvent = SETVELOCITY_STOP_E;;
	}
	Pilot_run(pPilot, tempEvent);
}

static void Pilot_sendMVT_Stop(Pilot* pPilot)
{
	pPilot->vector.dir = STOP;
	Pilot_sendMvt(pPilot);
}
static bool_e Pilot_hasBumped(Pilot* pPilot)
{
	return (pPilot->PState.collision == BUMPED)? TRUE : FALSE;
}
static void Pilot_actionNop(Pilot* pPilot){}
