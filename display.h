/* Copyright (c) 2014 Navicron Oy. All Rights Reserved.
 *
 * The information contained herein is property of Navicron Oy.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

 /** @cond To make doxygen skip this file */
 
#ifndef DISPLAY_H__
#define DISPLAY_H__
#include "fonts/smallcharset.h"
#include "fonts/bignumbers.h"

// defines
//buttons to read
#define DISPLAY_WHITE_FILL_PIN               BUTTON_2                                   /**< Fill the display white */
#define DISPLAY_BLACK_FILL_PIN               BUTTON_3                                   /**< Fill the display black */
//leds to use
#define DISPLAY_WHITE_FILL_LED_PIN           LED_2                                      /**< Is on when white fill is pressed */
#define DISPLAY_BLACK_FILL_LED_PIN           LED_3                                      /**< Is on when black fill is pressed */
#define DISPLAY_ALIVE_LED_PIN                LED_4                                      /**< Is on blinking when display handling is called */
//signals
#define DISPLAY_EXTCOMIN          19  /**< extcomin gpio*/
#define DISPLAY_DISP              20  /**< disp gpio*/

//callback timers
#define DISPLAY_INTERVAL                     APP_TIMER_TICKS(100, APP_TIMER_PRESCALER) /**< display refresh interval (ticks). */
#define DISPLAY_BUTTON_INTERVAL              APP_TIMER_TICKS(50, APP_TIMER_PRESCALER) /**< display alive led toggle interval (ticks). */

#define DELAY_MS               100        /**< Timer Delay in milli-seconds. */

#define SWAPBITS(x) \
	(((x & 0x80) >> 7) | ((x & 0x40) >> 5) | ((x & 0x20) >> 3) | ((x & 0x10) >> 1) \
	| ((x & 0x08) << 1) | ((x & 0x04) << 3) | ((x & 0x02) << 5) | ((x & 0x01) << 7))

#define CLEAR_SCREEN(x) clear_area(0,0,128,128,x)

#define FONT_TRANSPARENTBG 0x80
#define FONT_INVERSECOLOR 0x40
#define FONT_XORCHAR 0x20
#define FONT_FLAGMASK 0xf0
#define FONT_FONTMASK 0x0f
#define FONT_SMALL 0x00
#define FONT_BIG 0x01

//function defs
static void display_timeout_handler(void * p_context);
static void display_button_timeout_handler(void * p_context);
static void display_leds_init(void);
static void display_timers_init(void);
static void clear_area(uint8_t x,uint8_t y,uint8_t w,uint8_t h, bool color);
static void print(uint8_t x, uint8_t y,uint8_t font,uint8_t * str);
static uint8_t putchr(uint8_t x, uint8_t y,uint8_t font,uint8_t chr,uint8_t setdirty);



#endif // DISPLAY_H__

/** @} */
/** @endcond */
