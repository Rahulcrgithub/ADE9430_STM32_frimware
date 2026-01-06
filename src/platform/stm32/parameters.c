/***************************************************************************//**
 *   @file   parameters.c
 *   @brief  STM32 specific parameter definitions for the ADE9430 example.
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

#include <stdbool.h>
#include "parameters.h"
#include "no_os_uart.h"
#include "no_os_spi.h"

struct stm32_uart_init_param ade9430_uart_extra_ip = {
	.huart = &huart2,
	.timeout = 1000U,
};

struct no_os_uart_init_param ade9430_uart_ip = {
	.device_id = ADE9430_UART_DEVICE_ID,
	.irq_id = ADE9430_UART_IRQ_ID,
	.asynchronous_rx = false,
	.baud_rate = ADE9430_UART_BAUDRATE,
	.size = NO_OS_UART_CS_8,
	.parity = NO_OS_UART_PAR_NO,
	.stop = NO_OS_UART_STOP_1_BIT,
	.platform_ops = ADE9430_UART_OPS,
	.extra = &ade9430_uart_extra_ip,
};

struct stm32_spi_init_param ade9430_spi_extra_ip = {
	.chip_select_port = ADE9430_SPI_CS_PORT,
	.get_input_clock = HAL_RCC_GetPCLK2Freq,
};

struct no_os_spi_init_param ade9430_spi_ip = {
	.device_id = ADE9430_SPI_DEVICE_ID,
	.max_speed_hz = ADE9430_SPI_BAUDRATE,
	.chip_select = ADE9430_SPI_CS_PIN,
	.mode = NO_OS_SPI_MODE_0,
	.bit_order = NO_OS_SPI_BIT_ORDER_MSB_FIRST,
	.platform_ops = ADE9430_SPI_OPS,
	.extra = &ade9430_spi_extra_ip,
};

struct ade9430_init_param ade9430_ip = {
	.spi_init = &ade9430_spi_ip,
	.temp_en = true,
};
