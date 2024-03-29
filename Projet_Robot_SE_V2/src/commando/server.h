/**
 * @file  server.h
 *
 * @brief
 *
 * @author joshua
 * @date Feb 12, 2023
 * @version 1.0
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
#ifndef SRC_COMMANDO_SERVER_H_
#define SRC_COMMANDO_SERVER_H_
/* ----------------------  INCLUDES ------------------------------------------*/
#include "../commun.h"
#include "pilot.h"
/* ----------------------  PUBLIC TYPE DEFINITIONS ---------------------------*/
/**
 * \struct Server
 * \brief Server object.
 */
typedef struct Server_t Server;
/* ----------------------  PUBLIC ENUMERATIONS -------------------------------*/
/* ----------------------  PUBLIC STRUCTURES ---------------------------------*/
struct Server_t
{
	Pilot* pilot;
	int socket_ecoute;
	int socket_donnees;
	struct sockaddr_in mon_adresse;
	DesDonnees donnees;
};
/* ----------------------  PUBLIC VARIBLES -----------------------------------*/
/* ----------------------  PUBLIC FUNCTIONS PROTOTYPES  ----------------------*/
extern Server* Server_new();

extern void Server_start(Server* pServer);

extern void Server_stop(Server* pServer);

extern void Server_free(Server* pServer);

#endif /* SRC_COMMANDO_SERVER_H_ */
