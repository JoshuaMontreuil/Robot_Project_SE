/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */  
/**
 * @file  pilot.h
 *
 * @brief  header file for pilot.c
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

#ifndef SRC_COMMANDO_PILOT_H
#define SRC_COMMANDO_PILOT_H

//----INCLUDES -----------------------------------------------------------------
#include "prose.h"

//---- ENUM --------------------------------------------------------------------
/**
 * \enum Direction
 * \brief Constant for directions.
 */
typedef enum {LEFT=0, RIGHT, FORWARD, BACKWARD, STOP} Direction;

/**
 * \enum State_e
 * \brief State for the state machine.
 */
typedef enum
{
	IDLE,
	RUNNING
}State_e;

/**
 * \enum event
 * \brief event for the state machine.
 */
typedef enum
{
	SETVELOCITY,
	CHECK,
	STOP_EVENT
}event;

//---- STRUCTURES --------------------------------------------------------------
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

//---- PRIVATE FUNCTION DECLARATIONS -------------------------------------------
/**
 * \fn extern void Pilot_start()
 * \brief Start Pilot.
 */
extern void Pilot_start();


/**
 * \fn extern void Pilot_stop()
 * \brief Stop Pilot.
 */
extern void Pilot_stop();


/**
 * \fn extern void Pilot_new()
 * \brief Initialize in memory the object Pilot.
 */
extern void Pilot_new();


/**
 * \fn extern void Pilot_free()
 * \brief Destruct the object Pilot from memory.
 */
extern void Pilot_free();

/**
 * \fn extern void Pilot_setVelocity(VelocityVector vel)
 * \brief Setting the velocity vector of the Pilot.
 * 
 * \param VelocityVector vel.
 */
extern void Pilot_setVelocity(VelocityVector vel);

/**
 * \fn extern PilotState Pilot_getState()
 * \brief getter of the State of the Pilot.
 * 
 * \return PilotState.
 */
extern PilotState Pilot_getState();

/**
 * \fn extern void Pilot_check()
 * \brief gets the sensors state of the robot and put it into the Pilot object.
 */
extern void Pilot_check();

#endif /* SRC_COMMANDO_PILOT_H */

