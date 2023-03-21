/*!
    文件:    gd32f4xx_fwdgt.c
    简介:   FWDGT driver

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

#include "gd32f4xx_fwdgt.h"

/*!
    简介:    enable write access to FWDGT_PSC and FWDGT_RLD
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void fwdgt_write_enable(void) {
    FWDGT_CTL = FWDGT_WRITEACCESS_ENABLE;
}

/*!
    简介:    disable write access to FWDGT_PSC and FWDGT_RLD
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void fwdgt_write_disable(void) {
    FWDGT_CTL = FWDGT_WRITEACCESS_DISABLE;
}

/*!
    简介:    start the free watchdog timer counter
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void fwdgt_enable(void) {
    FWDGT_CTL = FWDGT_KEY_ENABLE;
}

/*!
    简介:      configure the free watchdog timer counter prescaler value
    参数[输入]:  prescaler_value: specify prescaler value
                only one parameter can be selected which is shown as below:
      参数:        FWDGT_PSC_DIV4: FWDGT prescaler set to 4
      参数:        FWDGT_PSC_DIV8: FWDGT prescaler set to 8
      参数:        FWDGT_PSC_DIV16: FWDGT prescaler set to 16
      参数:        FWDGT_PSC_DIV32: FWDGT prescaler set to 32
      参数:        FWDGT_PSC_DIV64: FWDGT prescaler set to 64
      参数:        FWDGT_PSC_DIV128: FWDGT prescaler set to 128
      参数:        FWDGT_PSC_DIV256: FWDGT prescaler set to 256
    参数[输出]:  无
    返回值:     ErrStatus: ERROR or SUCCESS
*/
ErrStatus fwdgt_prescaler_value_config(uint16_t prescaler_value) {
    uint32_t timeout = FWDGT_PSC_TIMEOUT;
    uint32_t flag_status = RESET;

    /* enable write access to FWDGT_PSC */
    FWDGT_CTL = FWDGT_WRITEACCESS_ENABLE;

    /* wait until the PUD flag to be reset */
    do {
        flag_status = FWDGT_STAT & FWDGT_STAT_PUD;
    } while((--timeout > 0U) && ((uint32_t)RESET != flag_status));

    if ((uint32_t)RESET != flag_status) {
        return ERROR;
    }

    /* configure FWDGT */
    FWDGT_PSC = (uint32_t)prescaler_value;

    return SUCCESS;
}

/*!
    简介:      configure the free watchdog timer counter reload value
    参数[输入]:  reload_value: specify reload value(0x0000 - 0x0FFF)
    参数[输出]:  无
    返回值:     ErrStatus: ERROR or SUCCESS
*/
ErrStatus fwdgt_reload_value_config(uint16_t reload_value) {
    uint32_t timeout = FWDGT_RLD_TIMEOUT;
    uint32_t flag_status = RESET;

    /* enable write access to FWDGT_RLD */
    FWDGT_CTL = FWDGT_WRITEACCESS_ENABLE;

    /* wait until the RUD flag to be reset */
    do {
        flag_status = FWDGT_STAT & FWDGT_STAT_RUD;
    } while((--timeout > 0U) && ((uint32_t)RESET != flag_status));

    if ((uint32_t)RESET != flag_status) {
        return ERROR;
    }

    FWDGT_RLD = RLD_RLD(reload_value);

    return SUCCESS;
}

/*!
    简介:    reload the counter of FWDGT
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void fwdgt_counter_reload(void) {
    FWDGT_CTL = FWDGT_KEY_RELOAD;
}

/*!
    简介:    configure counter reload value, and prescaler divider value
    参数[输入]:  reload_value: specify reload value(0x0000 - 0x0FFF)
    参数[输入]:  prescaler_div: FWDGT prescaler value
                only one parameter can be selected which is shown as below:
      参数:        FWDGT_PSC_DIV4: FWDGT prescaler set to 4
      参数:        FWDGT_PSC_DIV8: FWDGT prescaler set to 8
      参数:        FWDGT_PSC_DIV16: FWDGT prescaler set to 16
      参数:        FWDGT_PSC_DIV32: FWDGT prescaler set to 32
      参数:        FWDGT_PSC_DIV64: FWDGT prescaler set to 64
      参数:        FWDGT_PSC_DIV128: FWDGT prescaler set to 128
      参数:        FWDGT_PSC_DIV256: FWDGT prescaler set to 256
    参数[输出]:  无
    返回值:     ErrStatus: ERROR or SUCCESS
*/
ErrStatus fwdgt_config(uint16_t reload_value, uint8_t prescaler_div) {
    uint32_t timeout = FWDGT_PSC_TIMEOUT;
    uint32_t flag_status = RESET;

    /* enable write access to FWDGT_PSC,and FWDGT_RLD */
    FWDGT_CTL = FWDGT_WRITEACCESS_ENABLE;

    /* wait until the PUD flag to be reset */
    do {
        flag_status = FWDGT_STAT & FWDGT_STAT_PUD;
    } while((--timeout > 0U) && ((uint32_t)RESET != flag_status));

    if ((uint32_t)RESET != flag_status) {
        return ERROR;
    }

    /* configure FWDGT */
    FWDGT_PSC = (uint32_t)prescaler_div;

    timeout = FWDGT_RLD_TIMEOUT;

    /* wait until the RUD flag to be reset */
    do {
        flag_status = FWDGT_STAT & FWDGT_STAT_RUD;
    } while((--timeout > 0U) && ((uint32_t)RESET != flag_status));

    if ((uint32_t)RESET != flag_status) {
        return ERROR;
    }

    FWDGT_RLD = RLD_RLD(reload_value);

    /* reload the counter */
    FWDGT_CTL = FWDGT_KEY_RELOAD;

    return SUCCESS;
}

/*!
    简介:    get flag state of FWDGT
    参数[输入]:  flag: flag to get
                only one parameter can be selected which is shown as below:
      参数:        FWDGT_STAT_PUD: a write operation to FWDGT_PSC register is on going
      参数:        FWDGT_STAT_RUD: a write operation to FWDGT_RLD register is on going
    参数[输出]:  无
    返回值:     FlagStatus: SET or RESET
*/
FlagStatus fwdgt_flag_get(uint16_t flag) {
    if(RESET != (FWDGT_STAT & flag)) {
        return SET;
    }

    return RESET;
}
