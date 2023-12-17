/*!
    文件:    gd32f4xx_fmc.h
    简介:   definitions for the FMC

    版本: 2016-08-15, V1.0.0, firmware for GD32F4xx
    版本: 2018-12-12, V2.0.0, firmware for GD32F4xx
    版本: 2020-09-30, V2.1.0, firmware for GD32F4xx
    版本: 2020-12-20, V2.1.1, firmware for GD32F4xx
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


#ifndef GD32F4XX_FMC_H
#define GD32F4XX_FMC_H

#include "gd32f4xx.h"

/* FMC and option byte definition */
#define FMC                        FMC_BASE                       /*!< FMC register base address */
#define OB                         OB_BASE                        /*!< option byte base address */

/* registers definitions */
#define FMC_WS                     REG32((FMC) + 0x00000000U)     /*!< FMC wait state register */
#define FMC_KEY                    REG32((FMC) + 0x00000004U)     /*!< FMC unlock key register */
#define FMC_OBKEY                  REG32((FMC) + 0x00000008U)     /*!< FMC option byte unlock key register */
#define FMC_STAT                   REG32((FMC) + 0x0000000CU)     /*!< FMC status register */
#define FMC_CTL                    REG32((FMC) + 0x00000010U)     /*!< FMC control register */
#define FMC_OBCTL0                 REG32((FMC) + 0x00000014U)     /*!< FMC option byte control register 0 */
#define FMC_OBCTL1                 REG32((FMC) + 0x00000018U)     /*!< FMC option byte control register 1 */
#define FMC_PECFG                  REG32((FMC) + 0x00000020U)     /*!< FMC page erase configuration register */
#define FMC_PEKEY                  REG32((FMC) + 0x00000024U)     /*!< FMC unlock page erase key register */
#define FMC_WSEN                   REG32((FMC) + 0x000000FCU)     /*!< FMC wait state enable register */
#define FMC_PID                    REG32((FMC) + 0x00000100U)     /*!< FMC product ID register */

#define OB_WP1                     REG32((OB) + 0x00000008U)      /*!< option byte write protection 1 */
#define OB_USER                    REG32((OB) + 0x00010000U)      /*!< option byte user value*/
#define OB_SPC                     REG32((OB) + 0x00010001U)      /*!< option byte security protection value */
#define OB_WP0                     REG32((OB) + 0x00010008U)      /*!< option byte write protection 0 */

/* bits definitions */
/* FMC_WS */
#define FMC_WC_WSCNT               BITS(0,3)                      /*!< wait state counter */

/* FMC_KEY */
#define FMC_Key_KEY                BITS(0,31)                     /*!< FMC main flash key bits */

/* FMC_OBKEY */
#define FMC_OBKEY_OBKEY            BITS(0,31)                     /*!< option byte key bits */

/* FMC_STAT */
#define FMC_STAT_END               BIT(0)                         /*!< end of operation flag bit */
#define FMC_STAT_OPERR             BIT(1)                         /*!< flash operation error flag bit */
#define FMC_STAT_WPERR             BIT(4)                         /*!< erase/Program protection error flag bit */
#define FMC_STAT_PGMERR            BIT(6)                         /*!< program size not match error flag bit */
#define FMC_STAT_PGSERR            BIT(7)                         /*!< program sequence error flag bit */
#define FMC_STAT_RDDERR            BIT(8)                         /*!< read D-bus protection error flag bit */
#define FMC_STAT_BUSY              BIT(16)                        /*!< flash busy flag bit */

/* FMC_CTL */
#define FMC_CTL_PG                 BIT(0)                         /*!< main flash program command bit */
#define FMC_CTL_SER                BIT(1)                         /*!< main flash sector erase command bit */
#define FMC_CTL_MER0               BIT(2)                         /*!< main flash mass erase for bank0 command bit */
#define FMC_CTL_SN                 BITS(3,7)                      /*!< select which sector number to be erased */
#define FMC_CTL_PSZ                BITS(8,9)                      /*!< program size bit */
#define FMC_CTL_MER1               BIT(15)                        /*!< main flash mass erase for bank1 command bit */
#define FMC_CTL_START              BIT(16)                        /*!< send erase command to FMC bit */
#define FMC_CTL_ENDIE              BIT(24)                        /*!< end of operation interrupt enable bit */
#define FMC_CTL_ERRIE              BIT(25)                        /*!< error interrupt enable bit */
#define FMC_CTL_LK                 BIT(31)                        /*!< FMC_CTL lock bit */

