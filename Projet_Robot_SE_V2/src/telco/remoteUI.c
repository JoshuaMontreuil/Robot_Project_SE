/**
 * @file  remoteUI.c
 *
 * @brief  
 *
 * @author joshua
 * @date Feb 12, 2023
 * @version 
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
/* ----------------------  INCLUDES  ---------------------------------------- */
#include "remoteUI.h"
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
/* ----------------------  PRIVATE CONFIGURATIONS  -------------------------- */
/* ----------------------  PRIVATE TYPE DEFINITIONS  ------------------------ */
/* ----------------------  PRIVATE STRUCTURES  ------------------------------ */
/* ----------------------  PRIVATE ENUMERATIONS  ---------------------------- */
/* ----------------------  PRIVATE VARIABLES  ------------------------------- */
bool_e quit_case = TRUE; //Used to get out or stay into the while loop.
/* ----------------------  PRIVATE FUNCTIONS PROTOTYPES  -------------------- */
/**
 * \fn static void RemoteUI_captureChoice()
 * \brief Gets the key pressed from the user, dispatch to the right functions.
 */
static void RemoteUI_captureChoice();
/**
 * \fn static void RemoteUI_askMvt(Direction dir)
 * \brief Ask a movement to the pilot to set wheels speed from the direction sent.
 *
 * \param Direction dir: Direction to give to the robot.
 */
static void RemoteUI_askMVt(Direction dir);
/**
 * \fn static VelocityVector RemoteUI_translate(Direction dir)
 * \brief Translate a Direction to a VelocityVector object.
 *
 * \param Direction dir: Direction to give to the robot.
 *
 * \return VelocityVector: translate to a VelocityVector with a speed and a direction.
 */
static VelocityVector RemoteUI_translate(Direction dir);
/**
 * \fn static void RemoteUI_ask4Log()
 * \brief Gets the states and values of the sensors from the Pilot to be printed.
 */
static void RemoteUI_ask4Log();
/**
 * \fn static void RemoteUI_askClearLog()
 * \brief Ask the user if he really wants to clear the logs.
 */
static void RemoteUI_askClearLog();
/**
 * \fn static void RemoteUI_eraseLog()
 * \brief Erase the log of the cmd.
 */
static void RemoteUI_eraseLog();
/**
 * \fn static void RemoteUI_quit()
 * \brief Quitting the application.
 */
static void RemoteUI_quit();
/**
 * \fn static void RemoteUI_run()
 * \brief Core function of the UI.
 */
static void RemoteUI_run();
/**
 * \fn static void RemoteUI_display()
 * \brief Displays the command to be entered to the user.
 */
static void RemoteUI_display();
/* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */
/* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */
