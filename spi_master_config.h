#ifndef SPI_MASTER_CONFIG_H
#define SPI_MASTER_CONFIG_H

#define SPI_OPERATING_FREQUENCY  ( 0x02000000UL << (uint32_t)Freq_1Mbps )  /*!< Slave clock frequency. */

/*  SPI0 */
#define SPI_PSELSCK0              16   /*!< GPIO pin number for SPI clock (note that setting this to 31 will only work for loopback purposes as it not connected to a pin) */
#define SPI_PSELMOSI0             17   /*!< GPIO pin number for Master Out Slave In    */
#define SPI_PSELMISO0             0xffffffff   /*!< GPIO pin number for Master In Slave Out -> Not Connected   */
#define SPI_PSELSS0               18   /*!< GPIO pin number for Slave Select           */

/* SPI1 */ //not going to use it now
#define SPI_PSELSCK1              0xffffffff
#define SPI_PSELMOSI1             0xffffffff
#define SPI_PSELMISO1             0xffffffff
#define SPI_PSELSS1               0xffffffff

#define TIMEOUT_COUNTER          0xff00UL  /*!< timeout for getting rx bytes from slave */

#define TX_MSG_LENGTH   2306

#endif /* SPI_MASTER_CONFIG_H */
