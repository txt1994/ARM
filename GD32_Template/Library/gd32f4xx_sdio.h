/*!
    文件:    gd32f4xx_sdio.h
    简介:   definitions for the SDIO

    版本: 2016-08-15, V1.0.0, firmware for GD32F4xx
    版本: 2018-12-12, V2.0.0, firmware for GD32F4xx
    版本: 2020-09-30, V2.1.0, firmware for GD32F4xx
    版本: 2022-03-09, V3.0.0, firmware for GD32F4xx
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

#ifndef GD32F4XX_SDIO_H
#define GD32F4XX_SDIO_H

#include "gd32f4xx.h"

/* SDIO definitions */
#define SDIO                            SDIO_BASE

/* registers definitions */
#define SDIO_PWRCTL                     REG32(SDIO + 0x00000000U)    /*!< SDIO power control register */
#define SDIO_CLKCTL                     REG32(SDIO + 0x00000004U)    /*!< SDIO clock control register */
#define SDIO_CMDAGMT                    REG32(SDIO + 0x00000008U)    /*!< SDIO command argument register */
#define SDIO_CMDCTL                     REG32(SDIO + 0x0000000CU)    /*!< SDIO command control register */
#define SDIO_RSPCMDIDX                  REG32(SDIO + 0x00000010U)    /*!< SDIO command index response register */
#define SDIO_RESP0                      REG32(SDIO + 0x00000014U)    /*!< SDIO response register 0 */
#define SDIO_RESP1                      REG32(SDIO + 0x00000018U)    /*!< SDIO response register 1 */
#define SDIO_RESP2                      REG32(SDIO + 0x0000001CU)    /*!< SDIO response register 2 */
#define SDIO_RESP3                      REG32(SDIO + 0x00000020U)    /*!< SDIO response register 3 */
#define SDIO_DATATO                     REG32(SDIO + 0x00000024U)    /*!< SDIO data timeout register */
#define SDIO_DATALEN                    REG32(SDIO + 0x00000028U)    /*!< SDIO data length register */
#define SDIO_DATACTL                    REG32(SDIO + 0x0000002CU)    /*!< SDIO data control register */
#define SDIO_DATACNT                    REG32(SDIO + 0x00000030U)    /*!< SDIO data counter register */
#define SDIO_STAT                       REG32(SDIO + 0x00000034U)    /*!< SDIO status register */
#define SDIO_INTC                       REG32(SDIO + 0x00000038U)    /*!< SDIO interrupt clear register */
#define SDIO_INTEN                      REG32(SDIO + 0x0000003CU)    /*!< SDIO interrupt enable register */
#define SDIO_FIFOCNT                    REG32(SDIO + 0x00000048U)    /*!< SDIO FIFO counter register */
#define SDIO_FIFO                       REG32(SDIO + 0x00000080U)    /*!< SDIO FIFO data register */

/* bits definitions */
/* SDIO_PWRCTL */
#define SDIO_PWRCTL_PWRCTL              BITS(0,1)              /*!< SDIO power control bits */

/* SDIO_CLKCTL */
#define SDIO_CLKCTL_DIV                 BITS(0,7)              /*!< clock division */
#define SDIO_CLKCTL_CLKEN               BIT(8)                 /*!< SDIO_CLK clock output enable bit */
#define SDIO_CLKCTL_CLKPWRSAV           BIT(9)                 /*!< SDIO_CLK clock dynamic switch on/off for power saving */
#define SDIO_CLKCTL_CLKBYP              BIT(10)                /*!< clock bypass enable bit */
#define SDIO_CLKCTL_BUSMODE             BITS(11,12)            /*!< SDIO card bus mode control bit */
#define SDIO_CLKCTL_CLKEDGE             BIT(13)                /*!< SDIO_CLK clock edge selection bit */
#define SDIO_CLKCTL_HWCLKEN             BIT(14)                /*!< hardware clock control enable bit */
#define SDIO_CLKCTL_DIV8                BIT(31)                /*!< MSB of clock division */

/* SDIO_CMDAGMT */
#define SDIO_CMDAGMT_CMDAGMT            BITS(0,31)             /*!< SDIO card command argument */

/* SDIO_CMDCTL */
#define SDIO_CMDCTL_CMDIDX              BITS(0,5)              /*!< command index */
#define SDIO_CMDCTL_CMDRESP             BITS(6,7)              /*!< command response type bits */
#define SDIO_CMDCTL_INTWAIT             BIT(8)                 /*!< interrupt wait instead of timeout */
#define SDIO_CMDCTL_WaitDEND            BIT(9)                 /*!< wait for ends of data transfer */
#define SDIO_CMDCTL_CSMEN               BIT(10)                /*!< command state machine(CSM) enable bit */
#define SDIO_CMDCTL_SUSPEND             BIT(11)                /*!< SD I/O suspend command(SD I/O only) */
#define SDIO_CMDCTL_ENCMDC              BIT(12)                /*!< CMD completion signal enabled (CE-ATA only) */
#define SDIO_CMDCTL_NINTEN              BIT(13)                /*!< no CE-ATA interrupt (CE-ATA only) */
#define SDIO_CMDCTL_ATAEN               BIT(14)                /*!< CE-ATA command enable(CE-ATA only) */

/* SDIO_DATATO */
#define SDIO_DATATO_DATATO              BITS(0,31)             /*!< data timeout period */

/* SDIO_DATALEN */
#define SDIO_DATALEN_DATALEN            BITS(0,24)             /*!< data transfer length */

/* SDIO_DATACTL */
#define SDIO_DATACTL_DATAEN             BIT(0)                 /*!< data transfer enabled bit */
#define SDIO_DATACTL_DATADIR            BIT(1)                 /*!< data transfer direction */
#define SDIO_DATACTL_TRANSMOD           BIT(2)                 /*!< data transfer mode */
#define SDIO_DATACTL_DMAEN              BIT(3)                 /*!< DMA enable bit */
#define SDIO_DATACTL_BLKSZ              BITS(4,7)              /*!< data block size */
#define SDIO_DATACTL_RWEN               BIT(8)                 /*!< read wait mode enabled(SD I/O only) */
#define SDIO_DATACTL_RWSTOP             BIT(9)                 /*!< read wait stop(SD I/O only) */
#define SDIO_DATACTL_RWTYPE             BIT(10)                /*!< read wait type(SD I/O only) */
#define SDIO_DATACTL_IOEN               BIT(11)                /*!< SD I/O specific function enable(SD I/O only) */

/* SDIO_STAT */
#define SDIO_STAT_CCRCERR               BIT(0)                 /*!< command response received (CRC check failed) */
#define SDIO_STAT_DTCRCERR              BIT(1)                 /*!< data block sent/received (CRC check failed) */
#define SDIO_STAT_CMDTMOUT              BIT(2)                 /*!< command response timeout */
#define SDIO_STAT_DTTMOUT               BIT(3)                 /*!< data timeout */
#define SDIO_STAT_TXURE                 BIT(4)                 /*!< transmit FIFO underrun error occurs */
#define SDIO_STAT_RXORE                 BIT(5)                 /*!< received FIFO overrun error occurs */
#define SDIO_STAT_CMDRECV               BIT(6)                 /*!< command response received (CRC check passed) */
#define SDIO_STAT_CMDSEND               BIT(7)                 /*!< command sent (no response required) */
#define SDIO_STAT_DTEND                 BIT(8)                 /*!< data end (data counter, SDIO_DATACNT, is zero) */
#define SDIO_STAT_STBITE                BIT(9)                 /*!< start bit error in the bus */
#define SDIO_STAT_DTBLKEND              BIT(10)                /*!< data block sent/received (CRC check passed) */
#define SDIO_STAT_CMDRUN                BIT(11)                /*!< command transmission in progress */
#define SDIO_STAT_TXRUN                 BIT(12)                /*!< data transmission in progress */
#define SDIO_STAT_RXRUN                 BIT(13)                /*!< data reception in progress */
#define SDIO_STAT_TFH                   BIT(14)                /*!< transmit FIFO is half empty: at least 8 words can be written into the FIFO */
#define SDIO_STAT_RFH                   BIT(15)                /*!< receive FIFO is half full: at least 8 words can be read in the FIFO */
#define SDIO_STAT_TFF                   BIT(16)                /*!< transmit FIFO is full */
#define SDIO_STAT_RFF                   BIT(17)                /*!< receive FIFO is full */
#define SDIO_STAT_TFE                   BIT(18)                /*!< transmit FIFO is empty */
#define SDIO_STAT_RFE                   BIT(19)                /*!< receive FIFO is empty */
#define SDIO_STAT_TXDTVAL               BIT(20)                /*!< data is valid in transmit FIFO */
#define SDIO_STAT_RXDTVAL               BIT(21)                /*!< data is valid in receive FIFO */
#define SDIO_STAT_SDIOINT               BIT(22)                /*!< SD I/O interrupt received */
#define SDIO_STAT_ATAEND                BIT(23)                /*!< CE-ATA command completion signal received (only for CMD61) */

/* SDIO_INTC */
#define SDIO_INTC_CCRCERRC              BIT(0)                 /*!< CCRCERR flag clear bit */
#define SDIO_INTC_DTCRCERRC             BIT(1)                 /*!< DTCRCERR flag clear bit */
#define SDIO_INTC_CMDTMOUTC             BIT(2)                 /*!< CMDTMOUT flag clear bit */
#define SDIO_INTC_DTTMOUTC              BIT(3)                 /*!< DTTMOUT flag clear bit */
#define SDIO_INTC_TXUREC                BIT(4)                 /*!< TXURE flag clear bit */
#define SDIO_INTC_RXOREC                BIT(5)                 /*!< RXORE flag clear bit */
#define SDIO_INTC_CMDRECVC              BIT(6)                 /*!< CMDRECV flag clear bit */
#define SDIO_INTC_CMDSENDC              BIT(7)                 /*!< CMDSEND flag clear bit */
#define SDIO_INTC_DTENDC                BIT(8)                 /*!< DTEND flag clear bit */
#define SDIO_INTC_STBITEC               BIT(9)                 /*!< STBITE flag clear bit */
#define SDIO_INTC_DTBLKENDC             BIT(10)                /*!< DTBLKEND flag clear bit */
#define SDIO_INTC_SDIOINTC              BIT(22)                /*!< SDIOINT flag clear bit */
#define SDIO_INTC_ATAENDC               BIT(23)                /*!< ATAEND flag clear bit */

/* SDIO_INTEN */
#define SDIO_INTEN_CCRCERRIE            BIT(0)                 /*!< command response CRC fail interrupt enable */
#define SDIO_INTEN_DTCRCERRIE           BIT(1)                 /*!< data CRC fail interrupt enable */
#define SDIO_INTEN_CMDTMOUTIE           BIT(2)                 /*!< command response timeout interrupt enable */
#define SDIO_INTEN_DTTMOUTIE            BIT(3)                 /*!< data timeout interrupt enable */
#define SDIO_INTEN_TXUREIE              BIT(4)                 /*!< transmit FIFO underrun error interrupt enable */
#define SDIO_INTEN_RXOREIE              BIT(5)                 /*!< received FIFO overrun error interrupt enable */
#define SDIO_INTEN_CMDRECVIE            BIT(6)                 /*!< command response received interrupt enable */
#define SDIO_INTEN_CMDSENDIE            BIT(7)                 /*!< command sent interrupt enable */
#define SDIO_INTEN_DTENDIE              BIT(8)                 /*!< data end interrupt enable */
#define SDIO_INTEN_STBITEIE             BIT(9)                 /*!< start bit error interrupt enable */
#define SDIO_INTEN_DTBLKENDIE           BIT(10)                /*!< data block end interrupt enable */
#define SDIO_INTEN_CMDRUNIE             BIT(11)                /*!< command transmission interrupt enable */
#define SDIO_INTEN_TXRUNIE              BIT(12)                /*!< data transmission interrupt enable */
#define SDIO_INTEN_RXRUNIE              BIT(13)                /*!< data reception interrupt enable */
#define SDIO_INTEN_TFHIE                BIT(14)                /*!< transmit FIFO half empty interrupt enable */
#define SDIO_INTEN_RFHIE                BIT(15)                /*!< receive FIFO half full interrupt enable */
#define SDIO_INTEN_TFFIE                BIT(16)                /*!< transmit FIFO full interrupt enable */
#define SDIO_INTEN_RFFIE                BIT(17)                /*!< receive FIFO full interrupt enable */
#define SDIO_INTEN_TFEIE                BIT(18)                /*!< transmit FIFO empty interrupt enable */
#define SDIO_INTEN_RFEIE                BIT(19)                /*!< receive FIFO empty interrupt enable */
#define SDIO_INTEN_TXDTVALIE            BIT(20)                /*!< data valid in transmit FIFO interrupt enable */
#define SDIO_INTEN_RXDTVALIE            BIT(21)                /*!< data valid in receive FIFO interrupt enable */
#define SDIO_INTEN_SDIOINTIE            BIT(22)                /*!< SD I/O interrupt received interrupt enable */
#define SDIO_INTEN_ATAENDIE             BIT(23)                /*!< CE-ATA command completion signal received interrupt enable */

