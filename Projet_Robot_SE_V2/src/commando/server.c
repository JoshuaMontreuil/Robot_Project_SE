/**
 * @file  server.c
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
#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
/* ----------------------  PRIVATE CONFIGURATIONS  -------------------------- */
#define MAX_PENDING_CONNECTIONS 5
/* ----------------------  PRIVATE TYPE DEFINITIONS  ------------------------ */
/* ----------------------  PRIVATE STRUCTURES  ------------------------------ */
/* ----------------------  PRIVATE ENUMERATIONS  ---------------------------- */
/* ----------------------  PRIVATE VARIABLES  ------------------------------- */
bool_e shut_down = TRUE; //Used to get out or stay into the while loop.
/* ----------------------  PRIVATE FUNCTIONS PROTOTYPES  -------------------- */
static void Server_sendMsg(Server* pServer);

static void Server_readMsg(Server* pServer);

static void Server_run(Server* pServer);

static void Server_logs(Server* pServer);
/* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */
Server* Server_new()
{
	Server* pServer = (Server*) malloc(sizeof(Server));
	pServer->pilot = Pilot_new();
	if(pServer == NULL)
	{
		printf("ERROR : pRobot is NULL /n");
		while(1);
	}
	return pServer;
}

void Server_start(Server* pServer)
{
	Pilot_start(pServer->pilot);
	pServer->socket_ecoute = socket (PF_INET, SOCK_STREAM, 0);
	pServer->mon_adresse.sin_family = AF_INET;
	pServer->mon_adresse.sin_port = htons(PORT_DU_SERVEUR);

	pServer->mon_adresse.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(pServer->socket_ecoute, (struct sockaddr *)&pServer->mon_adresse, sizeof(pServer->mon_adresse));

	listen(pServer->socket_ecoute, MAX_PENDING_CONNECTIONS);

	pServer->socket_donnees = accept(pServer->socket_ecoute, NULL, 0);

	while(shut_down)
	{
		Server_run(pServer);
	}
}

void Server_stop(Server* pServer)
{
	close(pServer->socket_donnees);
}

void Server_free(Server* pServer)
{
	Pilot_free(pServer->pilot);
	free(pServer);
}
/* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */
static void Server_run(Server* pServer)
{
	printf("here\n");
	Server_readMsg(pServer);
	Server_logs(pServer);
	if(pServer->donnees.askLog == 1)
	{
		Pilot_check(pServer->pilot);
		PilotState p_state = Pilot_getState(pServer->pilot);
		pServer->donnees.bump = p_state.collision;
		pServer->donnees.luminosity = p_state.luminosity;
		pServer->donnees.power = p_state.speed;
		pServer->donnees.askLog = 0;
		printf("here2\n");
		Server_sendMsg(pServer);
		printf("LOG_MSG_SENT\n");
	}
	else if(pServer->donnees.stop == 1)
	{
		Pilot_stop(pServer->pilot);
		pServer->donnees.stop = 0;
		Server_stop(pServer);
		//shut_down = TRUE;
	}
	else
	{
		pServer->pilot->vector.dir = (Direction) pServer->donnees.direction;
		pServer->pilot->vector.power = pServer->donnees.power;
		Pilot_setVelocity(pServer->pilot);
	}
}
static void Server_readMsg(Server* pServer)
{
	read(pServer->socket_donnees, &pServer->donnees, sizeof(pServer->donnees));
}

static void Server_sendMsg(Server* pServer)
{
	int quantite_envoyee;
	quantite_envoyee = write(pServer->socket_donnees, &pServer->donnees, sizeof(pServer->donnees));
}
static void Server_logs(Server* pServer)
{
	printf("LOG_MSG_RCV : \n");
	printf("- asklog : %d\n", pServer->donnees.askLog);
	printf("- power : %d\n", pServer->donnees.power);
	printf("- direction : %d\n", pServer->donnees.direction);
	printf("- bump : %d\n", pServer->donnees.bump);
	printf("- luminosity : %f\n", pServer->donnees.luminosity);
	printf("- stop : %d\n", pServer->donnees.stop);
}