/* FMC_OBCTL0 */
#define FMC_OBCTL0_OB_LK           BIT(0)                         /*!< FMC_OBCTL0 lock bit */
#define FMC_OBCTL0_OB_START        BIT(1)                         /*!< send option byte change command to FMC bit */
#define FMC_OBCTL0_Bor_TH          BITS(2,3)                      /*!< option byte BOR threshold value */
#define FMC_OBCTL0_BB              BIT(4)                         /*!< option byte boot bank value */
#define FMC_OBCTL0_NWDG_HW         BIT(5)                         /*!< option byte watchdog value */
#define FMC_OBCTL0_NRST_DPSLP      BIT(6)                         /*!< option byte deepsleep reset value */
#define FMC_OBCTL0_NRST_STDBY      BIT(7)                         /*!< option byte standby reset value */
#define FMC_OBCTL0_SPC             BITS(8,15)                     /*!< option byte Security Protection code */
#define FMC_OBCTL0_WP0             BITS(16,27)                    /*!< erase/program protection of each sector when DRP is 0 */
#define FMC_OBCTL0_DBS             BIT(30)                        /*!< double banks or single bank selection when flash size is 1M bytes */
#define FMC_OBCTL0_Drp             BIT(31)                        /*!< D-bus read protection bit */

/* FMC_OBCTL1 */
#define FMC_OBCTL1_WP1             BITS(16,27)                    /*!< erase/program protection of each sector when DRP is 0 */

/* FMC_PECFG */
#define FMC_PE_EN                  BIT(31)                        /*!< the enable bit of page erase function */
#define FMC_PE_ADDR                BITS(0,28)                     /*!< page erase address */

/* FMC_PEKEY */
#define FMC_PE_KEY                 BITS(0,31)                     /*!< FMC_PECFG unlock key value */

/* FMC_WSEN */
#define FMC_WSEN_WSEN              BIT(0)                         /*!< FMC wait state enable bit */

/* FMC_PID */
#define FMC_PID_PID                BITS(0,31)                     /*!< product ID bits */

/* constants definitions */
/* fmc state */
typedef enum {
    FMC_READY = 0,                                                /*!< the operation has been completed */
    FMC_BUSY,                                                     /*!< the operation is in progress */
    FMC_RDDERR,                                                   /*!< read D-bus protection error */
    FMC_PGSERR,                                                   /*!< program sequence error */
    FMC_PGMERR,                                                   /*!< program size not match error */
    FMC_WPERR,                                                    /*!< erase/program protection error */
    FMC_OPERR,                                                    /*!< operation error */
    FMC_TOERR                                                     /*!< timeout error */
} FMC_State_enum;

/* unlock key */
#define UNLOCK_KEY0                ((uint32_t)0x45670123U)        /*!< unlock key 0 */
#define UNLOCK_KEY1                ((uint32_t)0xCDEF89ABU)        /*!< unlock key 1 */
#define UNLOCK_PE_KEY              ((uint32_t)0xA9B8C7D6U)        /*!< unlock page erase function key */

#define OB_UNLOCK_KEY0             ((uint32_t)0x08192A3BU)        /*!< ob unlock key 0 */
#define OB_UNLOCK_KEY1             ((uint32_t)0x4C5D6E7FU)        /*!< ob unlock key 1 */

/* option byte write protection */
#define OB_LWP                     ((uint32_t)0x000000FFU)        /*!< write protection low bits */
#define OB_HWP                     ((uint32_t)0x0000FF00U)        /*!< write protection high bits */

