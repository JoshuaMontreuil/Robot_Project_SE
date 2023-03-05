/**
 * @file  client.c
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
#include "client.h"
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
/* ----------------------  PRIVATE TYPE DEFINITIONS  ------------------------ */
/* ----------------------  PRIVATE STRUCTURES  ------------------------------ */
/* ----------------------  PRIVATE ENUMERATIONS  ---------------------------- */
/* ----------------------  PRIVATE VARIABLES  ------------------------------- */
/* ----------------------  PRIVATE FUNCTIONS PROTOTYPES  -------------------- */
/* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */
void Client_start(Client* pClient)
{
	pClient->un_socket = socket(PF_INET, SOCK_STREAM, 0);
	pClient->adresse_du_serveur.sin_family = AF_INET;
	pClient->adresse_du_serveur.sin_port = htons(PORT_DU_SERVEUR);
	pClient->adresse_du_serveur.sin_addr = *((struct in_addr *)gethostbyname(pClient->ip)->h_addr_list[0]);
	connect(pClient->un_socket, (struct sockaddr *)&pClient->adresse_du_serveur,sizeof(pClient->adresse_du_serveur));
}

Client* Client_new(void)
{
	Client* pClient = (Client*) malloc(sizeof(Client));
	if(pClient == NULL)
	{
		printf("ERROR : pRobot is NULL /n");
		while(1);
	}
	return pClient;
}

void Client_stop(Client* pClient)
{
	close(pClient->un_socket);
}

void Client_free(Client* pClient)
{
	free(pClient);
}

void Client_sendMsg(Client* pClient)
{
	DesDonnees data;
	data = pClient->donnees;
	int quantite_envoyee;
	quantite_envoyee = write(pClient->un_socket, &data, sizeof(data));
	printf("LOG_MSG_SENT\n");
}

void Client_readMsg(Client* pClient)
{
	printf("dans read avant connexion\n");
	DesDonnees data;
	printf("dans read\n");
	read(pClient->un_socket, &data, sizeof(data));
	pClient->donnees = data;
	printf("LOG_MSG_RCV : \n");
	printf("- asklog : %d\n", pClient->donnees.askLog);
	printf("- power : %d\n", pClient->donnees.power);
	printf("- direction : %d\n", pClient->donnees.direction);
	printf("- bump : %d\n", pClient->donnees.bump);
	printf("- luminosity : %f\n", pClient->donnees.luminosity);
	printf("- stop : %d\n", pClient->donnees.stop);
	/*pClient->donnees.direction = ntohl(data.direction);
	pClient->donnees.power = ntohl(data.power);
	pClient->donnees.luminosity = ntohl(data.luminosity);
	pClient->donnees.bump = ntohl(data.bump);
	pClient->donnees.askLog = ntohl(data.askLog);
	pClient->donnees.stop = ntohl(data.stop);*/
}
/* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */
