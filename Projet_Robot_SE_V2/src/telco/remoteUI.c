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
struct RemoteUI_t
{
	Client* client;
};
/* ----------------------  PRIVATE STRUCTURES  ------------------------------ */
/* ----------------------  PRIVATE ENUMERATIONS  ---------------------------- */
/* ----------------------  PRIVATE VARIABLES  ------------------------------- */
bool_e quit_case = TRUE; //Used to get out or stay into the while loop.
/* ----------------------  PRIVATE FUNCTIONS PROTOTYPES  -------------------- */
/**
 * \fn static void RemoteUI_captureChoice(RemoteUI* pRemoteUI)
 * \brief Gets the key pressed from the user, dispatch to the right functions.
 */
static void RemoteUI_captureChoice(RemoteUI* pRemoteUI);
/**
 * \fn static void RemoteUI_askMvt(Direction dir)
 * \brief Ask a movement to the pilot to set wheels speed from the direction sent.
 *
 * \param Direction dir: Direction to give to the robot.
 */
static void RemoteUI_askMVt(RemoteUI* pRemoteUI,Direction dir);
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
static void RemoteUI_quit(RemoteUI* pRemoteUI);
/**
 * \fn static void RemoteUI_run()
 * \brief Core function of the UI.
 */
static void RemoteUI_run();
/**
 * \fn static void RemoteUI_display(RemoteUI* pRemoteUI)
 * \brief Displays the command to be entered to the user.
 */
static void RemoteUI_display(RemoteUI* pRemoteUI);
/**
 * \fn static void RemoteUI_setIP(in ip)
 * \brief used to set the ip for the communication.
 *
 * \param char ip : the ip for the commando.
 */
static void RemoteUI_setIP(RemoteUI* pRemoteUI, const char * ip);
/* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */
RemoteUI* RemoteUI_new()
{
	RemoteUI* pRemoteUI = (RemoteUI*) malloc(sizeof(RemoteUI));
	pRemoteUI->client = Client_new();
	if(pRemoteUI == NULL)
	{
		printf("ERROR : pAdminUI i NULL \n");
		while(1);
	}
	return pRemoteUI;
}
void RemoteUI_start(RemoteUI* pRemoteUI)
{
	RemoteUI_setIP(pRemoteUI, (char * )"127.0.0.1");
	Client_start(pRemoteUI->client);
	RemoteUI_run(pRemoteUI);
}
static void RemoteUI_run(RemoteUI* pRemoteUI)
{
	while (quit_case)
	{
		RemoteUI_display(pRemoteUI);
	}
}
void RemoteUI_stop(RemoteUI* pRemoteUI)
{
	Client_stop(pRemoteUI->client);
}
void RemoteUI_free(RemoteUI* pRemoteUI)
{
	Client_free(pRemoteUI->client);
	free(pRemoteUI);
}
/* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */
static void RemoteUI_captureChoice(RemoteUI* pRemoteUI)
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
			RemoteUI_askMVt(pRemoteUI,LEFT);
			break;
		case LOG_RIGHT:
			RemoteUI_askMVt(pRemoteUI,RIGHT);
			break;
		case LOG_BACKWARD:
			RemoteUI_askMVt(pRemoteUI,BACKWARD);
			break;
		case LOG_FORWARD:
			RemoteUI_askMVt(pRemoteUI,FORWARD);
			break;
		case LOG_CLEAR:
			RemoteUI_askClearLog();
			break;
		case LOG_STOP:
			RemoteUI_askMVt(pRemoteUI,STOP);
			break;
		case LOG_ROBOT_STATE:
			RemoteUI_ask4Log(pRemoteUI);
			break;
		case LOG_QUIT:
			RemoteUI_quit(pRemoteUI);
			break;
		default:
			break;
	}
}

static void RemoteUI_askMVt(RemoteUI* pRemoteUI,Direction dir)
{
	VelocityVector vel = RemoteUI_translate(dir);
	pRemoteUI->client->donnees.direction = vel.dir;
	pRemoteUI->client->donnees.power = vel.power;
	Client_sendMsg(pRemoteUI->client);
}

static VelocityVector RemoteUI_translate(Direction dir)
{
	VelocityVector vel;
	vel.power = 100;
	vel.dir = dir;
	return vel;
}

static void RemoteUI_ask4Log(RemoteUI* pRemoteUI)
{
	pRemoteUI->client->donnees.askLog = 1;
	Client_sendMsg(pRemoteUI->client);
	Client_readMsg(pRemoteUI->client);
	printf("\n Collision; %d", pRemoteUI->client->donnees.bump);
	printf("\n Luminosity: %f", pRemoteUI->client->donnees.luminosity);
	printf("\n Speed: %d:\n", pRemoteUI->client->donnees.power);
}


static void RemoteUI_askClearLog()
{
	RemoteUI_eraseLog();
}

static void RemoteUI_eraseLog()
{
	printf("\033[2J");
}

static void RemoteUI_quit(RemoteUI* pRemoteUI)
{
	quit_case = FALSE;
	pRemoteUI->client->donnees.stop = 1;
	Client_sendMsg(pRemoteUI->client);
}

static void RemoteUI_display(RemoteUI* pRemoteUI)
{
	printf("Robot V2\n");
	printf("Vous pouvez faire les actions suivantes :\n");
	printf("q:aller à gauche\n");
	printf("d:aller à droite\n");
	printf("z:avancer\n");
	printf("s:reculer\n");
	printf(" :stopper\n");
	printf("e:effacer les logs\n");
	printf("r:afficher l'état du robot\n");
	printf("a:quitter\n");
	RemoteUI_captureChoice(pRemoteUI);
}

static void RemoteUI_setIP(RemoteUI* pRemoteUI, const char * ip)
{
	pRemoteUI->client->ip = ip;
}