/* FMC wait state counter */
#define WC_WSCNT(regval)           (BITS(0,3) & ((uint32_t)(regval)))
#define WS_WSCNT_0                 WC_WSCNT(0)                    /*!< FMC 0 wait */
#define WS_WSCNT_1                 WC_WSCNT(1)                    /*!< FMC 1 wait */
#define WS_WSCNT_2                 WC_WSCNT(2)                    /*!< FMC 2 wait */
#define WS_WSCNT_3                 WC_WSCNT(3)                    /*!< FMC 3 wait */
#define WS_WSCNT_4                 WC_WSCNT(4)                    /*!< FMC 4 wait */
#define WS_WSCNT_5                 WC_WSCNT(5)                    /*!< FMC 5 wait */
#define WS_WSCNT_6                 WC_WSCNT(6)                    /*!< FMC 6 wait */
#define WS_WSCNT_7                 WC_WSCNT(7)                    /*!< FMC 7 wait */
#define WS_WSCNT_8                 WC_WSCNT(8)                    /*!< FMC 8 wait */
#define WS_WSCNT_9                 WC_WSCNT(9)                    /*!< FMC 9 wait */
#define WS_WSCNT_10                WC_WSCNT(10)                   /*!< FMC 10 wait */
#define WS_WSCNT_11                WC_WSCNT(11)                   /*!< FMC 11 wait */
#define WS_WSCNT_12                WC_WSCNT(12)                   /*!< FMC 12 wait */
#define WS_WSCNT_13                WC_WSCNT(13)                   /*!< FMC 13 wait */
#define WS_WSCNT_14                WC_WSCNT(14)                   /*!< FMC 14 wait */
#define WS_WSCNT_15                WC_WSCNT(15)                   /*!< FMC 15 wait */

/* option byte BOR threshold value */
#define OBCTL0_Bor_TH(regval)      (BITS(2,3) & ((uint32_t)(regval))<< 2)
#define OB_Bor_TH_VALUE3           OBCTL0_Bor_TH(0)               /*!< BOR threshold value 3 */
#define OB_Bor_TH_VALUE2           OBCTL0_Bor_TH(1)               /*!< BOR threshold value 2 */
#define OB_Bor_TH_VALUE1           OBCTL0_Bor_TH(2)               /*!< BOR threshold value 1 */
#define OB_Bor_TH_OFF              OBCTL0_Bor_TH(3)               /*!< no BOR function */

/* option byte boot bank value */
#define OBCTL0_BB(regval)          (BIT(4) & ((uint32_t)(regval)<<4))
#define OB_BB_DISABLE              OBCTL0_BB(0)                   /*!< boot from bank0 */
#define OB_BB_ENABLE               OBCTL0_BB(1)                   /*!< boot from bank1 or bank0 if bank1 is void */

/* option byte software/hardware free watch dog timer */
#define OBCTL0_NWDG_HW(regval)     (BIT(5) & ((uint32_t)(regval))<< 5)
#define OB_FWDGT_SW                OBCTL0_NWDG_HW(1)              /*!< software free watchdog */
#define OB_FWDGT_HW                OBCTL0_NWDG_HW(0)              /*!< hardware free watchdog */

/* option byte reset or not entering deep sleep mode */
#define OBCTL0_NRST_DPSLP(regval)  (BIT(6) & ((uint32_t)(regval))<< 6)
#define OB_DEEPSLEEP_NRST          OBCTL0_NRST_DPSLP(1)           /*!< no reset when entering deepsleep mode */
#define OB_DEEPSLEEP_RST           OBCTL0_NRST_DPSLP(0)           /*!< generate a reset instead of entering deepsleep mode */

/* option byte reset or not entering standby mode */
#define OBCTL0_NRST_STDBY(regval)  (BIT(7) & ((uint32_t)(regval))<< 7)
#define OB_STDBY_NRST              OBCTL0_NRST_STDBY(1)           /*!< no reset when entering deepsleep mode */
#define OB_STDBY_RST               OBCTL0_NRST_STDBY(0)           /*!< generate a reset instead of entering standby mode */

/* read protect configure */
#define FMC_NSPC                   ((uint8_t)0xAAU)               /*!< no security protection */
#define FMC_LSPC                   ((uint8_t)0xABU)               /*!< low security protection */
#define FMC_HSPC                   ((uint8_t)0xCCU)               /*!< high security protection */

