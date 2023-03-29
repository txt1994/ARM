/*!
    文件:    gd32f4xx_fmc.c
    简介:   FMC driver

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


#include "gd32f4xx_fmc.h"

/*!
    简介:    set the FMC wait state counter
    参数[输入]:  wscnt: wait state counter value
                only one parameter can be selected which is shown as below:
      参数:        WS_WSCNT_0: FMC 0 wait
      参数:        WS_WSCNT_1: FMC 1 wait
      参数:        WS_WSCNT_2: FMC 2 wait
      参数:        WS_WSCNT_3: FMC 3 wait
      参数:        WS_WSCNT_4: FMC 4 wait
      参数:        WS_WSCNT_5: FMC 5 wait
      参数:        WS_WSCNT_6: FMC 6 wait
      参数:        WS_WSCNT_7: FMC 7 wait
      参数:        WS_WSCNT_8: FMC 8 wait
      参数:        WS_WSCNT_9: FMC 9 wait
      参数:        WS_WSCNT_10: FMC 10 wait
      参数:        WS_WSCNT_11: FMC 11 wait
      参数:        WS_WSCNT_12: FMC 12 wait
      参数:        WS_WSCNT_13: FMC 13 wait
      参数:        WS_WSCNT_14: FMC 14 wait
      参数:        WS_WSCNT_15: FMC 15 wait
    参数[输出]:  无
    返回值:      无
*/
void FMC_wscnt_set(uint32_t wscnt) {
    uint32_t reg;

    reg = FMC_WS;
    /* set the wait state counter value */
    reg &= ~FMC_WC_WSCNT;
    FMC_WS = (reg | wscnt);
}

/*!
    简介:    unlock the main FMC operation
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void FMC_unlock(void) {
    if((RESET != (FMC_CTL & FMC_CTL_LK))) {
        /* write the FMC key */
        FMC_KEY = UNLOCK_KEY0;
        FMC_KEY = UNLOCK_KEY1;
    }
}

/*!
    简介:    lock the main FMC operation
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void FMC_lock(void) {
    /* set the LK bit*/
    FMC_CTL |= FMC_CTL_LK;
}

#if defined (GD32F425) || defined (GD32F427) || defined (GD32F470)

/*!
    简介:      FMC erase page
    参数[输入]:  page_addr: the page address to be erased.
    参数[输出]:  无
    返回值:     state of FMC
      参数:        FMC_READY: the operation has been completed
      参数:        FMC_BUSY: the operation is in progress
      参数:        FMC_RDDERR: read D-bus protection error
      参数:        FMC_PGSERR: program sequence error
      参数:        FMC_PGMERR: program size not match error
      参数:        FMC_WPERR: erase/program protection error
      参数:        FMC_OPERR: operation error
      参数:        FMC_TOERR: timeout error
*/
FMC_state_enum FMC_page_erase(uint32_t page_addr) {
    FMC_state_enum FMC_state = FMC_READY;

    /* wait for the FMC ready */
    FMC_state = FMC_ready_Wait(FMC_TIMEOUT_COUNT);

    if(FMC_READY == FMC_state) {
        /* unlock page erase operation */
        FMC_PEKEY = UNLOCK_PE_KEY;

        /* start page erase */
        FMC_PECFG = FMC_PE_EN | page_addr;
        FMC_CTL &= ~FMC_CTL_SN;
        FMC_CTL |= FMC_CTL_SER;
        FMC_CTL |= FMC_CTL_START;

        /* wait for the FMC ready */
        FMC_state = FMC_ready_Wait(FMC_TIMEOUT_COUNT);

        FMC_PECFG &= ~FMC_PE_EN;
        FMC_CTL &= ~FMC_CTL_SER;
    }

    /* return the FMC state */
    return FMC_state;
}

#endif

/*!
    简介:    FMC erase sector
    参数[输入]:  FMC_sector: select the sector to erase
                only one parameter can be selected which is shown as below:
      参数:        CTL_SECTOR_NUMBER_0: sector 0
      参数:        CTL_SECTOR_NUMBER_1: sector 1
      参数:        CTL_SECTOR_NUMBER_2: sector 2
      参数:        CTL_SECTOR_NUMBER_3: sector 3
      参数:        CTL_SECTOR_NUMBER_4: sector 4
      参数:        CTL_SECTOR_NUMBER_5: sector 5
      参数:        CTL_SECTOR_NUMBER_6: sector 6
      参数:        CTL_SECTOR_NUMBER_7: sector 7
      参数:        CTL_SECTOR_NUMBER_8: sector 8
      参数:        CTL_SECTOR_NUMBER_9: sector 9
      参数:        CTL_SECTOR_NUMBER_10: sector 10
      参数:        CTL_SECTOR_NUMBER_11: sector 11
      参数:        CTL_SECTOR_NUMBER_12: sector 12
      参数:        CTL_SECTOR_NUMBER_13: sector 13
      参数:        CTL_SECTOR_NUMBER_14: sector 14
      参数:        CTL_SECTOR_NUMBER_15: sector 15
      参数:        CTL_SECTOR_NUMBER_16: sector 16
      参数:        CTL_SECTOR_NUMBER_17: sector 17
      参数:        CTL_SECTOR_NUMBER_18: sector 18
      参数:        CTL_SECTOR_NUMBER_19: sector 19
      参数:        CTL_SECTOR_NUMBER_20: sector 20
      参数:        CTL_SECTOR_NUMBER_21: sector 21
      参数:        CTL_SECTOR_NUMBER_22: sector 22
      参数:        CTL_SECTOR_NUMBER_23: sector 23
      参数:        CTL_SECTOR_NUMBER_24: sector 24
      参数:        CTL_SECTOR_NUMBER_25: sector 25
      参数:        CTL_SECTOR_NUMBER_26: sector 26
      参数:        CTL_SECTOR_NUMBER_27: sector 27
    参数[输出]:  无
    返回值:     state of FMC
      参数:        FMC_READY: the operation has been completed
      参数:        FMC_BUSY: the operation is in progress
      参数:        FMC_RDDERR: read D-bus protection error
      参数:        FMC_PGSERR: program sequence error
      参数:        FMC_PGMERR: program size not match error
      参数:        FMC_WPERR: erase/program protection error
      参数:        FMC_OPERR: operation error
      参数:        FMC_TOERR: timeout error
*/
FMC_state_enum FMC_sector_erase(uint32_t FMC_sector) {
    FMC_state_enum FMC_state = FMC_READY;
    /* wait for the FMC ready */
    FMC_state = FMC_ready_Wait(FMC_TIMEOUT_COUNT);

    if(FMC_READY == FMC_state) {
        /* start sector erase */
        FMC_CTL &= ~FMC_CTL_SN;
        FMC_CTL |= (FMC_CTL_SER | FMC_sector);
        FMC_CTL |= FMC_CTL_START;

        /* wait for the FMC ready */
        FMC_state = FMC_ready_Wait(FMC_TIMEOUT_COUNT);

        /* reset the SER bit */
        FMC_CTL &= (~FMC_CTL_SER);
        FMC_CTL &= ~FMC_CTL_SN;
    }

    /* return the FMC state */
    return FMC_state;
}

/*!
    简介:    FMC erase whole chip
    参数[输入]:  无
    参数[输出]:  无
    返回值:     state of FMC
      参数:        FMC_READY: the operation has been completed
      参数:        FMC_BUSY: the operation is in progress
      参数:        FMC_RDDERR: read D-bus protection error
      参数:        FMC_PGSERR: program sequence error
      参数:        FMC_PGMERR: program size not match error
      参数:        FMC_WPERR: erase/program protection error
      参数:        FMC_OPERR: operation error
      参数:        FMC_TOERR: timeout error
*/
FMC_state_enum FMC_mass_erase(void) {
    FMC_state_enum FMC_state = FMC_READY;
    /* wait for the FMC ready */
    FMC_state = FMC_ready_Wait(FMC_TIMEOUT_COUNT);

    if(FMC_READY == FMC_state) {
        /* start whole chip erase */
        FMC_CTL |= (FMC_CTL_MER0 | FMC_CTL_MER1);
        FMC_CTL |= FMC_CTL_START;

        /* wait for the FMC ready */
        FMC_state = FMC_ready_Wait(FMC_TIMEOUT_COUNT);

        /* reset the MER bits */
        FMC_CTL &= ~(FMC_CTL_MER0 | FMC_CTL_MER1);
    }

    /* return the fmc state */
    return FMC_state;
}

/*!
    简介:    FMC erase whole bank0
    参数[输入]:  无
    参数[输出]:  无
    返回值:     state of FMC
      参数:        FMC_READY: the operation has been completed
      参数:        FMC_BUSY: the operation is in progress
      参数:        FMC_RDDERR: read D-bus protection error
      参数:        FMC_PGSERR: program sequence error
      参数:        FMC_PGMERR: program size not match error
      参数:        FMC_WPERR: erase/program protection error
      参数:        FMC_OPERR: operation error
      参数:        FMC_TOERR: timeout error
*/
FMC_state_enum FMC_bank0_erase(void) {
    FMC_state_enum FMC_state = FMC_READY;
    /* wait for the FMC ready */
    FMC_state = FMC_ready_Wait(FMC_TIMEOUT_COUNT);

    if(FMC_READY == FMC_state) {
        /* start FMC bank0 erase */
        FMC_CTL |= FMC_CTL_MER0;
        FMC_CTL |= FMC_CTL_START;

        /* wait for the FMC ready */
        FMC_state = FMC_ready_Wait(FMC_TIMEOUT_COUNT);

        /* reset the MER0 bit */
        FMC_CTL &= (~FMC_CTL_MER0);
    }

    /* return the fmc state */
    return FMC_state;
}

/*!
    简介:    FMC erase whole bank1
    参数[输入]:  无
    参数[输出]:  无
    返回值:     state of FMC
      参数:        FMC_READY: the operation has been completed
      参数:        FMC_BUSY: the operation is in progress
      参数:        FMC_RDDERR: read D-bus protection error
      参数:        FMC_PGSERR: program sequence error
      参数:        FMC_PGMERR: program size not match error
      参数:        FMC_WPERR: erase/program protection error
      参数:        FMC_OPERR: operation error
      参数:        FMC_TOERR: timeout error
*/
FMC_state_enum FMC_bank1_erase(void) {
    FMC_state_enum FMC_state = FMC_READY;
    /* wait for the FMC ready */
    FMC_state = FMC_ready_Wait(FMC_TIMEOUT_COUNT);

    if(FMC_READY == FMC_state) {
        /* start FMC bank1 erase */
        FMC_CTL |= FMC_CTL_MER1;
        FMC_CTL |= FMC_CTL_START;

        /* wait for the FMC ready */
        FMC_state = FMC_ready_Wait(FMC_TIMEOUT_COUNT);

        /* reset the MER1 bit */
        FMC_CTL &= (~FMC_CTL_MER1);
    }

    /* return the fmc state */
    return FMC_state;
}

/*!
    简介:    program a word at the corresponding address
    参数[输入]:  address: address to program
    参数[输入]:  data: word to program(0x00000000 - 0xFFFFFFFF)
    参数[输出]:  无
    返回值:     state of FMC
      参数:        FMC_READY: the operation has been completed
      参数:        FMC_BUSY: the operation is in progress
      参数:        FMC_RDDERR: read D-bus protection error
      参数:        FMC_PGSERR: program sequence error
      参数:        FMC_PGMERR: program size not match error
      参数:        FMC_WPERR: erase/program protection error
      参数:        FMC_OPERR: operation error
      参数:        FMC_TOERR: timeout error
*/
FMC_state_enum FMC_Word_program(uint32_t address, uint32_t data) {
    FMC_state_enum FMC_state = FMC_READY;
    /* wait for the FMC ready */
    FMC_state = FMC_ready_Wait(FMC_TIMEOUT_COUNT);

    if(FMC_READY == FMC_state) {
        /* set the PG bit to start program */
        FMC_CTL &= ~FMC_CTL_PSZ;
        FMC_CTL |= CTL_PSZ_WORD;
        FMC_CTL |= FMC_CTL_PG;

        REG32(address) = data;

        /* wait for the FMC ready */
        FMC_state = FMC_ready_Wait(FMC_TIMEOUT_COUNT);

        /* reset the PG bit */
        FMC_CTL &= ~FMC_CTL_PG;
    }

    /* return the FMC state */
    return FMC_state;
}

/*!
    简介:    program a half word at the corresponding address
    参数[输入]:  address: address to program
    参数[输入]:  data: halfword to program(0x0000 - 0xFFFF)
    参数[输出]:  无
    返回值:     state of FMC
      参数:        FMC_READY: the operation has been completed
      参数:        FMC_BUSY: the operation is in progress
      参数:        FMC_RDDERR: read D-bus protection error
      参数:        FMC_PGSERR: program sequence error
      参数:        FMC_PGMERR: program size not match error
      参数:        FMC_WPERR: erase/program protection error
      参数:        FMC_OPERR: operation error
      参数:        FMC_TOERR: timeout error
*/
FMC_state_enum FMC_halfword_program(uint32_t address, uint16_t data) {
    FMC_state_enum FMC_state = FMC_READY;
    /* wait for the FMC ready */
    FMC_state = FMC_ready_Wait(FMC_TIMEOUT_COUNT);

    if(FMC_READY == FMC_state) {
        /* set the PG bit to start program */
        FMC_CTL &= ~FMC_CTL_PSZ;
        FMC_CTL |= CTL_PSZ_HALF_WORD;
        FMC_CTL |= FMC_CTL_PG;

        REG16(address) = data;

        /* wait for the FMC ready */
        FMC_state = FMC_ready_Wait(FMC_TIMEOUT_COUNT);

        /* reset the PG bit */
        FMC_CTL &= ~FMC_CTL_PG;
    }

    /* return the FMC state */
    return FMC_state;
}

/*!
    简介:    program a byte at the corresponding address
    参数[输入]:  address: address to program
    参数[输入]:  data: byte to program(0x00 - 0xFF)
    参数[输出]:  无
    返回值:     state of FMC
      参数:        FMC_READY: the operation has been completed
      参数:        FMC_BUSY: the operation is in progress
      参数:        FMC_RDDERR: read D-bus protection error
      参数:        FMC_PGSERR: program sequence error
      参数:        FMC_PGMERR: program size not match error
      参数:        FMC_WPERR: erase/program protection error
      参数:        FMC_OPERR: operation error
      参数:        FMC_TOERR: timeout error
*/
FMC_state_enum FMC_byte_program(uint32_t address, uint8_t data) {
    FMC_state_enum FMC_state = FMC_READY;
    /* wait for the FMC ready */
    FMC_state = FMC_ready_Wait(FMC_TIMEOUT_COUNT);

    if(FMC_READY == FMC_state) {
        /* set the PG bit to start program */
        FMC_CTL &= ~FMC_CTL_PSZ;
        FMC_CTL |= CTL_PSZ_BYTE;
        FMC_CTL |= FMC_CTL_PG;

        REG8(address) = data;

        /* wait for the FMC ready */
        FMC_state = FMC_ready_Wait(FMC_TIMEOUT_COUNT);

        /* reset the PG bit */
        FMC_CTL &= ~FMC_CTL_PG;
    }

    /* return the FMC state */
    return FMC_state;
}

/*!
    简介:    unlock the option byte operation
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void ob_unlock(void) {
    if(RESET != (FMC_OBCTL0 & FMC_OBCTL0_OB_LK)) {
        /* write the FMC key */
        FMC_OBKEY = OB_UNLOCK_KEY0;
        FMC_OBKEY = OB_UNLOCK_KEY1;
    }
}

/*!
    简介:    lock the option byte operation
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void ob_lock(void) {
    /* reset the OB_LK bit */
    FMC_OBCTL0 |= FMC_OBCTL0_OB_LK;
}

/*!
    简介:    send option byte change command
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void ob_start(void) {
    /* set the OB_START bit in OBCTL0 register */
    FMC_OBCTL0 |= FMC_OBCTL0_OB_START;
}

/*!
    简介:    erase option byte
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void ob_erase(void) {
    uint32_t reg, reg1;
    FMC_state_enum FMC_state = FMC_READY;
    /* wait for the FMC ready */
    FMC_state = FMC_ready_Wait(FMC_TIMEOUT_COUNT);
    reg = FMC_OBCTL0;
    reg1 = FMC_OBCTL1;

    if(FMC_READY == FMC_state) {

        /* reset the OB_FWDGT, OB_DEEPSLEEP and OB_STDBY, set according to ob_fwdgt ,ob_deepsleep and ob_stdby */
        reg |= (FMC_OBCTL0_NWDG_HW | FMC_OBCTL0_NRST_DPSLP | FMC_OBCTL0_NRST_STDBY);
        /* reset the BOR level */
        reg |= FMC_OBCTL0_BOR_TH;
        /* reset option byte boot bank value */
        reg &= ~FMC_OBCTL0_BB;
        /* reset option byte dbs value */
        reg &= ~FMC_OBCTL0_DBS;

        /* reset drp and wp value */
        reg |= FMC_OBCTL0_WP0;
        reg &= (~FMC_OBCTL0_DRP);
        FMC_OBCTL0 = reg;

        reg1 |= FMC_OBCTL1_WP1;
        FMC_OBCTL1 = reg1;

        FMC_OBCTL0 = reg;
    }
}

/*!
    简介:    enable write protection
    参数[输入]:  ob_wp: specify sector to be write protected
                one or more parameters can be selected which are shown as below:
      参数:        OB_WP_x(x=0..22):sector x(x = 0,1,2...22)
      参数:        OB_WP_23_27: sector23~27
      参数:        OB_WP_ALL: all sector
    参数[输出]:  无
    返回值:     SUCCESS or ERROR
*/
ErrStatus ob_write_protection_enable(uint32_t ob_wp) {
    uint32_t reg0 = FMC_OBCTL0;
    uint32_t reg1 = FMC_OBCTL1;
    FMC_state_enum FMC_state = FMC_READY;

    if(RESET != (FMC_OBCTL0 & FMC_OBCTL0_DRP)) {
        return ERROR;
    }

    /* wait for the FMC ready */
    FMC_state = FMC_ready_Wait(FMC_TIMEOUT_COUNT);

    if(FMC_READY == FMC_state) {
        reg0 &= (~((uint32_t)ob_wp << 16U));
        reg1 &= (~(ob_wp & 0xFFFF0000U));
        FMC_OBCTL0 = reg0;
        FMC_OBCTL1 = reg1;

        return SUCCESS;
    } else {
        return ERROR;
    }
}

