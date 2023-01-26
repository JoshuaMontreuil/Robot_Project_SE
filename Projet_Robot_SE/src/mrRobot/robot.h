/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */  
/**
 * @file  robot.h
 *
 * @brief  header of the file robot.c
 *
 * @author Joshua Montreuil
 * @date 26-01-2023
 * @version version
 * @section License
 *
 * The MIT License
 *
 * Copyright (c) 2021, Joshua Montreuil
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

#ifndef ROBOT_H
#define ROBOT_H

#include "prose.h"

/**
 * \enum Collision
 * \brief Constant for collisions.
 */
typedef enum {NO_BUMP=0, BUMPED} Collision;

/**
 * @struct SensorStae
 * @brief The captor's states of the robot (bumper and luminosity)
 */
typedef struct
{
    Collision collision;
    float luminosity;
} SensorState;

/**
 * @struct Robot
 * @brief The captor's states of the robot (bumper and luminosity)
 */
typedef struct
{
	Motor * rightMotor;
	Motor * leftMotor;
	ContactSensor * FloorSensor;
	ContactSensor * FrontSensor;
	LightSensor * lightSensor;
}Robot;

/**
 * Start the Robot (initialize communication and open port)
 */
extern void Robot_start();


/**
 * Stop Robot (stop communication and close port)
 */
extern void Robot_stop();


/**
 * @brief initialize in memory the object Robot
 * 
 */
extern void Robot_new();


/**
 *  @brief destruct the object Robot from memory 
 */
extern void Robot_free();

/**
 * Robot_getRobotSpeed
 * 
 * @brief return the speed of the robot (positive average of the right's and left's current wheel power) 
 * @return speed of the robot (beetween 0 and 100)
 */
extern int Robot_getRobotSpeed();

/**
 * Robot_getSensorState
 * 
 * @brief return the captor's states of the bumper and the luminosity
 * @return SensorState
 */
extern SensorState Robot_getSensorState();

/**
 * Robot_setWheelsVelocity
 * 
 * @brief set the power on the wheels of the robot
 * @param int mr : right's wheel power, value between -10O and 100
 * @param int ml : left's wheel power, value between -100 and 100
 */
extern void Robot_setWheelsVelocity(int mr,int ml);


#endif /* ROBOT_H */

