/* Copyright (c) 2009 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
* @defgroup spi_master_example_main main.c
* @{
* @ingroup spi_master_example
*
* @brief SPI Master Loopback Example Application main file.
*
* This file contains the source code for a sample application using SPI.
*
*/

#include "spi_master.h"
#include "nrf_delay.h"
#include "common.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "spi_master_config.h"
#include <stdlib.h>

static uint8_t tx_data[TX_RX_MSG_LENGTH]; /**< SPI TX buffer. */
static uint8_t *rx_data=NULL; /**< SPI RX buffer. */

#define DELAY_MS               100        /**< Timer Delay in milli-seconds. */


/** @brief Function for testing the SPI master.
 *  @param lsb_first[in] If true, the least significant bits are transferred first.
 *  @param mod_num[in] spi module to be used, either SPI0 or SPI1 from enumeration @ref SPIModuleNumber.
 *  @retval true 
 *  @retval false Error occurred
 */
static bool test_spi_tx_rx(SPIModuleNumber mod_num, uint8_t lsb_first)
{
    // Use SPI0, mode0 with lsb shifted as requested
    uint32_t *spi_base_address = spi_master_init(mod_num, SPI_MODE0, (bool)lsb_first);
    if (spi_base_address == 0)
    {
        return false;
    }
  
    
    // Fill tx_data with some simple pattern, rx is filled with zero's so that after receiving from
    // slave we verify rx_Data is same as tx_data.
    for(uint32_t i = 0; i < TX_RX_MSG_LENGTH; i++)
    {
        tx_data[i] = i;
        rx_data[i] = 0;
    }

    // Transmit TX_RX_MSG_LENGTH bytes from tx_data and receive same number of bytes and data into rx_data
    if(!spi_master_tx_rx(spi_base_address, TX_RX_MSG_LENGTH, (const uint8_t *)tx_data, rx_data))
    {
        return false;
    }

    // Validate that we got all transmitted bytes back in the exact order
    for(uint32_t i = 0; i < TX_RX_MSG_LENGTH; i++)
    {
        if( tx_data[i] != rx_data[i] )
        return false;
    }
    return true;
}


/** @brief Function for main application entry.
 */
int spitest_main(void)
{
    bool ret0, ret1;

    while(true)
    {
        // SPI0
        ret0 = test_spi_tx_rx(SPI0, 1);   /** Test with shift Lsb first mode 0. */
        if (ret0)
        {
            // previous tx/rx was successful with lsb shifted first, try the same with lsb shifted last
            nrf_delay_ms(DELAY_MS);   /** Delay for the events ready signal to be visually seen DEBUG flag is enabled.  */
            ret0 = test_spi_tx_rx(SPI0, 0 );
        }
        else
        {
            // Set gpio pin number ERROR_PIN to convey error, this pin can be connected to LED for visual debug
            NRF_GPIO->OUTSET = (1UL << ERROR_PIN_SPI0);
        }

        // SPI1
        ret1 = test_spi_tx_rx(SPI1, 1 );   /** test with shift Lsb first mode 0 */
        if (ret1)
        {
            // previous tx/rx was successful with lsb shifted first, try the same with lsb shifted last
            nrf_delay_ms(DELAY_MS);   /** Delay for the events ready signal to be visually seen DEBUG flag is enabled.  */
            ret1 = test_spi_tx_rx(SPI1, 0 );
        }
        else
        {
            // Set gpio pin number ERROR_PIN to convey error, this pin can be connected to LED for visual debug
            NRF_GPIO->OUTSET = (1UL << ERROR_PIN_SPI1);
        }

        if (!ret0 && !ret1 )
        {
            while(true)
            {
                // Do nothing. Loop forever.
            }
        }
    }
}
/** @} */
