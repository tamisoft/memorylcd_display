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
#define DISPLAY_INTERVAL                     APP_TIMER_TICKS(100, APP_TIMER_PRESCALER) /**< display alive led toggle interval (ticks). */

#define DELAY_MS               100        /**< Timer Delay in milli-seconds. */

#define SWAPBITS(x) \
	(((x & 0x80) >> 7) | ((x & 0x40) >> 5) | ((x & 0x20) >> 3) | ((x & 0x10) >> 1) \
	| ((x & 0x08) << 1) | ((x & 0x04) << 3) | ((x & 0x02) << 5) | ((x & 0x01) << 7))



//function defs
static void display_timeout_handler(void * p_context);
static void display_leds_init(void);
static void display_timers_init(void);

#endif // DISPLAY_H__

/** @} */
/** @endcond */
