/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */  
/**
 * @file  main.c
 *
 * @brief  Launch the Robot's programm for basic version (no modularity, no socket, no task, no error's management)
 *
 * @author Jerome Delatour
 * @date 19-04-2016
 * @version 1
 * @section License
 *
 * The MIT License
 *
 * Copyright (c) 2016, Jerome Delatour
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

#include "telco/remoteUI.h"
#include "commando/server.h"
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
/* ----------------------  PRIVATE CONFIGURATIONS  -------------------------- */
/* ----------------------  PRIVATE TYPE DEFINITIONS  ------------------------ */
/**
 * \enum log_keymain_e
 * \brief Defines the user entry key from their keyboards.
 */
typedef enum
{
	LOG_COMMANDO = '1',
	LOG_TELCO = '2',
	LOG_QUITM = '3'
}log_keymain_e;
/* ----------------------  PRIVATE STRUCTURES  ------------------------------ */
/* ----------------------  PRIVATE ENUMERATIONS  ---------------------------- */
/* ----------------------  PRIVATE VARIABLES  ------------------------------- */
/* ----------------------  PRIVATE FUNCTIONS PROTOTYPES  -------------------- */
static int Main_capture_choice();
static int Main_display();
/* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */
/* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */

/**
 * starts the robot V1 application
 */
int main (int argc, char *argv[])
{
	int main_loop = 0;
	while(main_loop == 0)
	{
		main_loop = Main_display();
	}

	if(main_loop == 1)
	{
		Server * pServer = Server_new();
		Server_start(pServer); //fonction bloquante ici
		Server_stop(pServer);
		Server_free(pServer);
	}
	else if(main_loop == 2)
	{
		RemoteUI* pRemoteUI = RemoteUI_new();
		RemoteUI_start(pRemoteUI);
		RemoteUI_stop(pRemoteUI);
		RemoteUI_free(pRemoteUI);
	}
	else
	{
		printf("Bye \n");
	}
	return 0;
}

static int Main_capture_choice()
{
	int return_value = 0;
	struct termios oldt, newt;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	log_keymain_e command = getchar();
	tcsetattr(STDIN_FILENO,TCSANOW, &oldt);

	switch(command)
		{
			case LOG_COMMANDO:
				return_value = 1;
				break;
			case LOG_TELCO:
				return_value = 2;
				break;
			case LOG_QUITM:
				return_value = 3;
				break;
			default:
				break;
		}
	return return_value;
}


static int Main_display()
{
	printf("--------- Accueil Robot V2 ---------\n");
	printf("-------- Voulez vous lancer : ------\n");
	printf("---------- 1 : Commando ------------\n");
	printf("----------- 2 : Telco --------------\n");
	printf("---------- 3 : Quitter -------------\n");
	return Main_capture_choice();
}