/* option bytes write protection */
#define OB_WP_0                    ((uint32_t)0x00000001U)        /*!< erase/program protection of sector 0  */
#define OB_WP_1                    ((uint32_t)0x00000002U)        /*!< erase/program protection of sector 1  */
#define OB_WP_2                    ((uint32_t)0x00000004U)        /*!< erase/program protection of sector 2  */
#define OB_WP_3                    ((uint32_t)0x00000008U)        /*!< erase/program protection of sector 3  */
#define OB_WP_4                    ((uint32_t)0x00000010U)        /*!< erase/program protection of sector 4  */
#define OB_WP_5                    ((uint32_t)0x00000020U)        /*!< erase/program protection of sector 5  */
#define OB_WP_6                    ((uint32_t)0x00000040U)        /*!< erase/program protection of sector 6  */
#define OB_WP_7                    ((uint32_t)0x00000080U)        /*!< erase/program protection of sector 7  */
#define OB_WP_8                    ((uint32_t)0x00000100U)        /*!< erase/program protection of sector 8  */
#define OB_WP_9                    ((uint32_t)0x00000200U)        /*!< erase/program protection of sector 9  */
#define OB_WP_10                   ((uint32_t)0x00000400U)        /*!< erase/program protection of sector 10 */
#define OB_WP_11                   ((uint32_t)0x00000800U)        /*!< erase/program protection of sector 11 */
#define OB_WP_12                   ((uint32_t)0x00010000U)        /*!< erase/program protection of sector 12 */
#define OB_WP_13                   ((uint32_t)0x00020000U)        /*!< erase/program protection of sector 13 */
#define OB_WP_14                   ((uint32_t)0x00040000U)        /*!< erase/program protection of sector 14 */
#define OB_WP_15                   ((uint32_t)0x00080000U)        /*!< erase/program protection of sector 15 */
#define OB_WP_16                   ((uint32_t)0x00100000U)        /*!< erase/program protection of sector 16 */
#define OB_WP_17                   ((uint32_t)0x00200000U)        /*!< erase/program protection of sector 17 */
#define OB_WP_18                   ((uint32_t)0x00400000U)        /*!< erase/program protection of sector 18 */
#define OB_WP_19                   ((uint32_t)0x00800000U)        /*!< erase/program protection of sector 19 */
#define OB_WP_20                   ((uint32_t)0x01000000U)        /*!< erase/program protection of sector 20 */
#define OB_WP_21                   ((uint32_t)0x02000000U)        /*!< erase/program protection of sector 21 */
#define OB_WP_22                   ((uint32_t)0x04000000U)        /*!< erase/program protection of sector 22 */
#define OB_WP_23_27                ((uint32_t)0x08000000U)        /*!< erase/program protection of sector 23~27 */
#define OB_WP_ALL                  ((uint32_t)0x0FFF0FFFU)        /*!< erase/program protection of all sectors */

