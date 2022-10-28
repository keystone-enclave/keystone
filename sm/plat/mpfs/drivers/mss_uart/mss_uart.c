/*******************************************************************************
 * Copyright 2019-2020 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * PolarFire SoC Microprocessor Subsystem MMUART bare metal software driver
 * implementation.
 *
 */

#include "mss_uart.h"
#include "mss_uart_regs.h"
#include "../../clocks/hw_mss_clks.h"
#include <assert.h>
#define ASSERT sm_assert

#ifdef __cplusplus
extern "C" {
#endif

#define MSS_UART0_LO_BASE           (MSS_UART_TypeDef*)0x20000000UL
#define MSS_UART1_LO_BASE           (MSS_UART_TypeDef*)0x20100000UL
#define MSS_UART2_LO_BASE           (MSS_UART_TypeDef*)0x20102000UL
#define MSS_UART3_LO_BASE           (MSS_UART_TypeDef*)0x20104000UL
#define MSS_UART4_LO_BASE           (MSS_UART_TypeDef*)0x20106000UL

#define MSS_UART0_HI_BASE           (MSS_UART_TypeDef*)0x28000000UL
#define MSS_UART1_HI_BASE           (MSS_UART_TypeDef*)0x28100000UL
#define MSS_UART2_HI_BASE           (MSS_UART_TypeDef*)0x28102000UL
#define MSS_UART3_HI_BASE           (MSS_UART_TypeDef*)0x28104000UL
#define MSS_UART4_HI_BASE           (MSS_UART_TypeDef*)0x28106000UL


mss_uart_instance_t g_mss_uart0_lo;
mss_uart_instance_t g_mss_uart1_lo;
mss_uart_instance_t g_mss_uart2_lo;
mss_uart_instance_t g_mss_uart3_lo;
mss_uart_instance_t g_mss_uart4_lo;

mss_uart_instance_t g_mss_uart0_hi;
mss_uart_instance_t g_mss_uart1_hi;
mss_uart_instance_t g_mss_uart2_hi;
mss_uart_instance_t g_mss_uart3_hi;
mss_uart_instance_t g_mss_uart4_hi;

/* This variable tracks if the UART peripheral is located on S5 or S6 on AXI
 * switch. This will be used to determine which UART instance to be passed to
 * UART interrupt handler. value 0 = S5(low). value 1 = S6(high)
 * Bit positions:
 * 0 ==> MMUART0
 * 1 ==> MMUART1
 * 2 ==> MMUART2
 * 3 ==> MMUART3
 * 4 ==> MMUART4

 */
static uint8_t g_uart_axi_pos = 0x0u;

/*******************************************************************************
 * Defines
 */
#define TX_COMPLETE                     0u
#define TX_FIFO_SIZE                    16u

#define FCR_TRIG_LEVEL_MASK             0xC0u

#define IIRF_MASK                       0x0Fu

#define INVALID_INTERRUPT               0u

#define MSS_UART_DATA_READY             ((uint8_t) 0x01)

#define SYNC_ASYNC_MODE_MASK            (0x7u)

/*******************************************************************************
 * Possible values for Interrupt Identification Register Field.
 */
#define IIRF_MODEM_STATUS               0x00u
#define IIRF_THRE                       0x02u
#define IIRF_MMI                        0x03u
#define IIRF_RX_DATA                    0x04u
#define IIRF_RX_LINE_STATUS             0x06u
#define IIRF_DATA_TIMEOUT               0x0Cu

/*******************************************************************************
 * Receiver error status mask.
 */
#define STATUS_ERROR_MASK    ( MSS_UART_OVERUN_ERROR | MSS_UART_PARITY_ERROR | \
                               MSS_UART_FRAMING_ERROR  | MSS_UART_BREAK_ERROR | \
                               MSS_UART_FIFO_ERROR)

/*******************************************************************************
 * Local functions.
 */
static void global_init(mss_uart_instance_t * this_uart, uint32_t baud_rate,
                                                         uint8_t line_config);
static void config_baud_divisors
(
    mss_uart_instance_t * this_uart,
    uint32_t baudrate
);

/*******************************************************************************
 * Public Functions
 *******************************************************************************/