/*!
    简介:    disable write protection
    参数[输入]:  ob_wp: specify sector to be write protected
                one or more parameters can be selected which are shown as below:
      参数:        OB_WP_x(x=0..22):sector x(x = 0,1,2...22)
      参数:        OB_WP_23_27: sector23~27
      参数:        OB_WP_ALL: all sector
    参数[输出]:  无
    返回值:     SUCCESS or ERROR
*/
ErrStatus ob_write_protection_disable(uint32_t ob_wp) {
    uint32_t reg0 = FMC_OBCTL0;
    uint32_t reg1 = FMC_OBCTL1;
    FMC_state_enum FMC_state = FMC_READY;

    if(RESET != (FMC_OBCTL0 & FMC_OBCTL0_DRP)) {
        return ERROR;
    }

    /* wait for the FMC ready */
    FMC_state = FMC_ready_Wait(FMC_TIMEOUT_COUNT);

    if(FMC_READY == FMC_state) {
        reg0 |= ((uint32_t)ob_wp << 16U);
        reg1 |= (ob_wp & 0xFFFF0000U);
        FMC_OBCTL0 = reg0;
        FMC_OBCTL1 = reg1;

        return SUCCESS;
    } else {
        return ERROR;
    }
}

/*!
    简介:    enable erase/program protection and D-bus read protection
    参数[输入]:  ob_drp: enable the WPx bits used as erase/program protection and D-bus read protection of each sector
                one or more parameters can be selected which are shown as below:
      参数:        OB_DRP_x(x=0..22): sector x(x = 0,1,2...22)
      参数:        OB_DRP_23_27: sector23~27
      参数:        OB_DRP_ALL: all sector
    参数[输出]:  无
    返回值:      无
*/
void ob_drp_enable(uint32_t ob_drp) {
    uint32_t reg0 = FMC_OBCTL0;
    uint32_t reg1 = FMC_OBCTL1;
    FMC_state_enum FMC_state = FMC_READY;
    uint32_t drp_state = FMC_OBCTL0 & FMC_OBCTL0_DRP;
    uint32_t wp0_state = FMC_OBCTL0 & FMC_OBCTL0_WP0;
    uint32_t wp1_state = FMC_OBCTL1 & FMC_OBCTL1_WP1;

    /* wait for the FMC ready */
    FMC_state = FMC_ready_Wait(FMC_TIMEOUT_COUNT);

    if(FMC_READY == FMC_state) {
        if(RESET == drp_state) {
            reg0 &= ~FMC_OBCTL0_WP0;
            reg1 &= ~FMC_OBCTL1_WP1;
        }

        reg0 |= ((uint32_t)ob_drp << 16U);
        reg0 |= FMC_OBCTL0_DRP;
        reg1 |= ((uint32_t)ob_drp & 0xFFFF0000U);

        FMC_OBCTL0 = reg0;
        FMC_OBCTL1 = reg1;
    }
}

/*!
    简介:    disable erase/program protection and D-bus read protection
    参数[输入]:  ob_drp: disable the WPx bits used as erase/program protection and D-bus read protection of each sector
                one or more parameters can be selected which are shown as below:
      参数:        OB_DRP_x(x=0..22): sector x(x = 0,1,2...22)
      参数:        OB_DRP_23_27: sector23~27
      参数:        OB_DRP_ALL: all sector
    参数[输出]:  无
    返回值:      无
*/
void ob_drp_disable(void) {
    uint32_t reg0 = FMC_OBCTL0;
    uint32_t reg1 = FMC_OBCTL1;
    FMC_state_enum FMC_state = FMC_READY;
    /* wait for the FMC ready */
    FMC_state = FMC_ready_Wait(FMC_TIMEOUT_COUNT);

    if(FMC_READY == FMC_state) {
        if(((uint8_t)(reg0 >> 8U)) == (uint8_t)FMC_NSPC) {
            /* security protection should be set as low level protection before disable D-BUS read protection */
            reg0 &= ~FMC_OBCTL0_SPC;
            reg0 |= ((uint32_t)FMC_LSPC << 8U);
            FMC_OBCTL0 = reg0;
            /* set the OB_START bit in OBCTL0 register */
            FMC_OBCTL0 |= FMC_OBCTL0_OB_START;
        }

        /* it is necessary to disable the security protection at the same time when D-BUS read protection is disabled */
        reg0 &= ~FMC_OBCTL0_SPC;
        reg0 |= ((uint32_t)FMC_NSPC << 8U);
        reg0 |= FMC_OBCTL0_WP0;
        reg0 &= (~FMC_OBCTL0_DRP);
        FMC_OBCTL0 = reg0;

        reg1 |= FMC_OBCTL1_WP1;
        FMC_OBCTL1 = reg1;

    }
}

/*!
    简介:    configure security protection level
    参数[输入]:  ob_spc: specify security protection level
                only one parameter can be selected which is shown as below:
      参数:        FMC_NSPC: no security protection
      参数:        FMC_LSPC: low security protection
      参数:        FMC_HSPC: high security protection
    参数[输出]:  无
    返回值:      无
*/
void ob_security_protection_Config(uint8_t ob_spc) {
    FMC_state_enum FMC_state = FMC_READY;
    /* wait for the FMC ready */
    FMC_state = FMC_ready_Wait(FMC_TIMEOUT_COUNT);

    if(FMC_READY == FMC_state) {
        uint32_t reg;

        reg = FMC_OBCTL0;
        /* reset the OBCTL0_SPC, set according to ob_spc */
        reg &= ~FMC_OBCTL0_SPC;
        reg |= ((uint32_t)ob_spc << 8U);
        FMC_OBCTL0 = reg;
    }
}

