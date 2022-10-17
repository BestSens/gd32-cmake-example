/*!
    \file    usbh_usr_lcd.h
    \brief   the declarations for user routines for LCD 

    \version 2020-08-01, V3.0.0, firmware for GD32F4xx
    \version 2022-03-09, V3.1.0, firmware for GD32F4xx
    \version 2022-06-30, V3.2.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2022, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#ifndef __USBH_USR_LCD_H
#define __USBH_USR_LCD_H

#include "usbh_hid_core.h"

/* left button: report data: 0x00*/
#define HID_MOUSE_LEFTBUTTON                0x00U

/* right button: report data: 0x01*/
#define HID_MOUSE_RIGHTBUTTON               0x01U

/* middle button: report data: 0x02*/
#define HID_MOUSE_MIDDLEBUTTON              0x02U

/* mouse directions */
#define MOUSE_TOP_DIR                       0x80U
#define MOUSE_BOTTOM_DIR                    0x00U
#define MOUSE_LEFT_DIR                      0x80U
#define MOUSE_RIGHT_DIR                     0x00U

#define MOUSE_WINDOW_X                      50U
#define MOUSE_WINDOW_Y                      0U
#define MOUSE_WINDOW_HEIGHT                 272U
#define MOUSE_WINDOW_WIDTH                  190U
#define MOUSE_WINDOW_X_MAX                  MOUSE_WINDOW_Y + MOUSE_WINDOW_HEIGHT
#define MOUSE_WINDOW_Y_MAX                  MOUSE_WINDOW_X + MOUSE_WINDOW_WIDTH

#define MOUSE_BUTTON_HEIGHT                 60U
#define MOUSE_BUTTON_WIDTH                  40U
#define MOUSE_BUTTON_XCHORD                 280U
#define MOUSE_BUTTON1_YCHORD                15U
#define MOUSE_BUTTON3_YCHORD                105U
#define MOUSE_BUTTON2_YCHORD                195U

#define MOUSE_SPL_X                         50U
#define MOUSE_SPL_Y                         0U

#define COMPUTE_XLOC(xloc, x)               (xloc) -= ((x) / 2)
#define COMPUTE_YLOC(yloc, y)               (yloc) += ((y) / 2)

#define Y_MIN                               MOUSE_WINDOW_X
#define X_MIN                               2U

#define MPL_X(x)                            (x)
#define MPL_Y(y)                            (y)

#define KYBRD_WINDOW_X                      70
#define KYBRD_WINDOW_Y                      0
#define KYBRD_WINDOW_WIDTH                  210
#define KYBRD_WINDOW_HEIGHT                 272

#define KYBRD_FIRST_COLUMN                  (uint16_t)256
#define KYBRD_LAST_COLUMN                   (uint16_t)0
#define KYBRD_FIRST_LINE                    (uint16_t)80
#define KYBRD_LAST_LINE                     (uint16_t)270

#define UPDATE_XP(xpos)                     (xpos) += SMALL_FONT_LINE_WIDTH
#define UPDATE_YP(ypos)                     (ypos) -= SMALL_FONT_COLUMN_WIDTH

#define KYBRD_LINE_LIMIT(xline)             ((xline) >= KYBRD_LAST_LINE)

#define CHAR_CURSOR(x, y)                   (x), (y)

#define MOUSE_LEFT_MOVE                     1U
#define MOUSE_RIGHT_MOVE                    2U
#define MOUSE_UP_MOVE                       3U
#define MOUSE_DOWN_MOVE                     4U

#define HID_MOUSE_HEIGHTLSB                 2U
#define HID_MOUSE_WIDTHLSB                  2U
#define HID_MOUSE_RES_X                     4U
#define HID_MOUSE_RES_Y                     4U

#define SMALL_FONT_COLUMN_WIDTH             8U
#define SMALL_FONT_LINE_WIDTH               16U

/* function declarations */
/* the function is to handle mouse scroll to update the mouse position on display window */
void hid_mouse_update_position (int8_t X, int8_t Y);
/* the function is to handle mouse button press */
void hid_mouse_button_pressed (uint8_t ButtonIdx);
/* the function is to handle mouse button release */
void hid_mouse_button_released (uint8_t ButtonIdx);

#endif /* __USBH_USR_LCD_H */
