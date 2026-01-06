# ADE9430 STM32 Firmware Guide

## 1. Overview
This document explains the structure, configuration, build steps, and runtime behavior of the `projects/ade9430_stm32` firmware. The example targets the STM32 Nucleo-F413ZH board and demonstrates how to communicate with the ADE9430 three-phase energy metering AFE over SPI while streaming measurements through UART2.

## 2. Project Layout and File Roles
| File/Folder | Purpose |
|-------------|---------|
| `Makefile` | Entry point for the no-OS build system. Selects the STM32CubeMX hardware description and includes the generic rules. |
| `builds.json` | Preset used by CI/build_projects.py to build the STM32 configuration. |
| `README.rst` | Quick-start instructions and wiring summary. |
| `DOCUMENTATION.md` (this file) | Detailed design & usage notes. |
| `carrier/nucleo-f413zh-ade9430.ioc` | STM32CubeMX hardware description. Defines clocks and pins (SPI1 on PA5/PA6/PA7, PB12 CS, UART2 on PA2/PA3). Used by `tools/scripts/stm32.mk` to auto-generate HAL sources. |
| `src/platform/stm32/main.c` | Application: initializes STM32 HAL, UART2 stdio, ADE9430 driver, reads temperature and phase data, prints results. |
| `src/platform/stm32/parameters.h/.c` | Platform glue: describes UART2 and SPI1 descriptors, chip-select GPIO, and the ADE9430 init structure. |
| `src/platform/stm32/platform_src.mk` | Adds STM32 driver sources (GPIO, SPI, UART, DMA…) to the build. |
| `src.mk` | Pulls in the STM32 sources plus the ADE9430 driver and core no-OS utilities. |

## 3. Software & Hardware Requirements
### Hardware
- STM32 Nucleo-F413ZH board.
- ADE9430 evaluation board (or equivalent hardware with SPI access).
- USB cable for ST-Link debug and UART2 console (PA2 routed to ST-Link virtual COM).
- Jumper wires for SPI signals and ground.

### Software
- Git clone of the Analog Devices no-OS repository (this project resides inside it).
- STM32CubeIDE and STM32CubeMX installed locally.
  - Export `STM32CUBEIDE=/path/to/stm32cubeide`
  - Export `STM32CUBEMX=/path/to/stm32cubemx`
  - These variables are mandatory on both Windows (PowerShell: `$env:STM32CUBEIDE=...`) and Linux (`export STM32CUBEIDE=...`).
- Make (GNU make) and Python (already required by no-OS build scripts).

## 4. Wiring Guide (Nucleo-F413ZH ? ADE9430)
| ADE9430 Signal | STM32 Pin | Notes |
|----------------|-----------|-------|
| SCLK | PA5 (CN10 D13) | Configured as SPI1_SCK in CubeMX. |
| MISO | PA6 (CN10 D12) | SPI1_MISO. |
| MOSI | PA7 (CN10 D11) | SPI1_MOSI. |
| CS | PB12 (CN12 D23) | Configured as GPIO output driving ADE9430 CS. |
| GND | GND | Common reference between boards. |
| VDD | 3V3 | Match ADE9430 logic levels. |
| UART TX (STM32?PC) | PA2 | USART2_TX. Connect to the host’s RX to read logs (already tied to ST-Link VCOM). |
| UART RX (PC?STM32) | PA3 | Not required unless interactive input is needed. |

Additional ADE9430 control pins (reset, IRQ) should follow the evaluation board user guide if used; this firmware only requires SPI and power rails.

## 5. Implementation Details
### 5.1 `parameters.c/.h`
- Defines `struct stm32_uart_init_param ade9430_uart_extra_ip` pointing at `huart2` (CubeMX generates this handle). UART is configured for 115200 baud, 8-N-1, blocking TX.
- Defines `struct stm32_spi_init_param ade9430_spi_extra_ip` with chip-select GPIO port `GPIO_PORT_B` (PB12) and `HAL_RCC_GetPCLK2Freq` so the SPI driver can compute prescalers.
- `ade9430_spi_ip` ties everything together: SPI1 (`device_id = 1`), CPOL/CPHA = 0, MSB first, `max_speed_hz = 1_000_000` (1 MHz). That value informs the driver’s `no_os_spi_init` call.
- `ade9430_ip` (driver init) references the SPI descriptor and sets `.temp_en = true`, enabling temperature conversions.

### 5.2 `main.c`
1. Calls `stm32_init()` (generated HAL init, provided by no-OS STM32 glue) to configure clocks, GPIO, and HAL handles.
2. Initializes UART2 via `no_os_uart_init` using the descriptors from `parameters.c`, then redirects `printf` via `no_os_uart_stdio`.
3. Initializes ADE9430 driver with `ade9430_init(&ade9430_dev, ade9430_ip)`. This function uses the no-OS SPI layer and performs chip ID checks and register configuration.
4. Calls `ade9430_set_egy_model(... ADE9430_EGY_NR_SAMPLES, 7999)` so the accumulation window matches the Linux reference (7999 samples).
5. Reads temperature with `ade9430_read_temp`, prints in Celsius.
6. Calls helper `ade9430_print_phase` three times, which invokes `ade9430_read_data_ph` for each phase (A/B/C) and prints raw current, voltage, and watt readings (register values, not converted into SI units).
7. Reads debug registers (`RUN`, `EP_CFG`, `EGY_TIME`) via `ade9430_read` for verification.
8. On any failure, jumps to cleanup label, prints the error, and tears down SPI/UART resources via `ade9430_remove` / `no_os_uart_remove`.