/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
void
MSS_UART_init
(
    mss_uart_instance_t* this_uart,
    uint32_t baud_rate,
    uint8_t line_config
)
{
    /* Perform generic initialization */
    global_init(this_uart, baud_rate, line_config);

    /* Disable LIN mode */
    this_uart->hw_reg->MM0 &= ~ELIN_MASK;

    /* Disable IrDA mode */
    this_uart->hw_reg->MM1 &= ~EIRD_MASK;

    /* Disable SmartCard Mode */
    this_uart->hw_reg->MM2 &= ~EERR_MASK;
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
void MSS_UART_lin_init
(
    mss_uart_instance_t* this_uart,
    uint32_t baud_rate,
    uint8_t line_config
)
{
    /* Perform generic initialization */
    global_init(this_uart, baud_rate, line_config);

     /* Enable LIN mode */
    this_uart->hw_reg->MM0 |= ELIN_MASK;

    /* Disable IrDA mode */
    this_uart->hw_reg->MM1 &= ~EIRD_MASK;

    /* Disable SmartCard Mode */
    this_uart->hw_reg->MM2 &= ~EERR_MASK;
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
void
MSS_UART_irda_init
(
    mss_uart_instance_t* this_uart,
    uint32_t baud_rate,
    uint8_t line_config,
    mss_uart_rzi_polarity_t rxpol,
    mss_uart_rzi_polarity_t txpol,
    mss_uart_rzi_pulsewidth_t pw
)
{
    /* Perform generic initialization */
    global_init(this_uart, baud_rate, line_config);

     /* Enable LIN mode */
    this_uart->hw_reg->MM0 &= ~ELIN_MASK;

    /* Disable IrDA mode */
    this_uart->hw_reg->MM1 |= EIRD_MASK;

    ((rxpol == MSS_UART_ACTIVE_LOW) ? (this_uart->hw_reg->MM1 &= ~EIRX_MASK) :
                                      (this_uart->hw_reg->MM1 |= EIRX_MASK));

    ((txpol == MSS_UART_ACTIVE_LOW) ? (this_uart->hw_reg->MM1 &= ~EITX_MASK) :
                                      (this_uart->hw_reg->MM1 |= EITX_MASK));

    ((pw == MSS_UART_3_BY_16) ? (this_uart->hw_reg->MM1 &= ~EITP_MASK) :
                                      (this_uart->hw_reg->MM1 |= EITP_MASK));
    /* Disable SmartCard Mode */
    this_uart->hw_reg->MM2 &= ~EERR_MASK;
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
void
MSS_UART_smartcard_init
(
    mss_uart_instance_t* this_uart,
    uint32_t baud_rate,
    uint8_t line_config
)
{
    /* Perform generic initialization */
    global_init(this_uart, baud_rate, line_config);

    /* Disable LIN mode */
    this_uart->hw_reg->MM0 &= ~ELIN_MASK;

    /* Disable IrDA mode */
    this_uart->hw_reg->MM1 &= ~EIRD_MASK;

    /* Enable SmartCard Mode : Only when data is 8-bit and 2 stop bits*/
    if ((MSS_UART_DATA_8_BITS | MSS_UART_TWO_STOP_BITS) ==
        (line_config & (MSS_UART_DATA_8_BITS | MSS_UART_TWO_STOP_BITS)))
    {
        this_uart->hw_reg->MM2 |= EERR_MASK;

        /* Enable single wire half-duplex mode */
        this_uart->hw_reg->MM2 |= ESWM_MASK;
    }
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
void
MSS_UART_polled_tx
(
    mss_uart_instance_t * this_uart,
    const uint8_t * pbuff,
    uint32_t tx_size
)
{
    uint32_t char_idx = 0u;
    uint32_t size_sent;
    uint8_t status;
    uint32_t temp_tx_size = tx_size;

    ASSERT(pbuff != ( (uint8_t*)0));
    ASSERT(tx_size > 0u);

    if ((pbuff != ((uint8_t*)0)) && (temp_tx_size > 0u))
    {
        /* Remain in this loop until the entire input buffer
         * has been transferred to the UART.
         */
        do
        {
            /* Read the Line Status Register and update the sticky record */
            status = this_uart->hw_reg->LSR;
            this_uart->status |= status;

            /* Check if TX FIFO is empty. */
            if (status & MSS_UART_THRE)
            {
                uint32_t fill_size = TX_FIFO_SIZE;

                /* Calculate the number of bytes to transmit. */
                if (temp_tx_size < TX_FIFO_SIZE)
                {
                    fill_size = temp_tx_size;
                }

                /* Fill the TX FIFO with the calculated the number of bytes. */
                for (size_sent = 0u; size_sent < fill_size; ++size_sent)
                {
                    /* Send next character in the buffer. */
                    this_uart->hw_reg->THR = pbuff[char_idx];
                    char_idx++;
                }

                /* Calculate the number of bytes remaining(not transmitted yet)*/
                temp_tx_size -= size_sent;
            }
        }while (temp_tx_size);
    }
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
void
MSS_UART_polled_tx_string
(
    mss_uart_instance_t * this_uart,
    const uint8_t * p_sz_string
)
{
    uint32_t char_idx = 0u;
    uint32_t fill_size;
    uint8_t data_byte;
    uint8_t status;

    ASSERT(p_sz_string != ((uint8_t*)0));

    if (p_sz_string != ((uint8_t*)0))
    {
        /* Get the first data byte from the input buffer */
        data_byte = p_sz_string[char_idx];

        /* First check for the NULL terminator byte.
         * Then remain in this loop until the entire string in the input buffer
         * has been transferred to the UART.
         */
        while (0u != data_byte)
        {
            /* Wait until TX FIFO is empty. */
            do
            {
                status = this_uart->hw_reg->LSR;
                this_uart->status |= status;
            }while (0u == (status & MSS_UART_THRE));

            /* Send bytes from the input buffer until the TX FIFO is full
             * or we reach the NULL terminator byte.
             */
            fill_size = 0u;

            while ((0u != data_byte) && (fill_size < TX_FIFO_SIZE))
            {
                /* Send the data byte */
                this_uart->hw_reg->THR = data_byte;
                ++fill_size;
                char_idx++;
                /* Get the next data byte from the input buffer */
                data_byte = p_sz_string[char_idx];
            }
        }
    }
}


/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
int8_t
MSS_UART_tx_complete
(
    mss_uart_instance_t * this_uart
)
{
    int8_t ret_value = 0;
    uint8_t status = 0u;

    /* Read the Line Status Register and update the sticky record. */
    status = this_uart->hw_reg->LSR;
    this_uart->status |= status;

    if ((TX_COMPLETE == this_uart->tx_buff_size) &&
       ((status & MSS_UART_TEMT) != 0u))
    {
        ret_value = (int8_t)1;
    }

    return ret_value;
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
size_t
MSS_UART_get_rx
(
    mss_uart_instance_t * this_uart,
    uint8_t * rx_buff,
    size_t buff_size
)
{
    size_t rx_size = 0u;
    uint8_t status = 0u;

    ASSERT(rx_buff != ((uint8_t*)0));
    ASSERT(buff_size > 0u);

    if ((rx_buff != (uint8_t*)0) && (buff_size > 0u))
    {
        status = this_uart->hw_reg->LSR;
        this_uart->status |= status;

        while (((status & MSS_UART_DATA_READY) != 0u) && (rx_size < buff_size))
        {
            rx_buff[rx_size] = this_uart->hw_reg->RBR;
            ++rx_size;
            status = this_uart->hw_reg->LSR;
            this_uart->status |= status;
        }
    }

    return rx_size;
}


/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
void
MSS_UART_set_loopback
(
    mss_uart_instance_t *   this_uart,
    mss_uart_loopback_t     loopback
)
{
    ASSERT(MSS_UART_INVALID_LOOPBACK > loopback);

    if (MSS_UART_INVALID_LOOPBACK > loopback)
    {
        switch (loopback)
        {
            case MSS_UART_LOCAL_LOOPBACK_OFF:
                /* Disable local loopback */
                this_uart->hw_reg->MCR &= ~LOOP_MASK;
            break;

            case MSS_UART_LOCAL_LOOPBACK_ON:
                /* Enable local loopback */
                this_uart->hw_reg->MCR |= LOOP_MASK;
            break;

            case MSS_UART_REMOTE_LOOPBACK_OFF:
            case MSS_UART_AUTO_ECHO_OFF:
                /* Disable remote loopback & automatic echo*/
                this_uart->hw_reg->MCR &= ~(RLOOP_MASK|ECHO_MASK);
            break;

            case MSS_UART_REMOTE_LOOPBACK_ON:
                /* Enable remote loopback */
                this_uart->hw_reg->MCR |= (1u << RLOOP);
                break;

            case MSS_UART_AUTO_ECHO_ON:
                /* Enable automatic echo */
                this_uart->hw_reg->MCR |= (1u << ECHO);
            break;

            case MSS_UART_INVALID_LOOPBACK:
                /* Fall through to default. */
            default:
                ASSERT(0);
            break;
        }
    }
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
size_t
MSS_UART_fill_tx_fifo
(
    mss_uart_instance_t * this_uart,
    const uint8_t * tx_buffer,
    size_t tx_size
)
{
    uint8_t status = 0u;
    uint32_t size_sent = 0u;

    ASSERT(tx_buffer != ( (uint8_t*)0));
    ASSERT(tx_size > 0);

    /* Fill the UART's Tx FIFO until the FIFO is full or the complete input
     * buffer has been written. */
    if ((tx_buffer != ((uint8_t*)0)) && (tx_size > 0u))
    {
        status = this_uart->hw_reg->LSR;
        this_uart->status |= status;

        if (status & MSS_UART_THRE)
        {
            uint32_t fill_size = TX_FIFO_SIZE;

            if (tx_size < TX_FIFO_SIZE)
            {
                fill_size = tx_size;
            }

            /* Fill up FIFO */
            for (size_sent = 0u; size_sent < fill_size; size_sent++)
            {
                /* Send next character in the buffer. */
                this_uart->hw_reg->THR = tx_buffer[size_sent];
            }
        }
    }

    return size_sent;
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
uint8_t
MSS_UART_get_rx_status
(
    mss_uart_instance_t * this_uart
)
{
    uint8_t status = MSS_UART_INVALID_PARAM;

    /*
     * Extract UART receive error status.
     * Bit 1 - Overflow error status
     * Bit 2 - Parity error status
     * Bit 3 - Frame error status
     * Bit 4 - Break interrupt indicator
     * Bit 7 - FIFO data error status
     */
    this_uart->status |= (this_uart->hw_reg->LSR);
    status = (this_uart->status & STATUS_ERROR_MASK);
    /* Clear the sticky status after reading */
    this_uart->status = 0u;

    return status;
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
uint8_t
MSS_UART_get_modem_status
(
    const mss_uart_instance_t * this_uart
)
{
    uint8_t status = MSS_UART_INVALID_PARAM;

    /*
     * Extract UART modem status and place in lower bits of "status".
     * Bit 0 - Delta Clear to Send Indicator
     * Bit 1 - Delta Clear to Receive Indicator
     * Bit 2 - Trailing edge of Ring Indicator detector
     * Bit 3 - Delta Data Carrier Detect indicator
     * Bit 4 - Clear To Send
     * Bit 5 - Data Set Ready
     * Bit 6 - Ring Indicator
     * Bit 7 - Data Carrier Detect
     */
    status = this_uart->hw_reg->MSR;

    return status;
}

/***************************************************************************//**
 * MSS_UART_get_tx_status.
 * See mss_uart.h for details of how to use this function.
 */
uint8_t
MSS_UART_get_tx_status
(
    mss_uart_instance_t * this_uart
)
{
    uint8_t status = MSS_UART_TX_BUSY;

    /* Read the Line Status Register and update the sticky record. */
    status = this_uart->hw_reg->LSR;
    this_uart->status |= status;

    /*
     * Extract the transmit status bits from the UART's Line Status Register.
     * Bit 5 - Transmitter Holding Register/FIFO Empty (THRE) status.
               (If = 1, TX FIFO is empty)
     * Bit 6 - Transmitter Empty (TEMT) status.
               (If = 1, both TX FIFO and shift register are empty)
     */
    status &= (MSS_UART_THRE | MSS_UART_TEMT);

    return status;
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
void
MSS_UART_set_break
(
    mss_uart_instance_t * this_uart
)
{
    /* set break character on Tx line */
    this_uart->hw_reg->LCR |= SB_MASK;
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
void
MSS_UART_clear_break
(
    mss_uart_instance_t * this_uart
)
{
    /* remove break character from Tx line */
    this_uart->hw_reg->LCR &= ~SB_MASK;
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
void
MSS_UART_enable_half_duplex
(
    mss_uart_instance_t * this_uart
)
{
    /* enable single wire half-duplex mode */
    this_uart->hw_reg->MM2 |= ESWM_MASK;
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
void
MSS_UART_disable_half_duplex
(
    mss_uart_instance_t * this_uart
)
{
    /* enable single wire half-duplex mode */
    this_uart->hw_reg->MM2 &= ~ESWM_MASK;
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
void
MSS_UART_set_rx_endian
(
    mss_uart_instance_t * this_uart,
    mss_uart_endian_t endian
)
{
    ASSERT(MSS_UART_INVALID_ENDIAN > endian);

    if (MSS_UART_INVALID_ENDIAN > endian)
    {
        /* Configure MSB first / LSB first for receiver */
        ((MSS_UART_LITTLEEND == endian) ? (this_uart->hw_reg->MM1 &= ~E_MSB_RX_MASK) :
                                          (this_uart->hw_reg->MM1 |= E_MSB_RX_MASK));
    }
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
void
MSS_UART_set_tx_endian
(
    mss_uart_instance_t * this_uart,
    mss_uart_endian_t endian
)
{
    ASSERT(MSS_UART_INVALID_ENDIAN > endian);

    if (MSS_UART_INVALID_ENDIAN > endian)
    {
        /* Configure MSB first / LSB first for transmitter */
        ((MSS_UART_LITTLEEND == endian) ? (this_uart->hw_reg->MM1 &= ~E_MSB_TX_MASK) :
                                          (this_uart->hw_reg->MM1 |= E_MSB_TX_MASK)) ;
    }
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
void
MSS_UART_set_filter_length
(
    mss_uart_instance_t * this_uart,
    mss_uart_filter_length_t length
)
{
    ASSERT(MSS_UART_INVALID_FILTER_LENGTH > length);

    if (MSS_UART_INVALID_FILTER_LENGTH > length)
    {
        /* Configure glitch filter length */
        this_uart->hw_reg->GFR = (uint8_t)length;
    }
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
void
MSS_UART_enable_afm
(
     mss_uart_instance_t * this_uart
)
{
    /* Disable RX FIFO till address flag with correct address is received */
    this_uart->hw_reg->MM2 |= EAFM_MASK;
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
void
MSS_UART_disable_afm
(
     mss_uart_instance_t * this_uart
)
{
    /* Enable RX FIFO irrespective of address flag and
       correct address is received */
    this_uart->hw_reg->MM2 &= ~EAFM_MASK;
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
void
MSS_UART_enable_afclear
(
     mss_uart_instance_t * this_uart
)
{
    /* Enable address flag clearing */
    /* Disable RX FIFO till another address flag with
       correct address is received */
    this_uart->hw_reg->MM2 |= EAFC_MASK;
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
void
MSS_UART_disable_afclear
(
     mss_uart_instance_t * this_uart
)
{
    /* Disable address flag clearing */
    this_uart->hw_reg->MM2 &= ~EAFC_MASK;
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
void
MSS_UART_enable_rx_timeout
(
    mss_uart_instance_t * this_uart,
    uint8_t timeout
)
{
    /* Load the receive timeout value */
    this_uart->hw_reg->RTO = timeout;

    /*Enable receiver time-out */
    this_uart->hw_reg->MM0 |= ERTO_MASK;
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
void
MSS_UART_disable_rx_timeout
(
    mss_uart_instance_t * this_uart
)
{
    /*Disable receiver time-out */
    this_uart->hw_reg->MM0 &= ~ERTO_MASK;
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
void
MSS_UART_enable_tx_time_guard
(
    mss_uart_instance_t * this_uart,
    uint8_t timeguard
)
{
    /* Load the transmitter time guard value */
    this_uart->hw_reg->TTG = timeguard;

    /*Enable transmitter time guard */
    this_uart->hw_reg->MM0 |= ETTG_MASK;
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
void
MSS_UART_disable_tx_time_guard
(
    mss_uart_instance_t * this_uart
)
{
    /*Disable transmitter time guard */
    this_uart->hw_reg->MM0 &= ~ETTG_MASK;
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
void
MSS_UART_set_address
(
    mss_uart_instance_t * this_uart,
    uint8_t address
)
{
    this_uart->hw_reg->ADR = address;
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
void
MSS_UART_set_ready_mode
(
    mss_uart_instance_t * this_uart,
    mss_uart_ready_mode_t mode
)
{
    ASSERT(MSS_UART_INVALID_READY_MODE > mode);

    if (MSS_UART_INVALID_READY_MODE > mode )
    {
        /* Configure mode 0 or mode 1 for TXRDY and RXRDY */
        ((MSS_UART_READY_MODE0 == mode) ? (this_uart->hw_reg->FCR &= ~RDYMODE_MASK) :
                                 (this_uart->hw_reg->FCR |= RDYMODE_MASK) );
    }
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
void
MSS_UART_set_usart_mode
(
    mss_uart_instance_t * this_uart,
    mss_uart_usart_mode_t mode
)
{
    ASSERT(MSS_UART_INVALID_SYNC_MODE > mode);

    if (MSS_UART_INVALID_SYNC_MODE > mode)
    {
        /* Nothing to do for the baudrate:
                                operates at PCLK / 2 + glitch filter length */
        /* Clear the ESYN bits 2:0 */
        this_uart->hw_reg->MM0 &= ~SYNC_ASYNC_MODE_MASK;
        this_uart->hw_reg->MM0 |= (uint8_t)mode;
    }
}

/*******************************************************************************
 * Local Functions
 *******************************************************************************/
/*******************************************************************************
 * Global initialization for all modes
 */
static void global_init
(
    mss_uart_instance_t * this_uart,
    uint32_t baud_rate,
    uint8_t line_config
)
{
    if ((&g_mss_uart0_lo == this_uart))
    {
        this_uart->hw_reg = MSS_UART0_LO_BASE;
        g_uart_axi_pos &= ~0x01u;
    }

    else if (&g_mss_uart1_lo == this_uart)
    {

        this_uart->hw_reg = MSS_UART1_LO_BASE;
        g_uart_axi_pos &= ~0x02u;
    }

    else if (&g_mss_uart2_lo == this_uart)
    {
        this_uart->hw_reg = MSS_UART2_LO_BASE;
        g_uart_axi_pos &= ~0x04u;
    }

    else if (&g_mss_uart3_lo == this_uart)
    {
        this_uart->hw_reg = MSS_UART3_LO_BASE;
        g_uart_axi_pos &= ~0x08u;
    }

    else if (&g_mss_uart4_lo == this_uart)
    {
        this_uart->hw_reg = MSS_UART4_LO_BASE;
        g_uart_axi_pos &= ~0x10u;
    }

    else if ((&g_mss_uart0_hi == this_uart))
    {
        this_uart->hw_reg = MSS_UART0_HI_BASE;
        g_uart_axi_pos |= 0x01u;
    }

    else if (&g_mss_uart1_hi == this_uart)
    {
        this_uart->hw_reg = MSS_UART1_HI_BASE;
        g_uart_axi_pos |= 0x02u;
    }

    else if (&g_mss_uart2_hi == this_uart)
    {
        this_uart->hw_reg = MSS_UART2_HI_BASE;
        g_uart_axi_pos |= 0x04u;
    }

    else if (&g_mss_uart3_hi == this_uart)
    {
        this_uart->hw_reg = MSS_UART3_HI_BASE;
        g_uart_axi_pos |= 0x08u;
    }

    else if (&g_mss_uart4_hi == this_uart)
    {
        this_uart->hw_reg = MSS_UART4_HI_BASE;
        g_uart_axi_pos |= 0x10u;
    }
    else
    {
        ASSERT(0); /*Comment to avoid LDRA warning*/
    }

    /* disable interrupts */
    this_uart->hw_reg->IER = 0u;

    /* FIFO configuration */
    this_uart->hw_reg->FCR = 0u;

    /* clear receiver FIFO */
    this_uart->hw_reg->FCR |= CLEAR_RX_FIFO_MASK;

    /* clear transmitter FIFO */
    this_uart->hw_reg->FCR |= CLEAR_TX_FIFO_MASK;

    /* set default READY mode : Mode 0*/
    /* enable RXRDYN and TXRDYN pins. The earlier FCR write to set the TX FIFO
     * trigger level inadvertently disabled the FCR_RXRDY_TXRDYN_EN bit. */
    this_uart->hw_reg->FCR |= RXRDY_TXRDYN_EN_MASK;

    /* disable loopback : local * remote */
    this_uart->hw_reg->MCR &= ~LOOP_MASK;

    this_uart->hw_reg->MCR &= ~RLOOP_MASK;

    /* set default TX endian */
    this_uart->hw_reg->MM1 &= ~E_MSB_TX_MASK;

    /* set default RX endian */
    this_uart->hw_reg->MM1 &= ~E_MSB_RX_MASK;

    /* default AFM : disabled */
    this_uart->hw_reg->MM2 &= ~EAFM_MASK;

    /* disable TX time guard */
    this_uart->hw_reg->MM0 &= ~ETTG_MASK;

    /* set default RX timeout */
    this_uart->hw_reg->MM0 &= ~ERTO_MASK;

    /* disable fractional baud-rate */
    this_uart->hw_reg->MM0 &= ~EFBR_MASK;

    /* disable single wire mode */
    this_uart->hw_reg->MM2 &= ~ESWM_MASK;

    /* set filter to minimum value */
    this_uart->hw_reg->GFR = 0u;

    /* set default TX time guard */
    this_uart->hw_reg->TTG = 0u;

    /* set default RX timeout */
    this_uart->hw_reg->RTO = 0u;

    /*
     * Configure baud rate divisors. This uses the fractional baud rate divisor
     * where possible to provide the most accurate baud rat possible.
     */
    config_baud_divisors(this_uart, baud_rate);

    /* set the line control register (bit length, stop bits, parity) */
    this_uart->hw_reg->LCR = line_config;

    /* Instance setup */
    this_uart->baudrate = baud_rate;
    this_uart->lineconfig = line_config;
    this_uart->tx_buff_size = TX_COMPLETE;
    this_uart->tx_buffer = (const uint8_t*)0;
    this_uart->tx_idx = 0u;

    /* Initialize the sticky status */
    this_uart->status = 0u;
}

/***************************************************************************//**
 * Configure baud divisors using fractional baud rate if possible.
 */
static void
config_baud_divisors
(
    mss_uart_instance_t * this_uart,
    uint32_t baudrate
)
{
    uint32_t baud_value;
    uint32_t baud_value_by_64;
    uint32_t baud_value_by_128;
    uint32_t fractional_baud_value;
    uint64_t pclk_freq;

    this_uart->baudrate = baudrate;

    /* Use the system clock value from hw_platform.h */
    pclk_freq = LIBERO_SETTING_MSS_APB_AHB_CLK;

    /*
     * Compute baud value based on requested baud rate and PCLK frequency.
     * The baud value is computed using the following equation:
     *      baud_value = PCLK_Frequency / (baud_rate * 16)
     */
    baud_value_by_128 = (uint32_t)((8UL * pclk_freq) / baudrate);
    baud_value_by_64 = baud_value_by_128 / 2u;
    baud_value = baud_value_by_64 / 64u;
    fractional_baud_value = baud_value_by_64 - (baud_value * 64u);
    fractional_baud_value += (baud_value_by_128 - (baud_value * 128u))
                             - (fractional_baud_value * 2u);

    /* Assert if integer baud value fits in 16-bit. */
    ASSERT(baud_value <= UINT16_MAX);

    if (baud_value <= (uint32_t)UINT16_MAX)
    {
        if (baud_value > 1u)
        {
            /*
             * Use Fractional baud rate divisors
             */
            /* set divisor latch */
            this_uart->hw_reg->LCR |= DLAB_MASK;

            /* msb of baud value */
            this_uart->hw_reg->DMR = (uint8_t)(baud_value >> 8);
            /* lsb of baud value */
            this_uart->hw_reg->DLR = (uint8_t)baud_value;

            /* reset divisor latch */
            this_uart->hw_reg->LCR &= ~DLAB_MASK;

            /* Enable Fractional baud rate */
            this_uart->hw_reg->MM0 |= EFBR_MASK;

            /* Load the fractional baud rate register */
            ASSERT(fractional_baud_value <= (uint32_t)UINT8_MAX);
            this_uart->hw_reg->DFR = (uint8_t)fractional_baud_value;
        }
        else
        {
            /*
             * Do NOT use Fractional baud rate divisors.
             */
            /* set divisor latch */
            this_uart->hw_reg->LCR |= DLAB_MASK;

            /* msb of baud value */
            this_uart->hw_reg->DMR = (uint8_t)(baud_value >> 8u);

            /* lsb of baud value */
            this_uart->hw_reg->DLR = (uint8_t)baud_value;

            /* reset divisor latch */
            this_uart->hw_reg->LCR &= ~DLAB_MASK;

            /* Disable Fractional baud rate */
            this_uart->hw_reg->MM0 &= ~EFBR_MASK;
        }
    }
}

#ifdef __cplusplus
}
#endif