/* option bytes D-bus read protection */
#define OB_Drp_0                   ((uint32_t)0x00000001U)        /*!< D-bus read protection protection of sector 0  */
#define OB_Drp_1                   ((uint32_t)0x00000002U)        /*!< D-bus read protection protection of sector 1  */
#define OB_Drp_2                   ((uint32_t)0x00000004U)        /*!< D-bus read protection protection of sector 2  */
#define OB_Drp_3                   ((uint32_t)0x00000008U)        /*!< D-bus read protection protection of sector 3  */
#define OB_Drp_4                   ((uint32_t)0x00000010U)        /*!< D-bus read protection protection of sector 4  */
#define OB_Drp_5                   ((uint32_t)0x00000020U)        /*!< D-bus read protection protection of sector 5  */
#define OB_Drp_6                   ((uint32_t)0x00000040U)        /*!< D-bus read protection protection of sector 6  */
#define OB_Drp_7                   ((uint32_t)0x00000080U)        /*!< D-bus read protection protection of sector 7  */
#define OB_Drp_8                   ((uint32_t)0x00000100U)        /*!< D-bus read protection protection of sector 8  */
#define OB_Drp_9                   ((uint32_t)0x00000200U)        /*!< D-bus read protection protection of sector 9  */
#define OB_Drp_10                  ((uint32_t)0x00000400U)        /*!< D-bus read protection protection of sector 10 */
#define OB_Drp_11                  ((uint32_t)0x00000800U)        /*!< D-bus read protection protection of sector 11 */
#define OB_Drp_12                  ((uint32_t)0x00010000U)        /*!< D-bus read protection protection of sector 12 */
#define OB_Drp_13                  ((uint32_t)0x00020000U)        /*!< D-bus read protection protection of sector 13 */
#define OB_Drp_14                  ((uint32_t)0x00040000U)        /*!< D-bus read protection protection of sector 14 */
#define OB_Drp_15                  ((uint32_t)0x00080000U)        /*!< D-bus read protection protection of sector 15 */
#define OB_Drp_16                  ((uint32_t)0x00100000U)        /*!< D-bus read protection protection of sector 16 */
#define OB_Drp_17                  ((uint32_t)0x00200000U)        /*!< D-bus read protection protection of sector 17 */
#define OB_Drp_18                  ((uint32_t)0x00400000U)        /*!< D-bus read protection protection of sector 18 */
#define OB_Drp_19                  ((uint32_t)0x00800000U)        /*!< D-bus read protection protection of sector 19 */
#define OB_Drp_20                  ((uint32_t)0x01000000U)        /*!< D-bus read protection protection of sector 20 */
#define OB_Drp_21                  ((uint32_t)0x02000000U)        /*!< D-bus read protection protection of sector 21 */
#define OB_Drp_22                  ((uint32_t)0x04000000U)        /*!< D-bus read protection protection of sector 22 */
#define OB_Drp_23_27               ((uint32_t)0x08000000U)        /*!< D-bus read protection protection of sector 23~27 */
#define OB_Drp_ALL                 ((uint32_t)0x0FFF0FFFU)        /*!< D-bus read protection protection of all sectors */

/* double banks or single bank selection when flash size is 1M bytes */
#define OBCTL0_DBS(regval)         (BIT(30) & ((uint32_t)(regval) << 30U))
#define OB_DBS_DISABLE             OBCTL0_DBS(0)                  /*!< single bank when flash size is 1M bytes */
#define OB_DBS_ENABLE              OBCTL0_DBS(1)                  /*!< double bank when flash size is 1M bytes */

/* option bytes D-bus read protection mode */
#define OBCTL0_Drp(regval)         (BIT(31) & ((uint32_t)(regval) << 31U))
#define OB_Drp_DISABLE             OBCTL0_Drp(0)                  /*!< the WPx bits used as erase/program protection of each sector */
#define OB_Drp_ENABLE              OBCTL0_Drp(1)                  /*!< the WPx bits used as erase/program protection and D-bus read protection of each sector */

