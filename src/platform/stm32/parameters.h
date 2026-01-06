/***************************************************************************//**
 *   @file   parameters.h
 *   @brief  STM32 platform specific definitions for the ADE9430 example.
 ********************************************************************************
 * Copyright 2025(c) Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  - Neither the name of Analog Devices, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *  - The use of this software may or may not infringe the patent rights
 *    of one or more patent holders.  This license does not release you
 *    from the requirement that you obtain separate licenses from these
 *    patent holders to use this software.
 *  - Use of the software either in source or binary form, must be run
 *    on or directly connected to an Analog Devices Inc. component.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ANALOG DEVICES BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************/
#ifndef __ADE9430_PARAMETERS_H__
#define __ADE9430_PARAMETERS_H__

#include "stm32_hal.h"
#include "stm32_spi.h"
#include "stm32_uart.h"
#include "stm32_uart_stdio.h"
#include "ade9430.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Helper defines for the GPIO port identifiers expected by the STM32 drivers. */
#define GPIO_PORT_A 0U
#define GPIO_PORT_B 1U

#define ADE9430_UART_DEVICE_ID  2U
#define ADE9430_UART_BAUDRATE   115200U
#define ADE9430_UART_IRQ_ID     USART2_IRQn
#define ADE9430_UART_OPS        &stm32_uart_ops

#define ADE9430_SPI_DEVICE_ID   1U
#define ADE9430_SPI_BAUDRATE    1000000U
#define ADE9430_SPI_CS_PIN      12U
#define ADE9430_SPI_CS_PORT     GPIO_PORT_B
#define ADE9430_SPI_OPS         &stm32_spi_ops

#define ADE9430_EGY_ACCUM_SAMPLES   7999U

extern UART_HandleTypeDef huart2;

extern struct stm32_uart_init_param ade9430_uart_extra_ip;
extern struct no_os_uart_init_param ade9430_uart_ip;
extern struct stm32_spi_init_param ade9430_spi_extra_ip;
extern struct no_os_spi_init_param ade9430_spi_ip;
extern struct ade9430_init_param ade9430_ip;

#ifdef __cplusplus
}
#endif

#endif /* __ADE9430_PARAMETERS_H__ */