/* SDIO_FIFO */
#define SDIO_FIFO_FIFODT                BITS(0,31)             /*!< receive FIFO data or transmit FIFO data */

/* constants definitions */
/* SDIO flags */
#define SDIO_Flag_CCRCERR               BIT(0)                 /*!< command response received (CRC check failed) flag */
#define SDIO_Flag_DTCRCERR              BIT(1)                 /*!< data block sent/received (CRC check failed) flag */
#define SDIO_Flag_CMDTMOUT              BIT(2)                 /*!< command response timeout flag */
#define SDIO_Flag_DTTMOUT               BIT(3)                 /*!< data timeout flag */
#define SDIO_Flag_TXURE                 BIT(4)                 /*!< transmit FIFO underrun error occurs flag */
#define SDIO_Flag_RXORE                 BIT(5)                 /*!< received FIFO overrun error occurs flag */
#define SDIO_Flag_CMDRECV               BIT(6)                 /*!< command response received (CRC check passed) flag */
#define SDIO_Flag_CMDSEND               BIT(7)                 /*!< command sent (no response required) flag */
#define SDIO_Flag_DTEND                 BIT(8)                 /*!< data end (data counter, SDIO_DATACNT, is zero) flag */
#define SDIO_Flag_STBITE                BIT(9)                 /*!< start bit error in the bus flag */
#define SDIO_Flag_DTBLKEND              BIT(10)                /*!< data block sent/received (CRC check passed) flag */
#define SDIO_Flag_CMDRUN                BIT(11)                /*!< command transmission in progress flag */
#define SDIO_Flag_TXRUN                 BIT(12)                /*!< data transmission in progress flag */
#define SDIO_Flag_RXRUN                 BIT(13)                /*!< data reception in progress flag */
#define SDIO_Flag_TFH                   BIT(14)                /*!< transmit FIFO is half empty flag: at least 8 words can be written into the FIFO */
#define SDIO_Flag_RFH                   BIT(15)                /*!< receive FIFO is half full flag: at least 8 words can be read in the FIFO */
#define SDIO_Flag_TFF                   BIT(16)                /*!< transmit FIFO is full flag */
#define SDIO_Flag_RFF                   BIT(17)                /*!< receive FIFO is full flag */
#define SDIO_Flag_TFE                   BIT(18)                /*!< transmit FIFO is empty flag */
#define SDIO_Flag_RFE                   BIT(19)                /*!< receive FIFO is empty flag */
#define SDIO_Flag_TXDTVAL               BIT(20)                /*!< data is valid in transmit FIFO flag */
#define SDIO_Flag_RXDTVAL               BIT(21)                /*!< data is valid in receive FIFO flag */
#define SDIO_Flag_SDIOINT               BIT(22)                /*!< SD I/O interrupt received flag */
#define SDIO_Flag_ATAEND                BIT(23)                /*!< CE-ATA command completion signal received (only for CMD61) flag */