/* FMC sectors */
#define CTL_SN(regval)             (BITS(3,7) & ((uint32_t)(regval))<< 3)
#define CTL_SECTOR_Number_0        CTL_SN(0)                      /*!< sector 0   */
#define CTL_SECTOR_Number_1        CTL_SN(1)                      /*!< sector 1   */
#define CTL_SECTOR_Number_2        CTL_SN(2)                      /*!< sector 2   */
#define CTL_SECTOR_Number_3        CTL_SN(3)                      /*!< sector 3   */
#define CTL_SECTOR_Number_4        CTL_SN(4)                      /*!< sector 4   */
#define CTL_SECTOR_Number_5        CTL_SN(5)                      /*!< sector 5   */
#define CTL_SECTOR_Number_6        CTL_SN(6)                      /*!< sector 6   */
#define CTL_SECTOR_Number_7        CTL_SN(7)                      /*!< sector 7   */
#define CTL_SECTOR_Number_8        CTL_SN(8)                      /*!< sector 8   */
#define CTL_SECTOR_Number_9        CTL_SN(9)                      /*!< sector 9   */
#define CTL_SECTOR_Number_10       CTL_SN(10)                     /*!< sector 10  */
#define CTL_SECTOR_Number_11       CTL_SN(11)                     /*!< sector 11  */
#define CTL_SECTOR_Number_24       CTL_SN(12)                     /*!< sector 24  */
#define CTL_SECTOR_Number_25       CTL_SN(13)                     /*!< sector 25  */
#define CTL_SECTOR_Number_26       CTL_SN(14)                     /*!< sector 26  */
#define CTL_SECTOR_Number_27       CTL_SN(15)                     /*!< sector 27  */
#define CTL_SECTOR_Number_12       CTL_SN(16)                     /*!< sector 12  */
#define CTL_SECTOR_Number_13       CTL_SN(17)                     /*!< sector 13  */
#define CTL_SECTOR_Number_14       CTL_SN(18)                     /*!< sector 14  */
#define CTL_SECTOR_Number_15       CTL_SN(19)                     /*!< sector 15  */
#define CTL_SECTOR_Number_16       CTL_SN(20)                     /*!< sector 16  */
#define CTL_SECTOR_Number_17       CTL_SN(21)                     /*!< sector 17  */
#define CTL_SECTOR_Number_18       CTL_SN(22)                     /*!< sector 18  */
#define CTL_SECTOR_Number_19       CTL_SN(23)                     /*!< sector 19  */
#define CTL_SECTOR_Number_20       CTL_SN(24)                     /*!< sector 20  */
#define CTL_SECTOR_Number_21       CTL_SN(25)                     /*!< sector 21  */
#define CTL_SECTOR_Number_22       CTL_SN(26)                     /*!< sector 22  */
#define CTL_SECTOR_Number_23       CTL_SN(27)                     /*!< sector 23  */


/* FMC program size */
#define CTL_PSZ(regval)            (BITS(8,9) & ((uint32_t)(regval))<< 8U)
#define CTL_PSZ_BYTE               CTL_PSZ(0)                     /*!< FMC program by byte access */
#define CTL_PSZ_HALF_WORD          CTL_PSZ(1)                     /*!< FMC program by half-word access */
#define CTL_PSZ_WORD               CTL_PSZ(2)                     /*!< FMC program by word access */

/* FMC interrupt enable */
#define FMC_INT_END                ((uint32_t)0x01000000U)        /*!< enable FMC end of program 中断 */
#define FMC_INT_ERR                ((uint32_t)0x02000000U)        /*!< enable FMC error 中断 */

/* FMC flags */
#define FMC_Flag_END               FMC_STAT_END                   /*!< FMC end of operation flag bit */
#define FMC_Flag_OPERR             FMC_STAT_OPERR                 /*!< FMC operation error flag bit */
#define FMC_Flag_WPERR             FMC_STAT_WPERR                 /*!< FMC erase/program protection error flag bit */
#define FMC_Flag_PGMERR            FMC_STAT_PGMERR                /*!< FMC program size not match error flag bit */
#define FMC_Flag_PGSERR            FMC_STAT_PGSERR                /*!< FMC program sequence error flag bit */
#define FMC_Flag_RDDERR            FMC_STAT_RDDERR                /*!< FMC read D-bus protection error flag bit */
#define FMC_Flag_BUSY              FMC_STAT_BUSY                  /*!< FMC busy flag */

/* FMC interrupt flags */
#define FMC_INT_Flag_END           FMC_STAT_END                   /*!< FMC end of operation interrupt flag */
#define FMC_INT_Flag_OPERR         FMC_STAT_OPERR                 /*!< FMC operation error interrupt flag */
#define FMC_INT_Flag_WPERR         FMC_STAT_WPERR                 /*!< FMC erase/program protection error interrupt flag */
#define FMC_INT_Flag_PGMERR        FMC_STAT_PGMERR                /*!< FMC program size not match error interrupt flag */
#define FMC_INT_Flag_PGSERR        FMC_STAT_PGSERR                /*!< FMC program sequence error interrupt flag */
#define FMC_INT_Flag_RDDERR        FMC_STAT_RDDERR                /*!< FMC read D-bus protection error interrupt flag */