/*!
    简介:    program the FMC user option byte
    参数[输入]:  ob_fwdgt: option byte watchdog value
                only one parameter can be selected which is shown as below:
      参数:        OB_FWDGT_SW: software free watchdog
      参数:        OB_FWDGT_HW: hardware free watchdog
    参数[输入]:  ob_deepsleep: option byte deepsleep reset value
                only one parameter can be selected which is shown as below:
      参数:        OB_DEEPSLEEP_NRST: no reset when entering deepsleep mode
      参数:        OB_DEEPSLEEP_RST: generate a reset instead of entering deepsleep mode
    参数[输入]:  ob_stdby:option byte standby reset value
                only one parameter can be selected which is shown as below:
      参数:        OB_STDBY_NRST: no reset when entering standby mode
      参数:        OB_STDBY_RST: generate a reset instead of entering standby mode
    参数[输出]:  无
    返回值:      无
*/
void ob_user_write(uint32_t ob_fwdgt, uint32_t ob_deepsleep, uint32_t ob_stdby) {
    FMC_state_enum FMC_state = FMC_READY;

    /* wait for the FMC ready */
    FMC_state = FMC_ready_Wait(FMC_TIMEOUT_COUNT);

    if(FMC_READY == FMC_state) {
        uint32_t reg;

        reg = FMC_OBCTL0;
        /* reset the OB_FWDGT, OB_DEEPSLEEP and OB_STDBY, set according to ob_fwdgt ,ob_deepsleep and ob_stdby */
        reg &= ~(FMC_OBCTL0_NWDG_HW | FMC_OBCTL0_NRST_DPSLP | FMC_OBCTL0_NRST_STDBY);
        FMC_OBCTL0 = (reg | ob_fwdgt | ob_deepsleep | ob_stdby);
    }
}

/*!
    简介:    program the option byte BOR threshold value
    参数[输入]:  ob_bor_th: user option byte
                only one parameter can be selected which is shown as below:
      参数:        OB_BOR_TH_VALUE3: BOR threshold value 3
      参数:        OB_BOR_TH_VALUE2: BOR threshold value 2
      参数:        OB_BOR_TH_VALUE1: BOR threshold value 1
      参数:        OB_BOR_TH_OFF: no BOR function
    参数[输出]:  无
    返回值:      无
*/
void ob_user_bor_threshold(uint32_t ob_bor_th) {
    uint32_t reg;

    reg = FMC_OBCTL0;
    /* set the BOR level */
    reg &= ~FMC_OBCTL0_BOR_TH;
    FMC_OBCTL0 = (reg | ob_bor_th);
}

/*!
    简介:    configure the option byte boot bank value
    参数[输入]:  boot_mode: specifies the option byte boot bank value
                only one parameter can be selected which is shown as below:
      参数:        OB_BB_DISABLE: boot from bank0
      参数:        OB_BB_ENABLE: boot from bank1 or bank0 if bank1 is void
    参数[输出]:  无
    返回值:      无
*/
void ob_boot_mode_Config(uint32_t boot_mode) {
    uint32_t reg;

    reg = FMC_OBCTL0;
    /* set option byte boot bank value */
    reg &= ~FMC_OBCTL0_BB;
    FMC_OBCTL0 = (reg | boot_mode);
}

/*!
    简介:    get the FMC user option byte
    参数[输入]:  无
    参数[输出]:  无
    返回值:     the FMC user option byte values: ob_fwdgt(Bit0), ob_deepsleep(Bit1), ob_stdby(Bit2)
*/
uint8_t ob_user_get(void) {
    return (uint8_t)((uint8_t)(FMC_OBCTL0 >> 5U) & 0x07U);
}

/*!
    简介:    get the FMC option byte write protection
    参数[输入]:  无
    参数[输出]:  无
    返回值:     the FMC write protection option byte value
*/
uint16_t ob_write_protection0_get(void) {
    /* return the FMC write protection option byte value */
    return (uint16_t)(((uint16_t)(FMC_OBCTL0 >> 16U)) & 0x0FFFU);
}

/*!
    简介:    get the FMC option byte write protection
    参数[输入]:  无
    参数[输出]:  无
    返回值:     the FMC write protection option byte value
*/
uint16_t ob_write_protection1_get(void) {
    /* return the the FMC write protection option byte value */
    return (uint16_t)(((uint16_t)(FMC_OBCTL1 >> 16U)) & 0x0FFFU);
}