/* SDIO interrupt enable or disable */
#define SDIO_INT_CCRCERR                BIT(0)                 /*!< SDIO CCRCERR interrupt */
#define SDIO_INT_DTCRCERR               BIT(1)                 /*!< SDIO DTCRCERR interrupt */
#define SDIO_INT_CMDTMOUT               BIT(2)                 /*!< SDIO CMDTMOUT interrupt */
#define SDIO_INT_DTTMOUT                BIT(3)                 /*!< SDIO DTTMOUT interrupt */
#define SDIO_INT_TXURE                  BIT(4)                 /*!< SDIO TXURE interrupt */
#define SDIO_INT_RXORE                  BIT(5)                 /*!< SDIO RXORE interrupt */
#define SDIO_INT_CMDRECV                BIT(6)                 /*!< SDIO CMDRECV interrupt */
#define SDIO_INT_CMDSEND                BIT(7)                 /*!< SDIO CMDSEND interrupt */
#define SDIO_INT_DTEND                  BIT(8)                 /*!< SDIO DTEND interrupt */
#define SDIO_INT_STBITE                 BIT(9)                 /*!< SDIO STBITE interrupt */
#define SDIO_INT_DTBLKEND               BIT(10)                /*!< SDIO DTBLKEND interrupt */
#define SDIO_INT_CMDRUN                 BIT(11)                /*!< SDIO CMDRUN interrupt */
#define SDIO_INT_TXRUN                  BIT(12)                /*!< SDIO TXRUN interrupt */
#define SDIO_INT_RXRUN                  BIT(13)                /*!< SDIO RXRUN interrupt */
#define SDIO_INT_TFH                    BIT(14)                /*!< SDIO TFH interrupt */
#define SDIO_INT_RFH                    BIT(15)                /*!< SDIO RFH interrupt */
#define SDIO_INT_TFF                    BIT(16)                /*!< SDIO TFF interrupt */
#define SDIO_INT_RFF                    BIT(17)                /*!< SDIO RFF interrupt */
#define SDIO_INT_TFE                    BIT(18)                /*!< SDIO TFE interrupt */
#define SDIO_INT_RFE                    BIT(19)                /*!< SDIO RFE interrupt */
#define SDIO_INT_TXDTVAL                BIT(20)                /*!< SDIO TXDTVAL interrupt */
#define SDIO_INT_RXDTVAL                BIT(21)                /*!< SDIO RXDTVAL interrupt */
#define SDIO_INT_SDIOINT                BIT(22)                /*!< SDIO SDIOINT interrupt */
#define SDIO_INT_ATAEND                 BIT(23)                /*!< SDIO ATAEND interrupt */

/* SDIO interrupt flags */
#define SDIO_INT_Flag_CCRCERR           BIT(0)                 /*!< SDIO CCRCERR interrupt flag */
#define SDIO_INT_Flag_DTCRCERR          BIT(1)                 /*!< SDIO DTCRCERR interrupt flag */
#define SDIO_INT_Flag_CMDTMOUT          BIT(2)                 /*!< SDIO CMDTMOUT interrupt flag */
#define SDIO_INT_Flag_DTTMOUT           BIT(3)                 /*!< SDIO DTTMOUT interrupt flag */
#define SDIO_INT_Flag_TXURE             BIT(4)                 /*!< SDIO TXURE interrupt flag */
#define SDIO_INT_Flag_RXORE             BIT(5)                 /*!< SDIO RXORE interrupt flag */
#define SDIO_INT_Flag_CMDRECV           BIT(6)                 /*!< SDIO CMDRECV interrupt flag */
#define SDIO_INT_Flag_CMDSEND           BIT(7)                 /*!< SDIO CMDSEND interrupt flag */
#define SDIO_INT_Flag_DTEND             BIT(8)                 /*!< SDIO DTEND interrupt flag */
#define SDIO_INT_Flag_STBITE            BIT(9)                 /*!< SDIO STBITE interrupt flag */
#define SDIO_INT_Flag_DTBLKEND          BIT(10)                /*!< SDIO DTBLKEND interrupt flag */
#define SDIO_INT_Flag_CMDRUN            BIT(11)                /*!< SDIO CMDRUN interrupt flag */
#define SDIO_INT_Flag_TXRUN             BIT(12)                /*!< SDIO TXRUN interrupt flag */
#define SDIO_INT_Flag_RXRUN             BIT(13)                /*!< SDIO RXRUN interrupt flag */
#define SDIO_INT_Flag_TFH               BIT(14)                /*!< SDIO TFH interrupt flag */
#define SDIO_INT_Flag_RFH               BIT(15)                /*!< SDIO RFH interrupt flag */
#define SDIO_INT_Flag_TFF               BIT(16)                /*!< SDIO TFF interrupt flag */
#define SDIO_INT_Flag_RFF               BIT(17)                /*!< SDIO RFF interrupt flag */
#define SDIO_INT_Flag_TFE               BIT(18)                /*!< SDIO TFE interrupt flag */
#define SDIO_INT_Flag_RFE               BIT(19)                /*!< SDIO RFE interrupt flag */
#define SDIO_INT_Flag_TXDTVAL           BIT(20)                /*!< SDIO TXDTVAL interrupt flag */
#define SDIO_INT_Flag_RXDTVAL           BIT(21)                /*!< SDIO RXDTVAL interrupt flag */
#define SDIO_INT_Flag_SDIOINT           BIT(22)                /*!< SDIO SDIOINT interrupt flag */
#define SDIO_INT_Flag_ATAEND            BIT(23)                /*!< SDIO ATAEND interrupt flag */

