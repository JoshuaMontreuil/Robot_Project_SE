/**
 * @file  client.h
 *
 * @brief
 *
 * @author joshua
 * @date Feb 12, 2023
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
#ifndef SRC_TELCO_CLIENT_H_
#define SRC_TELCO_CLIENT_H_
#include "../commun.h"
/* ----------------------  INCLUDES ------------------------------------------*/
/* ----------------------  PUBLIC TYPE DEFINITIONS ---------------------------*/
/**
 * \struct Client
 * \brief Client object.
 */
typedef struct Client_t Client;
/* ----------------------  PUBLIC ENUMERATIONS -------------------------------*/
/* ----------------------  PUBLIC STRUCTURES ---------------------------------*/
struct Client_t
{
	const char * ip;
	int un_socket;
	struct sockaddr_in adresse_du_serveur;
	DesDonnees donnees;
};
/* ----------------------  PUBLIC VARIBLES -----------------------------------*/
/* ----------------------  PUBLIC FUNCTIONS PROTOTYPES  ----------------------*/
/**
 * \fn
 * \brief
 */
extern Client* Client_new();
/**
 * \fn
 * \brief
 */
extern void Client_free(Client* pClient);
/**
 * \fn
 * \brief
 */
extern void Client_start(Client* pClient);
/**
 * \fn
 * \brief
 */
extern void Client_stop(Client* pClient);
/**
 * \fn
 * \brief
 */
extern void Client_sendMsg(Client* pClient);
/**
 * \fn
 * \brief
 */
extern void Client_readMsg(Client* pClient);
#endif /* SRC_TELCO_CLIENT_H_ */