/*!
    简介:    get the FMC erase/program protection and D-bus read protection option bytes value
    参数[输入]:  无
    参数[输出]:  无
    返回值:     the FMC erase/program protection and D-bus read protection option bytes value
*/
uint16_t ob_drp0_get(void) {
    /* return the FMC erase/program protection and D-bus read protection option bytes value */
    if(FMC_OBCTL0 & FMC_OBCTL0_DRP) {
        return (uint16_t)(((uint16_t)(FMC_OBCTL0 >> 16U)) & 0x0FFFU);
    } else {
        return 0xF000U;
    }
}

/*!
    简介:    get the FMC erase/program protection and D-bus read protection option bytes value
    参数[输入]:  无
    参数[输出]:  无
    返回值:     the FMC erase/program protection and D-bus read protection option bytes value
*/
uint16_t ob_drp1_get(void) {
    /* return the FMC erase/program protection and D-bus read protection option bytes value */
    if(FMC_OBCTL0 & FMC_OBCTL0_DRP) {
        return (uint16_t)(((uint16_t)(FMC_OBCTL1 >> 16U)) & 0x0FFFU);
    } else {
        return 0xF000U;
    }
}

/*!
    简介:    get option byte security protection code value
    参数[输入]:  无
    参数[输出]:  无
    返回值:     FlagStatus: SET or RESET
*/
FlagStatus ob_spc_get(void) {
    FlagStatus spc_state = RESET;

    if(((uint8_t)(FMC_OBCTL0 >> 8U)) != FMC_NSPC) {
        spc_state = SET;
    } else {
        spc_state = RESET;
    }

    return spc_state;
}

/*!
    简介:    get the FMC option byte BOR threshold value
    参数[输入]:  无
    参数[输出]:  无
    返回值:     the FMC BOR threshold value:OB_BOR_TH_OFF,OB_BOR_TH_VALUE1,OB_BOR_TH_VALUE2,OB_BOR_TH_VALUE3
*/
uint8_t ob_user_bor_threshold_get(void) {
    /* return the FMC BOR threshold value */
    return (uint8_t)((uint8_t)FMC_OBCTL0 & 0x0CU);
}

/*!
    简介:    get flag set or reset
    参数[输入]:  FMC_flag: check FMC flag
                only one parameter can be selected which is shown as below:
      参数:        FMC_FLAG_BUSY: FMC busy flag bit
      参数:        FMC_FLAG_RDDERR: FMC read D-bus protection error flag bit
      参数:        FMC_FLAG_PGSERR: FMC program sequence error flag bit
      参数:        FMC_FLAG_PGMERR: FMC program size not match error flag bit
      参数:        FMC_FLAG_WPERR: FMC Erase/Program protection error flag bit
      参数:        FMC_FLAG_OPERR: FMC operation error flag bit
      参数:        FMC_FLAG_END: FMC end of operation flag bit
    参数[输出]:  无
    返回值:     FlagStatus: SET or RESET
*/
FlagStatus FMC_flag_get(uint32_t FMC_flag) {
    if(FMC_STAT & FMC_flag) {
        return SET;
    }

    /* return the state of corresponding FMC flag */
    return RESET;
}

/*!
    简介:    clear the FMC pending flag
    参数[输入]:  FMC_flag: clear FMC flag
                only one parameter can be selected which is shown as below:
      参数:        FMC_FLAG_RDDERR: FMC read D-bus protection error flag bit
      参数:        FMC_FLAG_PGSERR: FMC program sequence error flag bit
      参数:        FMC_FLAG_PGMERR: FMC program size not match error flag bit
      参数:        FMC_FLAG_WPERR: FMC erase/program protection error flag bit
      参数:        FMC_FLAG_OPERR: FMC operation error flag bit
      参数:        FMC_FLAG_END: FMC end of operation flag bit
    参数[输出]:  无
    返回值:      无
*/
void FMC_flag_clear(uint32_t FMC_flag) {
    /* clear the flags */
    FMC_STAT = FMC_flag;
}

/*!
    简介:    enable FMC interrupt
    参数[输入]:  FMC_int: the FMC interrupt source
                only one parameter can be selected which is shown as below:
      参数:        FMC_INT_END: enable FMC end of program interrupt
      参数:        FMC_INT_ERR: enable FMC error interrupt
    参数[输出]:  无
    返回值:      无
*/
void FMC_Interrupt_enable(uint32_t FMC_int) {
    FMC_CTL |= FMC_int;
}

/*!
    简介:    disable FMC interrupt
    参数[输入]:  FMC_int: the FMC interrupt source
                only one parameter can be selected which is shown as below:
      参数:        FMC_INT_END: disable FMC end of program interrupt
      参数:        FMC_INT_ERR: disable FMC error interrupt
    参数[输出]:  无
    返回值:      无
*/
void FMC_Interrupt_disable(uint32_t FMC_int) {
    FMC_CTL &= ~(uint32_t)FMC_int;
}