/* SDIO power control */
#define PWRCTL_PWRCTL(regval)           (BITS(0,1) & ((uint32_t)(regval) << 0))
#define SDIO_Power_OFF                  PWRCTL_PWRCTL(0)       /*!< SDIO power off */
#define SDIO_Power_ON                   PWRCTL_PWRCTL(3)       /*!< SDIO power on */

/* SDIO card bus mode control */
#define CLKCTL_BUSMODE(regval)          (BITS(11,12) & ((uint32_t)(regval) << 11))
#define SDIO_BUSMODE_1BIT               CLKCTL_BUSMODE(0)      /*!< 1-bit SDIO card bus mode */
#define SDIO_BUSMODE_4BIT               CLKCTL_BUSMODE(1)      /*!< 4-bit SDIO card bus mode */
#define SDIO_BUSMODE_8BIT               CLKCTL_BUSMODE(2)      /*!< 8-bit SDIO card bus mode */

/* SDIO_CLK clock edge selection */
#define SDIO_SDIOCLKEDGE_RISING         (uint32_t)0x00000000U  /*!< select the rising edge of the SDIOCLK to generate SDIO_CLK */
#define SDIO_SDIOCLKEDGE_FALLING        SDIO_CLKCTL_CLKEDGE    /*!< select the falling edge of the SDIOCLK to generate SDIO_CLK */

/* clock bypass enable or disable */
#define SDIO_CLOCKBYPASS_DISABLE        (uint32_t)0x00000000U  /*!< no bypass */
#define SDIO_CLOCKBYPASS_ENABLE         SDIO_CLKCTL_CLKBYP     /*!< clock bypass */

/* SDIO_CLK clock dynamic switch on/off for power saving */
#define SDIO_CLOCKPWRSAVE_DISABLE       (uint32_t)0x00000000U  /*!< SDIO_CLK clock is always on */
#define SDIO_CLOCKPWRSAVE_ENABLE        SDIO_CLKCTL_CLKPWRSAV  /*!< SDIO_CLK closed when bus is idle */

/* SDIO command response type */
#define CMDCTL_CMDRESP(regval)          (BITS(6,7) & ((uint32_t)(regval) << 6))
#define SDIO_RESPONSETYPE_NO            CMDCTL_CMDRESP(0)      /*!< no response */
#define SDIO_RESPONSETYPE_SHORT         CMDCTL_CMDRESP(1)      /*!< short response */
#define SDIO_RESPONSETYPE_LONG          CMDCTL_CMDRESP(3)      /*!< long response */

/* command state machine wait type */
#define SDIO_WaitTYPE_NO                (uint32_t)0x00000000U  /*!< not wait interrupt */
#define SDIO_WaitTYPE_INTERRUPT         SDIO_CMDCTL_INTWAIT    /*!< wait interrupt */
#define SDIO_WaitTYPE_DATAEND           SDIO_CMDCTL_WaitDEND   /*!< wait the end of data transfer */

#define SDIO_RESPONSE0                  (uint32_t)0x00000000U  /*!< card response[31:0]/card response[127:96] */
#define SDIO_RESPONSE1                  (uint32_t)0x00000001U  /*!< card response[95:64] */
#define SDIO_RESPONSE2                  (uint32_t)0x00000002U  /*!< card response[63:32] */
#define SDIO_RESPONSE3                  (uint32_t)0x00000003U  /*!< card response[31:1], plus bit 0 */

