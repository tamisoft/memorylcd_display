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

static app_timer_id_t                        m_display_timer_id;                        /**< Display button handler. */
static uint8_t tx_data[TX_MSG_LENGTH]; /**< SPI TX buffer. */
//static uint8_t *rx_data=NULL; /**< SPI RX buffer. */

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
}

/**@brief Function for starting application timers.
 */
static void display_application_timers_start(void)
{
    uint32_t err_code;

    err_code = app_timer_start(m_display_timer_id, DISPLAY_INTERVAL, NULL);
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
	spi_tx_image(SPI0,0);
}

static void display_timeout_handler(void * p_context)
{
    bool white_fill,black_fill;

    UNUSED_PARAMETER(p_context);

    nrf_gpio_pin_toggle(DISPLAY_ALIVE_LED_PIN);
	nrf_gpio_pin_toggle(DISPLAY_EXTCOMIN);

    // Read the GPIOs for the button pressed
    white_fill = (nrf_gpio_pin_read(DISPLAY_WHITE_FILL_PIN) == 0);
    black_fill = (nrf_gpio_pin_read(DISPLAY_BLACK_FILL_PIN) == 0);
    if (white_fill || black_fill)
        {
            if (white_fill)
                {
                    nrf_gpio_pin_toggle(DISPLAY_WHITE_FILL_LED_PIN);
                    tx_data[10]=0xff;
                    tx_data[28]=0xff;
                    tx_data[46]=0xff;
                    tx_data[64]=0xff;
                }else{ //black fill
                    nrf_gpio_pin_toggle(DISPLAY_BLACK_FILL_LED_PIN);
                    tx_data[10]=0x00;
                    tx_data[28]=0x00;
                    tx_data[46]=0x00;
                    tx_data[64]=0x00;
                }
            spi_tx_image(SPI0,0);
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

    return true;
}