/*!
    简介:    get FMC interrupt flag set or reset
    参数[输入]:  FMC_int_flag: FMC interrupt flag
                only one parameter can be selected which is shown as below:
      参数:        FMC_INT_FLAG_RDDERR: FMC read D-bus protection error interrupt flag
      参数:        FMC_INT_FLAG_PGSERR: FMC program sequence error interrupt flag
      参数:        FMC_INT_FLAG_PGMERR: FMC program size not match error interrupt flag
      参数:        FMC_INT_FLAG_WPERR: FMC Erase/Program protection error interrupt flag
      参数:        FMC_INT_FLAG_OPERR: FMC operation error interrupt flag
      参数:        FMC_INT_FLAG_END: FMC end of operation interrupt flag
    参数[输出]:  无
    返回值:     FlagStatus: SET or RESET
*/
FlagStatus FMC_Interrupt_flag_get(uint32_t FMC_int_flag) {
    if(FMC_FLAG_END == FMC_int_flag) {
        /* end of operation interrupt flag */
        if(FMC_CTL & FMC_CTL_ENDIE) {
            if(FMC_STAT & FMC_int_flag) {
                return SET;
            }
        }
    } else {
        /* error interrupt flags */
        if(FMC_CTL & FMC_CTL_ERRIE) {
            if(FMC_STAT & FMC_int_flag) {
                return SET;
            }
        }
    }

    return RESET;
}

/*!
    简介:    clear the FMC interrupt flag
    参数[输入]:  FMC_int_flag: FMC interrupt flag
                only one parameter can be selected which is shown as below:
      参数:        FMC_INT_FLAG_RDDERR: FMC read D-bus protection error interrupt flag
      参数:        FMC_INT_FLAG_PGSERR: FMC program sequence error interrupt flag
      参数:        FMC_INT_FLAG_PGMERR: FMC program size not match error interrupt flag
      参数:        FMC_INT_FLAG_WPERR: FMC Erase/Program protection error interrupt flag
      参数:        FMC_INT_FLAG_OPERR: FMC operation error interrupt flag
      参数:        FMC_INT_FLAG_END: FMC end of operation interrupt flag
    参数[输出]:  无
    返回值:      无
*/
void FMC_Interrupt_flag_clear(uint32_t FMC_int_flag) {
    /* clear the interrupt flag */
    FMC_STAT = FMC_int_flag;
}

/*!
    简介:    get the FMC state
    参数[输入]:  无
    参数[输出]:  无
    返回值:     state of FMC
      参数:        FMC_READY: the operation has been completed
      参数:        FMC_BUSY: the operation is in progress
      参数:        FMC_RDDERR: read D-bus protection error
      参数:        FMC_PGSERR: program sequence error
      参数:        FMC_PGMERR: program size not match error
      参数:        FMC_WPERR: erase/program protection error
      参数:        FMC_OPERR: operation error
*/
FMC_state_enum FMC_state_get(void) {
    FMC_state_enum FMC_state = FMC_READY;
    uint32_t temp_val = FMC_STAT;

    if(RESET != (temp_val & FMC_FLAG_BUSY)) {
        FMC_state = FMC_BUSY;
    } else if(RESET != (temp_val & FMC_FLAG_RDDERR)) {
        FMC_state = FMC_RDDERR;
    } else if(RESET != (temp_val & FMC_FLAG_PGSERR)) {
        FMC_state = FMC_PGSERR;
    } else if(RESET != (temp_val & FMC_FLAG_PGMERR)) {
        FMC_state = FMC_PGMERR;
    } else if(RESET != (temp_val & FMC_FLAG_WPERR)) {
        FMC_state = FMC_WPERR;
    } else if(RESET != (temp_val & FMC_FLAG_OPERR)) {
        FMC_state = FMC_OPERR;
    } else {
        FMC_state = FMC_READY;
    }

    /* return the FMC state */
    return FMC_state;
}

/*!
    简介:    check whether FMC is ready or not
    参数[输入]:  无
    参数[输出]:  无
    返回值:     state of FMC
      参数:        FMC_READY: the operation has been completed
      参数:        FMC_BUSY: the operation is in progress
      参数:        FMC_RDDERR: read D-bus protection error
      参数:        FMC_PGSERR: program sequence error
      参数:        FMC_PGMERR: program size not match error
      参数:        FMC_WPERR: erase/program protection error
      参数:        FMC_OPERR: operation error
      参数:        FMC_TOERR: timeout error
*/
FMC_state_enum FMC_ready_Wait(uint32_t timeout) {
    FMC_state_enum FMC_state = FMC_BUSY;

    /* wait for FMC ready */
    do {
        /* get FMC state */
        FMC_state = FMC_state_get();
        timeout--;
    } while((FMC_BUSY == FMC_state) && (0U != timeout));

    if(0U == timeout) {
        FMC_state = FMC_TOERR;
    }

    /* return the FMC state */
    return FMC_state;
}
