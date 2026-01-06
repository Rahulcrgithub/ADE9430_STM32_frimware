# ADE9430 STM32 Demo (Nucleo-F413ZH)

Minimal no-OS firmware that brings up the ADE9430 three-phase energy metering AFE
on an STM32 Nucleo-F413ZH board. The firmware talks to the ADE9430 over SPI1 and
prints measurements on UART2 so they are visible on the ST-Link VCOM port.

## What it does
- Initializes STM32 HAL, SPI1, and UART2.
- Initializes the ADE9430 driver and configures the energy accumulation window.
- Reads and prints:
  - Temperature (C).
  - Raw phase measurements for A/B/C: current (IRMS), voltage (VRMS), and power (WATT).
  - Debug registers: RUN, EP_CFG, and EGY_TIME.

Note: The phase values are raw register codes (not converted to SI units). You can
apply scaling/calibration if you need real-world units.

## Hardware
- STM32 Nucleo-F413ZH
- ADE9430 evaluation board (or equivalent hardware with SPI access)
- USB cable for ST-Link (debug + UART2 VCOM)
- Jumper wires

## Wiring (Nucleo-F413ZH -> ADE9430)
| Function | ADE9430 Signal | STM32 Pin |
| --- | --- | --- |
| SPI SCK | SCLK | PA5 (CN10 D13) |
| SPI MOSI | SDI | PA7 (CN10 D11) |
| SPI MISO | SDO | PA6 (CN10 D12) |
| Chip Select | CS | PB12 (CN12 D23) |
| UART2 TX (console) | RX on USB/UART | PA2 (CN10 D1) |
| UART2 RX (optional) | TX from host | PA3 (CN10 D0) |
| GND | GND | GND |
| 3V3 | VDD | 3V3 |

Reset/IRQ/power rails should be wired per the ADE9430 evaluation board guide.

## Build (Windows + MSYS2 MINGW64)
1. Install STM32CubeIDE and STM32CubeMX.
2. Set environment variables in your MSYS2 MINGW64 shell:
   ```bash
   export STM32CUBEIDE="C:/ST/STM32CubeIDE"
   export STM32CUBEMX="C:/ST/STM32CubeMX"
   ```
3. From this repository root:
   ```bash
   make PLATFORM=stm32 HARDWARE=carrier/nucleo-f413zh-ade9430.ioc
   ```

The build generates the HAL project from the `.ioc` file, then links a single ELF.

## Flashing
- CLI: `make PLATFORM=stm32 HARDWARE=carrier/nucleo-f413zh-ade9430.ioc run`
- Or open the generated project in STM32CubeIDE and flash the ELF.

## UART output
Connect to the ST-Link virtual COM port at `115200 8N1`.
Example output:
```text
ADE9430 + STM32 basic example
Temperature: 32 C

********** Raw Phase Values *************
Phase A
  Current: 123456 (raw)
  Voltage: 654321 (raw)
  Power:   111111 (raw)
Phase B
  ...
Phase C
  ...

Debug Registers:
RUN Register:      7
EP_CFG Register:   0x00000001
EGY_TIME Register: 7999
```

## Configuration notes
- SPI speed: adjust `ADE9430_SPI_BAUDRATE` in `src/platform/stm32/parameters.h`.
- Chip select pin: update the CS GPIO in `src/platform/stm32/parameters.h` and the
  CubeMX `.ioc` file if you rewire.
- Accumulation samples: change `ADE9430_EGY_ACCUM_SAMPLES` in
  `src/platform/stm32/parameters.h`.

## Project layout
- `src/platform/stm32/main.c`: application logic and prints.
- `src/platform/stm32/parameters.h/.c`: SPI/UART descriptors and ADE9430 init.
- `carrier/nucleo-f413zh-ade9430.ioc`: CubeMX hardware definition.
- `Makefile` / `src.mk`: build entry points.