/* FMC time out */
#define FMC_Timeout_COUNT          ((uint32_t)0x4FFFFFFFU)        /*!< count to judge of FMC timeout */

/* function declarations */
/* FMC main memory programming functions */
/* set the FMC wait state counter */
void FMC_wscnt_Set(uint32_t wscnt);
/* unlock the main FMC operation */
void FMC_unlock(void);
/* lock the main FMC operation */
void FMC_lock(void);
#if defined (GD32F425) || defined (GD32F427) || defined (GD32F470)
/* FMC erase page */
FMC_State_enum FMC_page_Erase(uint32_t page_addr);
#endif
/* FMC erase sector */
FMC_State_enum FMC_sector_Erase(uint32_t FMC_sector);
/* FMC erase whole chip */
FMC_State_enum FMC_mass_Erase(void);
/* FMC erase whole bank0 */
FMC_State_enum FMC_bank0_Erase(void);
/* FMC erase whole bank1 */
FMC_State_enum FMC_bank1_Erase(void);
/* FMC program a word at the corresponding address */
FMC_State_enum FMC_Word_program(uint32_t address, uint32_t data);
/* FMC program a half word at the corresponding address */
FMC_State_enum FMC_halfword_program(uint32_t address, uint16_t data);
/* FMC program a byte at the corresponding address */
FMC_State_enum FMC_byte_program(uint32_t address, uint8_t data);

/* FMC option bytes programming functions */
/* unlock the option byte operation */
void OB_unlock(void);
/* lock the option byte operation */
void OB_lock(void);
/* send option byte change command */
void OB_start(void);
/* erase option byte */
void OB_Erase(void);
/* enable write protect */
ErrStatus OB_Write_Protection_Enable(uint32_t OB_wp);
/* disable write protect */
ErrStatus OB_Write_Protection_Disable(uint32_t OB_wp);
/* enable erase/program protection and D-bus read protection */
void OB_Drp_Enable(uint32_t OB_Drp);
/* disable erase/program protection and D-bus read protection */
void OB_Drp_Disable(void);
/* configure security protection level */
void OB_security_Protection_Config(uint8_t OB_spc);
/* program the FMC user option byte */
void OB_User_Write(uint32_t OB_fwdgt, uint32_t OB_deepsleep, uint32_t OB_stdby);
/* program the option byte BOR threshold value */
void OB_User_Bor_threshold(uint32_t OB_Bor_th);
/* configure the boot mode */
void OB_boot_Mode_Config(uint32_t boot_mode);
/* get the FMC user option byte */
uint8_t OB_User_Get(void);
/* get the FMC option byte write protection */
uint16_t OB_Write_Protection0_Get(void);
/* get the FMC option byte write protection */
uint16_t OB_Write_Protection1_Get(void);
/* get the FMC erase/program protection and D-bus read protection option bytes value */
uint16_t OB_Drp0_Get(void);
/* get the FMC erase/program protection and D-bus read protection option bytes value */
uint16_t OB_Drp1_Get(void);
/* get option byte security protection code value */
FlagStatus OB_spc_Get(void);
/* get the FMC option byte BOR threshold value */
uint8_t OB_User_Bor_Threshold_Get(void);

/* FMC interrupts and flags management functions */
/* get flag set or reset */
FlagStatus FMC_Flag_Get(uint32_t FMC_flag);
/* clear the FMC pending flag */
void FMC_Flag_Clear(uint32_t FMC_flag);
/* enable FMC 中断 */
void FMC_Interrupt_Enable(uint32_t FMC_int);
/* disable FMC 中断 */
void FMC_Interrupt_Disable(uint32_t FMC_int);
/* get FMC interrupt flag set or reset */
FlagStatus FMC_Interrupt_Flag_Get(uint32_t FMC_int_flag);
/* clear the FMC interrupt flag */
void FMC_Interrupt_Flag_Clear(uint32_t FMC_int_flag);
/* get the FMC state */
FMC_State_enum FMC_State_Get(void);
/* check whether FMC is ready or not */
FMC_State_enum FMC_ready_Wait(uint32_t timeout);

#endif /* GD32F4XX_FMC_H */