### 5.3 SPI Transactions
- The no-OS ADE9430 driver uses `no_os_spi_write_and_read` under the hood. Each read/write is 24-bit command + 32-bit data per register (see `drivers/meter/ade9430/ade9430.c`).
- With `max_speed_hz = 1 MHz`, the STM32 SPI driver (`drivers/platform/stm32/stm32_spi.c`) selects the lowest prescaler that doesn’t exceed 1 MHz, based on the APB2 clock frequency (96 MHz in the CubeMX file ? prescaler 128, giving 750 kHz). Adjust `ADE9430_SPI_BAUDRATE` to push higher rates if desired.

## 6. ADE9430 Initialization Flow
1. `ade9430_init` – resets the device via SPI, reads `ADE9430_REG_CHIP_ID` (expect 0x63), sets up configuration registers for measurements, and caches descriptor pointer.
2. `ade9430_set_egy_model` – writes sample count to `ADE9430_REG_EGY_TIME`. Passing 7999 matches the Linux example.
3. `ade9430_read_temp` – sets `ADE9430_TEMP_EN`, triggers conversion, polls status, then stores the temperature in `dev->temp_deg` (°C).
4. `ade9430_read_data_ph` – for each phase constant, reads VRMS/IRMS/WATT registers and stores them in the device struct (displayed later).

## 7. Build Instructions
### 7.1 Common Steps
1. Ensure repository path has no spaces (no-OS requirement).
2. From repository root, run `git submodule update --init --recursive` if needed.
3. Install STM32CubeMX & STM32CubeIDE and set environment variables.

### 7.2 Linux
```bash
export STM32CUBEIDE=/opt/stm32cubeide
export STM32CUBEMX=/opt/stm32cubemx
cd /path/to/no-OS
make PLATFORM=stm32 HARDWARE=carrier/nucleo-f413zh-ade9430.ioc
```
- The first build automatically launches CubeMX headless mode to generate HAL sources inside `projects/ade9430_stm32/build/app`.
- Artifacts:
  - `projects/ade9430_stm32/build/app/Release/ade9430_stm32.elf`
  - OpenOCD scripts: `.openocd` and `.openocd-cmsis`

### 7.3 Windows (PowerShell)
```powershell
$env:STM32CUBEIDE="C:\ST\STM32CubeIDE"
$env:STM32CUBEMX="C:\ST\STM32CubeMX"
cd D:\Aigelon\AI-driven Edge device\Firmware\no-OS-main
make PLATFORM=stm32 HARDWARE=carrier/nucleo-f413zh-ade9430.ioc
```
- Requirements: `make` available (e.g., via MSYS2 or the tools packaged with STM32CubeIDE). Ensure Python is accessible for the build scripts.

## 8. Flashing the Firmware
Two options once the `.elf` is built:
1. **CLI** – `make PLATFORM=stm32 HARDWARE=carrier/nucleo-f413zh-ade9430.ioc run`
   - Uses the generated `.openocd` script to connect to the Nucleo’s ST-Link and program the binary.
2. **STM32CubeIDE** – Import the generated project (`projects/ade9430_stm32/build/app`) and use the IDE’s Run/Debug buttons to flash `ade9430_stm32.elf`.

## 9. Observing Output
- Connect to the ST-Link virtual COM port (listed as “STMicroelectronics STLink Virtual COM Port”) at 115200-8-N-1.
- Typical session:
```
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
- Raw readings depend on the attached sensors and calibration.

## 10. Customization Notes
- **SPI speed** – change `ADE9430_SPI_BAUDRATE` in `parameters.h`. After rebuilding, the SPI driver selects a prescaler that stays under the requested speed.
- **Chip-select pin** – update `ADE9430_SPI_CS_PIN`/`PORT` plus the CubeMX `.ioc` file if the hardware wiring differs.
- **Accumulation samples** – adjust `ADE9430_EGY_ACCUM_SAMPLES`. Larger windows smooth readings; smaller values respond faster.
- **Output format** – modify `ade9430_print_phase` to convert raw codes to physical units using ADE9430 RMS constants if needed.

## 11. Implementation Methodology Summary
1. Reused the Linux ADE9430 example logic and ported it to STM32 by leveraging the no-OS platform drivers (`stm32_spi`, `stm32_uart`).
2. Created STM32CubeMX hardware definition to satisfy `tools/scripts/stm32.mk`, which generates HAL startup files and ensures `stm32_init()` is available.
3. Provided wiring/documentation so the board-to-board connections follow the header layout of the Nucleo-F413ZH.
4. Ensured firmware remains minimal: no RTOS or networking dependencies; only SPI + UART drivers and the ADE9430 library are included.

## 12. Troubleshooting Tips
- **CubeMX/IDE not found** – verify the environment variables; `make` stops with “not defined” messages if paths are wrong.
- **Build fails at CubeMX step** – ensure Java is available (CubeMX bundles its own JRE, but on some systems you must allow execution).
- **OpenOCD connection issues** – check that no other debugger (CubeIDE) holds the ST-Link; try power-cycling the board.
- **No UART output** – confirm the ST-Link VCOM driver is installed and the terminal is opened at 115200 baud before reset. Ensure `no_os_uart_stdio` succeeds (it prints nothing if UART init fails).

---
For any changes (new pins, SPI speed, or different STM32 board), update the `.ioc` file accordingly, rerun the build to regenerate HAL sources, and adjust `parameters.h` macros to match the new wiring.
