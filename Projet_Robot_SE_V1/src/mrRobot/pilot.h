/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */  
/**
 * @file  pilot.h
 *
 * @brief  header file for pilot.c
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

#ifndef PILOT_H
#define PILOT_H
/* ----------------------  INCLUDES ------------------------------------------*/
#include "prose.h"
#include "robot.h"
/* ----------------------  PUBLIC TYPE DEFINITIONS ---------------------------*/
typedef struct Pilot_t Pilot;
/**
 * \enum Direction
 * \brief Constant for directions.
 */
typedef enum {LEFT=0, RIGHT, FORWARD, BACKWARD, STOP} Direction;
/**
 * \struct VelocityVector
 * \brief Constants for velocity vector.
 */
typedef struct
{
    Direction dir;
    int power;
} VelocityVector;

/**
 * \struct PilotState
 * \brief Constants for pilot state.
 */
typedef struct
{
    int speed;
    int collision;
    float luminosity;
} PilotState;

typedef enum
{
	NOP_A,
	VELOCITY_CHANGE_A,
	SEND_MVT_STOP_A,
	SEND_MVT_A,
	CHECK_A,
	NB_ACTION
}action_e;

/**
 * \enum State_e
 * \brief State for the state machine.
 */
typedef enum
{
	FORGET_S,
	IDLE,
	RUNNING,
	DEATH_S,
	NB_S
}State_e;
/* ----------------------  PUBLIC ENUMERATIONS -------------------------------*/
/* ----------------------  PUBLIC STRUCTURES ---------------------------------*/
struct Pilot_t
{
	State_e state;
	action_e action;
	VelocityVector vector;
	PilotState PState;
	Robot* robot;
};
/* ----------------------  PUBLIC VARIBLES -----------------------------------*/
/* ----------------------  PUBLIC FUNCTIONS PROTOTYPES  ----------------------*/
/**
 * \fn extern Pilot$* Pilot_new()
 * \brief Initialize in memory the object Pilot.
 *
 * \return Pilot*
 */
extern Pilot* Pilot_new();
/**
 * \fn extern void Pilot_start(Pilot* pPilot)
 * \brief Start Pilot.
 */
extern void Pilot_start(Pilot* pPilot);
/**
 * \fn extern void Pilot_stop(Pilot* pPilot)
 * \brief Stop Pilot.
 */
extern void Pilot_stop(Pilot* pPilot);
/**
 * \fn extern void Pilot_free(Pilot* pPilot)
 * \brief Destruct the object Pilot from memory.
 */
extern void Pilot_free(Pilot* pPilot);
/**
 * \fn extern void Pilot_setVelocity(VelocityVector vel,Pilot* pPilot)
 * \brief Setting the velocity vector of the object Pilot.
 * 
 * \param VelocityVector vel.
 */
extern void Pilot_setVelocity(Pilot* pPilot);
/**
 * \fn extern PilotState Pilot_getState(Pilot* pPilot)
 * \brief getter of the State of the Pilot.
 * 
 * \return PilotState.
 */
extern PilotState Pilot_getState(Pilot* pPilot);
/**
 * \fn extern void Pilot_check(Pilot* pPilot)
 * \brief gets the sensors state of the robot and put it into the Pilot object.
 */
extern void Pilot_check(Pilot* pPilot);

#endif /* PILOT_H */

