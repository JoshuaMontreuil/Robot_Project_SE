/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/**
 * @file  adminUI.c
 *
 * @brief  This component describe the admin user interface. It captures the user's controls and send them to the pilot module.
 *
 * @author Joshua Montreuil
 * @date 27-01-2023
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
#include "adminui.h"
#include "pilot.h"
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
/* ----------------------  PRIVATE CONFIGURATIONS  -------------------------- */
/* ----------------------  PRIVATE TYPE DEFINITIONS  ------------------------ */
/**
 * \enum log_key_e
 * \brief Defines the user entry key from their keyboards.
 */
typedef enum
{
	LOG_LEFT = 'q',       /**< LOG_LEFT */
	LOG_RIGHT = 'd',      /**< LOG_RIGHT */
	LOG_FORWARD = 'z',    /**< LOG_FORWARD */
	LOG_BACKWARD = 's',   /**< LOG_BACKWARD */
	LOG_STOP = ' ',       /**< LOG_STOP */
	LOG_CLEAR = 'e',      /**< LOG_CLEAR */
	LOG_ROBOT_STATE = 'r',/**< LOG_ROBOT_STATE */
	LOG_QUIT = 'a'        /**< LOG_QUIT */
}log_key_e;
/* ----------------------  PRIVATE STRUCTURES  ------------------------------ */
struct AdminUI_t
{
	Pilot* pilot;
};
/* ----------------------  PRIVATE ENUMERATIONS  ---------------------------- */
/* ----------------------  PRIVATE VARIABLES  ------------------------------- */
static bool_e quit_case = TRUE; //Used to get out or stay into the while loop.
/* ----------------------  PRIVATE FUNCTIONS PROTOTYPES  -------------------- */
/**
 * \fn static void AdminUI_captureChoice()
 * \brief Gets the key pressed from the user, dispatch to the right functions.
 */
static void AdminUI_captureChoice(AdminUI* pAdminUI);
/**
 * \fn static void AdminUI_askMvt(Direction dir)
 * \brief Ask a movement to the pilot to set wheels speed from the direction sent.
 *
 * \param Direction dir: Direction to give to the robot.
 */
static void AdminUI_askMVt(AdminUI* pAdminUI, Direction dir);
/**
 * \fn static VelocityVector AdminUI_translate(Direction dir)
 * \brief Translate a Direction to a VelocityVector object.
 *
 * \param Direction dir: Direction to give to the robot.
 *
 * \return VelocityVector: translate to a VelocityVector with a speed and a direction.
 */
static VelocityVector AdminUI_translate(Direction dir);
/**
 * \fn static void AdminUI_ask4Log()
 * \brief Gets the states and values of the sensors from the Pilot to be printed.
 */
static void AdminUI_ask4Log(AdminUI* pAdminUI);
/**
 * \fn static void AdminUI_askClearLog()
 * \brief Ask the user if he really wants to clear the logs.
 */
static void AdminUI_askClearLog();
/**
 * \fn static void AdminUI_eraseLog()
 * \brief Erase the log of the cmd.
 */
static void AdminUI_eraseLog();
/**
 * \fn static void AdminUI_quit()
 * \brief Quitting the application.
 */
static void AdminUI_quit(AdminUI* pAdminUI);
/**
 * \fn static void AdminUI_run()
 * \brief Core function of the UI.
 */
static void AdminUI_run(AdminUI* pAdminUI);
/**
 * \fn static void AdminUI_display()
 * \brief Displays the command to be entered to the user.
 */
static void AdminUI_display(AdminUI* pAdminUI);
/* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */
AdminUI* AdminUI_new()
{
	AdminUI* pAdminUI = (AdminUI*) malloc(sizeof(AdminUI));
	pAdminUI->pilot = Pilot_new();
	if(pAdminUI == NULL)
	{
		printf("ERROR : pAdminUI i NULL \n");
		while(1);
	}
	return pAdminUI;
}
void AdminUI_start(AdminUI* pAdminUI)
{
	Pilot_start(pAdminUI->pilot);
	AdminUI_run(pAdminUI);
}
static void AdminUI_run(AdminUI* pAdminUI)
{
	while (quit_case)
	{
		AdminUI_display(pAdminUI);
	}
}
void AdminUI_stop(AdminUI* pAdminUI)
{
	//blank
}
void AdminUI_free(AdminUI* pAdminUI)
{
	Pilot_free(pAdminUI->pilot);
	free(pAdminUI);
}

/* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */
static void AdminUI_captureChoice(AdminUI* pAdminUI)
{
	//Method to remove the enter key to be pressed
	struct termios oldt, newt;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	log_key_e command = getchar();
	tcsetattr(STDIN_FILENO,TCSANOW, &oldt);

	switch(command)
	{
		case LOG_LEFT:
			AdminUI_askMVt(pAdminUI,LEFT);
			break;
		case LOG_RIGHT:
			AdminUI_askMVt(pAdminUI,RIGHT);
			break;
		case LOG_BACKWARD:
			AdminUI_askMVt(pAdminUI,BACKWARD);
			break;
		case LOG_FORWARD:
			AdminUI_askMVt(pAdminUI,FORWARD);
			break;
		case LOG_CLEAR:
			AdminUI_askClearLog();
			break;
		case LOG_STOP:
			AdminUI_askMVt(pAdminUI,STOP);
			break;
		case LOG_ROBOT_STATE:
			AdminUI_ask4Log(pAdminUI);
			break;
		case LOG_QUIT:
			AdminUI_quit(pAdminUI);
			break;
		default:
			break;
	}
}

static void AdminUI_askMVt(AdminUI* pAdminUI,Direction dir)
{
	pAdminUI->pilot->vector = AdminUI_translate(dir);
	Pilot_setVelocity(pAdminUI->pilot);
}

static VelocityVector AdminUI_translate(Direction dir)
{
	VelocityVector vel;
	vel.power = 100;
	vel.dir = dir;
	return vel;
}

static void AdminUI_ask4Log(AdminUI* pAdminUI)
{
	if(pAdminUI->pilot->state == IDLE && pAdminUI->pilot->vector.dir != STOP)
	{
		pAdminUI->pilot->state = RUNNING;
	}
	Pilot_check(pAdminUI->pilot);
	PilotState p_state = Pilot_getState(pAdminUI->pilot);
	printf("\n Collision; %d", p_state.collision);
	printf("\n Luminosity: %f", p_state.luminosity);
	printf("\n Speed: %d:\n", p_state.speed);
}

static void AdminUI_askClearLog()
{
	AdminUI_eraseLog();
}

static void AdminUI_eraseLog()
{
	printf("\033[2J");
}

static void AdminUI_quit(AdminUI* pAdminUI)
{
	quit_case = FALSE;
	Pilot_stop(pAdminUI->pilot);
}

static void AdminUI_display(AdminUI* pAdminUI)
{
	printf("Robot V1\n");
	printf("Vous pouvez faire les actions suivantes :\n");
	printf("q:aller à gauche\n");
	printf("d:aller à droite\n");
	printf("z:avancer\n");
	printf("s:reculer\n");
	printf(" :stopper\n");
	printf("e:effacer les logs\n");
	printf("r:afficher l'état du robot\n");
	printf("a:quitter\n");
	AdminUI_captureChoice(pAdminUI);
}
