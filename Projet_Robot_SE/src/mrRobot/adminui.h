/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */  
/**
 * @file  adminUI.h
 *
 * @brief  This component describe the admin user interface. It captures the user's controls and send them to the pilot module.
 *
 * @author Joshua Montreuil
 * @date 27-01-2023
 * @version 1.1
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

#ifndef ADMINUI_H
#define ADMINUI_H

//----INCLUDES -----------------------------------------------------------------
#include "pilot.h"

//---- ENUM --------------------------------------------------------------------

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

//----PUBLIC FUNCTION DECLARATIONS ---------------------------------------------
/**
 * \fn extern void AdminUI_start()
 * \brief Start AdminUI and waits for the user's input until the user ask to quit.
 */
extern void AdminUI_start();

/**
 * \fn extern void AdminUI_stop()
 * \brief Stop AdminUI.
 */
extern void AdminUI_stop();

/**
 * \fn extern void AdminUI_new()
 * \brief Initialize in memory AdminUI.
 */
extern void AdminUI_new();

/**
 * \fn extern void AdminUI_free()
 * \brief Destruct the AdminUI from memory.
 */
extern void AdminUI_free();

#endif /* ADMINUI_H */
