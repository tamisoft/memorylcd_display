#include "display.h"
#include "nrf_gpio.h"
#include "boards/nrf6310.h"
#include "app_timer.h"
#include "spi_master.h"
#include "nrf_delay.h"
#include "common.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "spi_master_config.h"
#include <stdlib.h>

static app_timer_id_t                        m_display_timer_id;                        /**< Display refresh handler. */
static app_timer_id_t                        m_display_button_timer_id;                        /**< Display button handler. */
static uint8_t tx_data[TX_MSG_LENGTH]; 		/**< SPI TX buffer. */
static uint8_t dirty_y_min=0xff; 			/**< dirty y coordinate*/
static uint8_t dirty_y_max=0xff; 			/**< dirty y coordinate*/

static bool spi_tx_image(SPIModuleNumber mod_num, uint8_t lsb_first);



static void display_leds_init(void)
{
    nrf_gpio_cfg_output(DISPLAY_WHITE_FILL_LED_PIN);
    nrf_gpio_cfg_output(DISPLAY_BLACK_FILL_LED_PIN);
    nrf_gpio_cfg_output(DISPLAY_ALIVE_LED_PIN);
    nrf_gpio_cfg_output(LED_5);
    nrf_gpio_cfg_output(LED_6);
}


/**@brief Function for the Timer initialization.
 *
 * @details Initializes the timer module. This creates and starts application timers.
 */
static void display_timers_init(void)
{
	uint32_t        err_code;
    err_code = app_timer_create(&m_display_timer_id,
                                APP_TIMER_MODE_REPEATED,
                                display_timeout_handler);
    APP_ERROR_CHECK(err_code);
    
    err_code = app_timer_create(&m_display_button_timer_id,
                                APP_TIMER_MODE_REPEATED,
                                display_button_timeout_handler);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for starting application timers.
 */
static void display_application_timers_start(void)
{
    uint32_t err_code;

    err_code = app_timer_start(m_display_timer_id, DISPLAY_INTERVAL, NULL);
    APP_ERROR_CHECK(err_code);
    err_code = app_timer_start(m_display_button_timer_id, DISPLAY_BUTTON_INTERVAL, NULL);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for initializing buttons.
 */
static void display_buttons_init(void)
{
    nrf_gpio_cfg_sense_input(DISPLAY_WHITE_FILL_PIN,
                             BUTTON_PULL, 
                             NRF_GPIO_PIN_SENSE_LOW);

    nrf_gpio_cfg_sense_input(DISPLAY_BLACK_FILL_PIN,
                             BUTTON_PULL, 
                             NRF_GPIO_PIN_SENSE_LOW);
}


static void display_init(void)
{
	uint8_t i;
	uint16_t x;

    nrf_gpio_cfg_output(DISPLAY_EXTCOMIN);
    nrf_gpio_cfg_output(DISPLAY_DISP);
	nrf_gpio_pin_set(DISPLAY_EXTCOMIN);
	nrf_gpio_pin_set(DISPLAY_DISP);
	
	
	for(x=0;x<TX_MSG_LENGTH;x++) tx_data[x]=0x00; //this clears the display, but also establishes the dummy bytes for display
	
	tx_data[0]=0x80; //M2=H M1=L M0=L
	for (i=0;i<128;i++)
	{
		tx_data[(1+(i*18))] = SWAPBITS( (i+1) );
	}
	CLEAR_SCREEN(1);
	spi_tx_image(SPI0,0);
}

static void clear_area(uint8_t x,uint8_t y,uint8_t w,uint8_t h, bool color)
{
	uint8_t ty,tx,orig,fb,lb,fbp,lbp; //tempx-y, orig data, fractional first byte, first byte, fractional last byte, last byte
	uint8_t ffb,flb,out;
	uint16_t addr;
	//sanity checks not to get out of bounds
	if (x>127) x=127;
	if (y>127) y=127;
	if ((x+(w-1))>127) w=128-x;
	if ((y+(h-1))>127) h=128-y;

	dirty_y_min	= y;
	dirty_y_max = y+(h-1);

	ffb=((x%8)==0)?0:1;
	flb=(((x+(w-1))%8)==7)?0:1;
	fb=(0xff>>(x%8));	
	lb=(uint8_t)(0xff<<(7-((x+(w-1))%8)));
	fbp=x>>3; // x/8
	lbp=(x+w-1)>>3; //(x+w)/8

    for (ty=y;ty<(y+h);ty++)
    {
      for (tx=fbp;tx<=lbp;tx++)
      {
        addr=(2+ty*18+tx);
        orig=tx_data[addr];
        if ((tx==fbp)&&(tx==lbp)&&(ffb|flb)) //when we are manipulating inside the same byte and not all
        {
            if (color==1)
              {
                out=((fb&lb) | orig);
              }else{
                out=(~(fb&lb) & orig);
              }
        }else{
          if ((tx==fbp) && (ffb))
           {
            if (color==1)
              {
                out=orig | fb;
              }else{
                out=orig & ~fb;
              }
           }else{
            if ((tx==lbp) && (flb))
             {
                if (color==1)
                  {
                    out=orig | lb;
                  }else{
                    out=orig & ~lb;
                  }
             }else{
                if (color==1)
                  {
                    out=0xff;
                  }else{
                    out=0x00;
                  }
             }
           }
        }
      
        tx_data[addr]=out;

      } //tx
    } //ty

}

static void display_timeout_handler(void * p_context)
{
     UNUSED_PARAMETER(p_context);
     nrf_gpio_pin_toggle(DISPLAY_EXTCOMIN);
	 if ((dirty_y_max!=255) || (dirty_y_min!=255)) //only refresh the screen if there are dirty lines
		spi_tx_image(SPI0,0);
 }

static void display_button_timeout_handler(void * p_context)
{
    bool white_fill,black_fill;

    UNUSED_PARAMETER(p_context);

    // Read the GPIOs for the button pressed
    white_fill = (nrf_gpio_pin_read(DISPLAY_WHITE_FILL_PIN) == 0);
    black_fill = (nrf_gpio_pin_read(DISPLAY_BLACK_FILL_PIN) == 0);
    if (white_fill || black_fill)
        {
            if (white_fill)
                {
                    nrf_gpio_pin_toggle(DISPLAY_WHITE_FILL_LED_PIN);
                    clear_area(40,40,5,5,1);
                }else{ //black fill
                    nrf_gpio_pin_toggle(DISPLAY_BLACK_FILL_LED_PIN);
                    clear_area(32,32,64,64,0);
                }
        }
}


static bool spi_tx_image(SPIModuleNumber mod_num, uint8_t lsb_first)
{
    // Use SPI0, mode0 with msb shifted as requested
    uint32_t *spi_base_address = spi_master_init(mod_num, SPI_MODE0, (bool)lsb_first);
    if (spi_base_address == 0)
    {
        return false;
    }
    nrf_gpio_pin_toggle(LED_5);
    // Transmit TX_RX_MSG_LENGTH bytes from tx_data and receive same number of bytes and data into rx_data
    if(!spi_master_tx_rx(spi_base_address, TX_MSG_LENGTH, (const uint8_t *)tx_data, NULL))
    {
        return false;
    }
    nrf_gpio_pin_toggle(LED_6);
	//updates are done let's flag that we don't need more
	dirty_y_max=255;
	dirty_y_min=255;
    return true;
}