/* SDIO data block size */
#define DATACTL_BLKSZ(regval)           (BITS(4,7) & ((uint32_t)(regval) << 4))
#define SDIO_DATABLOCKSIZE_1BYTE        DATACTL_BLKSZ(0)       /*!< block size = 1 byte */
#define SDIO_DATABLOCKSIZE_2BYTES       DATACTL_BLKSZ(1)       /*!< block size = 2 bytes */
#define SDIO_DATABLOCKSIZE_4BYTES       DATACTL_BLKSZ(2)       /*!< block size = 4 bytes */
#define SDIO_DATABLOCKSIZE_8BYTES       DATACTL_BLKSZ(3)       /*!< block size = 8 bytes */
#define SDIO_DATABLOCKSIZE_16BYTES      DATACTL_BLKSZ(4)       /*!< block size = 16 bytes */
#define SDIO_DATABLOCKSIZE_32BYTES      DATACTL_BLKSZ(5)       /*!< block size = 32 bytes */
#define SDIO_DATABLOCKSIZE_64BYTES      DATACTL_BLKSZ(6)       /*!< block size = 64 bytes */
#define SDIO_DATABLOCKSIZE_128BYTES     DATACTL_BLKSZ(7)       /*!< block size = 128 bytes */
#define SDIO_DATABLOCKSIZE_256BYTES     DATACTL_BLKSZ(8)       /*!< block size = 256 bytes */
#define SDIO_DATABLOCKSIZE_512BYTES     DATACTL_BLKSZ(9)       /*!< block size = 512 bytes */
#define SDIO_DATABLOCKSIZE_1024BYTES    DATACTL_BLKSZ(10)      /*!< block size = 1024 bytes */
#define SDIO_DATABLOCKSIZE_2048BYTES    DATACTL_BLKSZ(11)      /*!< block size = 2048 bytes */
#define SDIO_DATABLOCKSIZE_4096BYTES    DATACTL_BLKSZ(12)      /*!< block size = 4096 bytes */
#define SDIO_DATABLOCKSIZE_8192BYTES    DATACTL_BLKSZ(13)      /*!< block size = 8192 bytes */
#define SDIO_DATABLOCKSIZE_16384BYTES   DATACTL_BLKSZ(14)      /*!< block size = 16384 bytes */

/* SDIO data transfer mode */
#define SDIO_TRANSMODE_BLOCK            (uint32_t)0x00000000U  /*!< block transfer */
#define SDIO_TRANSMODE_STREAM           SDIO_DATACTL_TRANSMOD  /*!< stream transfer or SDIO multibyte transfer */

/* SDIO data transfer direction */
#define SDIO_TRANSDIRECTION_TOCARD      (uint32_t)0x00000000U  /*!< write data to card */
#define SDIO_TRANSDIRECTION_TOSDIO      SDIO_DATACTL_DATADIR   /*!< read data from card */

/* SDIO read wait type */
#define SDIO_READWAITTYPE_DAT2          (uint32_t)0x00000000U  /*!< read wait control using SDIO_DAT[2] */
#define SDIO_READWAITTYPE_CLK           SDIO_DATACTL_RWTYPE    /*!< read wait control by stopping SDIO_CLK */

/* function declarations */
/* de/initialization functions, hardware clock, bus mode, power_state and SDIO clock configuration */
/* deinitialize the SDIO */
void SDIO_DeInit(void);
/* configure the SDIO clock */
void SDIO_Clock_Config(uint32_t clock_edge, uint32_t clock_bypass, uint32_t clock_powersave, uint16_t clock_division);
/* enable hardware clock control */
void SDIO_Hardware_Clock_Enable(void);
/* disable hardware clock control */
void SDIO_Hardware_Clock_Disable(void);
/* set different SDIO card bus mode */
void SDIO_bus_Mode_Set(uint32_t bus_mode);
/* set the SDIO power state */
void SDIO_Power_State_Set(uint32_t power_state);
/* get the SDIO power state */
uint32_t SDIO_Power_State_Get(void);
/* enable SDIO_CLK clock output */
void SDIO_Clock_Enable(void);
/* disable SDIO_CLK clock output */
void SDIO_Clock_Disable(void);

