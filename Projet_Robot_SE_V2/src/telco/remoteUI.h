/**
 * @file  remoteUI.h
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
#ifndef SRC_TELCO_REMOTEUI_H_
#define SRC_TELCO_REMOTEUI_H_
/* ----------------------  INCLUDES ------------------------------------------*/
#include "client.h"
#include "../commun.h"
#include "prose.h"
/* ----------------------  PUBLIC TYPE DEFINITIONS ---------------------------*/
typedef struct RemoteUI_t RemoteUI;
/* ----------------------  PUBLIC ENUMERATIONS -------------------------------*/
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
/* ----------------------  PUBLIC STRUCTURES ---------------------------------*/
/* ----------------------  PUBLIC VARIBLES -----------------------------------*/
/* ----------------------  PUBLIC FUNCTIONS PROTOTYPES  ----------------------*/
/**
 * \fn extern void RemoteUI_start(RemoteUI* pRemoteUI)
 * \brief Start RemoteUI and waits for the user's input until the user ask to quit.
 */
extern void RemoteUI_start(RemoteUI* pRemoteUI);

/**
 * \fn extern void RemoteUI_stop(RemoteUI* pRemoteUI)
 * \brief Stop RemoteUI.
 */
extern void RemoteUI_stop(RemoteUI* pRemoteUI);

/**
 * \fn extern RemoteUI* RemoteUI_new()
 * \brief Initialize in memory RemoteUI.
 */
extern RemoteUI* RemoteUI_new();

/**
 * \fn extern void RemoteUI_free(RemoteUI* pRemoteUI)
 * \brief Destruct the RemoteUI from memory.
 */
extern void RemoteUI_free(RemoteUI* pRemoteUI);

#endif /* SRC_TELCO_REMOTEUI_H_ */
