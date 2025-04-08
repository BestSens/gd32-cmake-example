/*!
    \file    lcd_log.c
    \brief   this file provides all the LCD Log firmware functions

    \version 2024-12-20, V3.3.1, firmware for GD32F4xx
*/

/*
    Copyright (c) 2024, GigaDevice Semiconductor Inc.

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

#include <stdio.h>
#include "lcd_log.h"
#include "usb_lcd_conf.h"

uint32_t lcd_line_color;
uint16_t lcd_cachebuf_xptr;
uint16_t lcd_cachebuf_yptr_top;
uint16_t lcd_cachebuf_yptr_bottom;
uint16_t lcd_scrollback_step;

uint16_t lcd_cachebuf_yptr_top_bak;
uint16_t lcd_cachebuf_yptr_bottom_bak;
LCD_LOG_line lcd_cachebuf[LCD_CACHE_DEPTH];

ControlStatus lcd_cachebuf_yptr_invert;
ControlStatus lcd_scroll_active;
ControlStatus lcd_lock;
ControlStatus lcd_scrolled;

/*!
    \brief      initialize the LCD Log module
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lcd_log_init(void)
{
    lcd_log_deinit();

    lcd_clear(LCD_COLOR_BLACK);
}

/*!
    \brief      de-initialize the LCD Log module
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lcd_log_deinit(void)
{
    lcd_line_color = LCD_LOG_TEXT_COLOR;
    lcd_cachebuf_xptr = 0;
    lcd_cachebuf_yptr_top = 0;
    lcd_cachebuf_yptr_bottom = 0;

    lcd_cachebuf_yptr_top_bak = 0;
    lcd_cachebuf_yptr_bottom_bak = 0;

    lcd_cachebuf_yptr_invert = ENABLE;
    lcd_scroll_active = DISABLE;
    lcd_lock = DISABLE;
    lcd_scrolled = DISABLE;
    lcd_scrollback_step = 0;
}

/*!
    \brief      display the application header (title) on the LCD screen
    \param[in]  ptitle: pointer to the string to be displayed
    \param[in]  start_x: x location
    \param[out] none
    \retval     none
*/
void lcd_log_header_set(uint8_t *ptitle, uint16_t start_x)
{
    lcd_font_set(&font8x16);

    lcd_text_color_set(LCD_COLOR_BLUE);

    lcd_rectangle_fill(LCD_HEADER_X, LCD_HEADER_Y, LCD_FLAG_WIDTH, LCD_FLAG_HEIGHT);

    lcd_background_color_set(LCD_COLOR_BLUE);
    lcd_text_color_set(LCD_COLOR_RED);

    lcd_vertical_string_display(LCD_HEADER_LINE, start_x, ptitle);

    lcd_background_color_set(LCD_COLOR_BLACK);
}

/*!
    \brief      display the application footer (status) on the LCD screen
    \param[in]  pstatus: pointer to the string to be displayed
    \param[in]  start_x: x location
    \param[out] none
    \retval     none
*/
void lcd_log_footer_set(uint8_t *pstatus, uint16_t start_x)
{
    lcd_text_color_set(LCD_COLOR_BLUE);

    lcd_rectangle_fill(LCD_FOOTER_X, LCD_FOOTER_Y, LCD_FLAG_WIDTH, LCD_FLAG_HEIGHT);

    lcd_background_color_set(LCD_COLOR_BLUE);
    lcd_text_color_set(LCD_COLOR_RED);

    lcd_vertical_string_display(LCD_FOOTER_LINE, start_x, pstatus);

    lcd_background_color_set(LCD_COLOR_BLACK);
}

/*!
    \brief      clear the text zone
    \param[in]  start_x, start_y, width, height: zone location
    \param[out] none
    \retval     none
*/
void lcd_log_textzone_clear(uint16_t start_x,
                            uint16_t start_y,
                            uint16_t width,
                            uint16_t height)
{
    lcd_rectangle_fill(start_x, start_y, width, height);
}

