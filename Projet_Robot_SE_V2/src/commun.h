/**
 * @file  commun.h
 *
 * @brief
 *
 * @author joshua
 * @date Feb 28, 2023
 * @version 1
 * @section License
 *
 * The MIT License
 *
 * Copyright (c) 2023, joshua
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
#ifndef SRC_COMMUN_H_
#define SRC_COMMUN_H_
#include <arpa/inet.h>
#define PORT_DU_SERVEUR (12346)


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

/**
 * \brief Boolean.
 *
 * bool_e représente un booléen, il existe sous 2 états.
 */

typedef struct{
    int direction;
    int power;
    float luminosity;
    int bump;
    int askLog; //0 no, 1 yes.
    int stop; //0 no stop, 1 stop.
}DesDonnees;


#endif /* SRC_COMMUN_H_ */
