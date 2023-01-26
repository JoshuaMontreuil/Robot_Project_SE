/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */

#include "pilot.h"
#include "robot.h"
#include <stdlib.h>

PilotState *pState;
State_e currentState =IDLE;

/**
 * run
 *
 * @brief description
 */
static void Pilot_run(event ev, VelocityVector vel);

/**
 * sendMvt
 *
 * @brief description
 */
static void Pilot_sendMvt(VelocityVector vel);

/**
 * hasBumped
 *
 * @brief description
 */
static bool_e Pilot_hasBumped();


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