/*!
    \brief      redirect the printf to the LCD
    \param[in]  ch: character to be displayed
    \param[in]  f: output file pointer
    \param[out] none
    \retval     none
*/
LCD_LOG_PUTCHAR {
    font_struct *char_font = lcd_font_get();
    uint32_t idx;

    if (lcd_lock == DISABLE) {
        if (lcd_scroll_active == ENABLE) {
            lcd_cachebuf_yptr_bottom = lcd_cachebuf_yptr_bottom_bak;
            lcd_cachebuf_yptr_top = lcd_cachebuf_yptr_top_bak;
            lcd_scroll_active = DISABLE;
            lcd_scrolled = DISABLE;
            lcd_scrollback_step = 0;
        }

        if((lcd_cachebuf_xptr < LCD_FLAG_HEIGHT / char_font->width) && (ch != '\n')) {
            if(ch != '\r') {
                lcd_cachebuf[lcd_cachebuf_yptr_bottom].line[lcd_cachebuf_xptr++] = (uint16_t)ch;
            }
        } else {
            if (lcd_cachebuf_yptr_top >= lcd_cachebuf_yptr_bottom) {
                if (lcd_cachebuf_yptr_invert == DISABLE) {
                    lcd_cachebuf_yptr_top++;

                    if(lcd_cachebuf_yptr_top == LCD_CACHE_DEPTH) {
                        lcd_cachebuf_yptr_top = 0;
                    }
                } else {
                    lcd_cachebuf_yptr_invert = DISABLE;
                }
            }

            for (idx = lcd_cachebuf_xptr; idx < LCD_FLAG_HEIGHT / char_font->width; idx++) {
                lcd_cachebuf[lcd_cachebuf_yptr_bottom].line[lcd_cachebuf_xptr++] = ' ';
            }

            lcd_cachebuf[lcd_cachebuf_yptr_bottom].color = lcd_line_color;
            lcd_cachebuf_xptr = 0;
            LCD_LOG_UpdateDisplay();
            lcd_cachebuf_yptr_bottom++;

            if (lcd_cachebuf_yptr_bottom == LCD_CACHE_DEPTH) {
                lcd_cachebuf_yptr_bottom = 0;
                lcd_cachebuf_yptr_top = 1;
                lcd_cachebuf_yptr_invert = ENABLE;
            }

            if((ch != '\n') && (ch != '\r')) {
                lcd_cachebuf[lcd_cachebuf_yptr_bottom].line[lcd_cachebuf_xptr++] = (uint16_t)ch;
            }
        }
    }

    return ch;
}

/*!
    \brief      update the text area display
    \param[in]  none
    \param[out] none
    \retval     none
*/
void LCD_LOG_UpdateDisplay(void)
{
    uint8_t cnt = 0 ;
    uint16_t length = 0 ;
    uint16_t ptr = 0, index = 0;

    font_struct *cFont = lcd_font_get();

    if((lcd_cachebuf_yptr_bottom < (YWINDOW_SIZE - 1)) && (lcd_cachebuf_yptr_bottom >= lcd_cachebuf_yptr_top)) {
        lcd_text_color_set(lcd_cachebuf[cnt + lcd_cachebuf_yptr_bottom].color);
        lcd_vertical_string_display((YWINDOW_MIN + lcd_cachebuf_yptr_bottom) * cFont->height, 0,
                                    (uint8_t *)(lcd_cachebuf[cnt + lcd_cachebuf_yptr_bottom].line));
    } else {
        if(lcd_cachebuf_yptr_bottom < lcd_cachebuf_yptr_top) {
            /* Virtual length for rolling */
            length = LCD_CACHE_DEPTH + lcd_cachebuf_yptr_bottom ;
        } else {
            length = lcd_cachebuf_yptr_bottom;
        }

        ptr = length - YWINDOW_SIZE + 1;

        for(cnt = 0; cnt < YWINDOW_SIZE; cnt ++) {
            index = (cnt + ptr) % LCD_CACHE_DEPTH;

            lcd_text_color_set(lcd_cachebuf[index].color);
            lcd_vertical_string_display((cnt + YWINDOW_MIN) * cFont->height, 0, (uint8_t *)(lcd_cachebuf[index].line));
        }
    }
}
