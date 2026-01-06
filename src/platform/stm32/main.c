/***************************************************************************//**
 *   @file   main.c
 *   @brief  ADE9430 basic bring-up example for the STM32 platform.
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

#include <inttypes.h>
#include <stdio.h>

#include "no_os_error.h"
#include "no_os_uart.h"
#include "no_os_spi.h"
#include "ade9430.h"
#include "parameters.h"

/**
 * @brief Helper that triggers a phase measurement and prints the raw data.
 */
static int ade9430_print_phase(struct ade9430_dev *dev,
			       enum ade9430_phase phase,
			       const char *label)
{
	int ret = ade9430_read_data_ph(dev, phase);

	if (ret) {
		printf("Failed to read %s phase data (err=%d)\r\n", label, ret);
		return ret;
	}

	printf("%s Phase\r\n", label);
	printf("  Current: %" PRIu32 " (raw)\r\n", dev->irms_val);
	printf("  Voltage: %" PRIu32 " (raw)\r\n", dev->vrms_val);
	printf("  Power:   %" PRIu32 " (raw)\r\n", dev->watt_val);

	return 0;
}

/**
 * @brief Application entry point.
 */
int main(void)
{
	int ret;
	struct no_os_uart_desc *uart = NULL;
	struct ade9430_dev *ade9430_dev = NULL;
	uint32_t run_val = 0, ep_cfg = 0, egy_time = 0;

	stm32_init();

	ret = no_os_uart_init(&uart, &ade9430_uart_ip);
	if (ret)
		return ret;

	no_os_uart_stdio(uart);
	printf("\r\nADE9430 + STM32 basic example\r\n");

	ret = ade9430_init(&ade9430_dev, ade9430_ip);
	if (ret) {
		printf("ADE9430 initialization failed! (err=%d)\r\n", ret);
		goto cleanup;
	}

	ret = ade9430_set_egy_model(ade9430_dev,
				     ADE9430_EGY_NR_SAMPLES,
				     ADE9430_EGY_ACCUM_SAMPLES);
	if (ret) {
		printf("Energy model configuration failed! (err=%d)\r\n", ret);
		goto cleanup;
	}

	ret = ade9430_read_temp(ade9430_dev);
	if (ret) {
		printf("Temperature read failed! (err=%d)\r\n", ret);
		goto cleanup;
	}
	printf("Temperature: %" PRId32 " C\r\n", ade9430_dev->temp_deg);

	printf("\r\n********** Raw Phase Values *************\r\n");
	ret = ade9430_print_phase(ade9430_dev, ADE9430_PHASE_A, "Phase A");
	if (ret)
		goto cleanup;
	ret = ade9430_print_phase(ade9430_dev, ADE9430_PHASE_B, "Phase B");
	if (ret)
		goto cleanup;
	ret = ade9430_print_phase(ade9430_dev, ADE9430_PHASE_C, "Phase C");
	if (ret)
		goto cleanup;

	ade9430_read(ade9430_dev, ADE9430_REG_RUN, &run_val);
	ade9430_read(ade9430_dev, ADE9430_REG_EP_CFG, &ep_cfg);
	ade9430_read(ade9430_dev, ADE9430_REG_EGY_TIME, &egy_time);

	printf("\r\nDebug Registers:\r\n");
	printf("RUN Register:      %" PRIu32 "\r\n", run_val);
	printf("EP_CFG Register:   0x%08" PRIX32 "\r\n", ep_cfg);
	printf("EGY_TIME Register: %" PRIu32 "\r\n", egy_time);

cleanup:
	if (ade9430_dev)
		ade9430_remove(ade9430_dev);
	if (uart)
		no_os_uart_remove(uart);

	return ret;
}