/* configure the command index, argument, response type, wait type and CSM to send command functions */
/* configure the command and response */
void SDIO_command_response_Config(uint32_t cmd_index, uint32_t cmd_argument, uint32_t response_type);
/* set the command state machine wait type */
void SDIO_Wait_type_Set(uint32_t wait_type);
/* enable the CSM(command state machine) */
void SDIO_csm_Enable(void);
/* disable the CSM(command state machine) */
void SDIO_csm_Disable(void);
/* get the last response command index */
uint8_t SDIO_command_index_Get(void);
/* get the response for the last received command */
uint32_t SDIO_response_Get(uint32_t SDIO_responsex);

/* configure the data timeout, length, block size, transfer mode, direction and DSM for data transfer functions */
/* configure the data timeout, data length and data block size */
void SDIO_Data_Config(uint32_t data_timeout, uint32_t data_length, uint32_t data_blocksize);
/* configure the data transfer mode and direction */
void SDIO_Data_Transfer_Config(uint32_t transfer_mode, uint32_t transfer_direction);
/* enable the DSM(data state machine) for data transfer */
void SDIO_dsm_Enable(void);
/* disable the DSM(data state machine) */
void SDIO_dsm_Disable(void);
/* write data(one word) to the transmit FIFO */
void SDIO_Data_Write(uint32_t data);
/* read data(one word) from the receive FIFO */
uint32_t SDIO_Data_Read(void);
/* get the number of remaining data bytes to be transferred to card */
uint32_t SDIO_Data_Counter_Get(void);
/* get the number of words remaining to be written or read from FIFO */
uint32_t SDIO_FIFO_Counter_Get(void);
/* enable the DMA request for SDIO */
void SDIO_DMA_Enable(void);
/* disable the DMA request for SDIO */
void SDIO_DMA_Disable(void);

/* flag and interrupt functions */
/* get the flags state of SDIO */
FlagStatus SDIO_Flag_Get(uint32_t flag);
/* clear the pending flags of SDIO */
void SDIO_Flag_Clear(uint32_t flag);
/* enable the SDIO interrupt */
void SDIO_Interrupt_Enable(uint32_t int_flag);
/* disable the SDIO interrupt */
void SDIO_Interrupt_Disable(uint32_t int_flag);
/* get the interrupt flags state of SDIO */
FlagStatus SDIO_Interrupt_Flag_Get(uint32_t int_flag);
/* clear the interrupt pending flags of SDIO */
void SDIO_Interrupt_Flag_Clear(uint32_t int_flag);

/* SD I/O card functions */
/* enable the read wait mode(SD I/O only) */
void SDIO_readwait_Enable(void);
/* disable the read wait mode(SD I/O only) */
void SDIO_readwait_Disable(void);
/* enable the function that stop the read wait process(SD I/O only) */
void SDIO_Stop_readwait_Enable(void);
/* disable the function that stop the read wait process(SD I/O only) */
void SDIO_Stop_readwait_Disable(void);
/* set the read wait type(SD I/O only) */
void SDIO_readwait_type_Set(uint32_t readwait_type);
/* enable the SD I/O mode specific operation(SD I/O only) */
void SDIO_operation_Enable(void);
/* disable the SD I/O mode specific operation(SD I/O only) */
void SDIO_operation_Disable(void);
/* enable the SD I/O suspend operation(SD I/O only) */
void SDIO_suspend_Enable(void);
/* disable the SD I/O suspend operation(SD I/O only) */
void SDIO_suspend_Disable(void);

/* CE-ATA functions */
/* enable the CE-ATA command(CE-ATA only) */
void SDIO_Ceata_command_Enable(void);
/* disable the CE-ATA command(CE-ATA only) */
void SDIO_Ceata_command_Disable(void);
/* enable the CE-ATA interrupt(CE-ATA only) */
void SDIO_Ceata_Interrupt_Enable(void);
/* disable the CE-ATA interrupt(CE-ATA only) */
void SDIO_Ceata_Interrupt_Disable(void);
/* enable the CE-ATA command completion signal(CE-ATA only) */
void SDIO_Ceata_command_completion_Enable(void);
/* disable the CE-ATA command completion signal(CE-ATA only) */
void SDIO_Ceata_command_completion_Disable(void);

#endif /* GD32F4XX_SDIO_H */
