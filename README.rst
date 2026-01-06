ADE9430 STM32 Demo
===================

This project provides a minimal no-OS example that brings up the ADE9430 three-phase
energy metering AFE on the STM32 Nucleo-F413ZH development board.  The firmware uses
SPI1 to communicate with the ADE9430 and routes all console prints through UART2 so
that measurement results are available on the Arduino-compatible header (pins PA2/PA3).

Hardware connections
--------------------

+----------------------+---------------------------+-------------------------------+
| Function             | ADE9430 board signal      | STM32 Nucleo-F413ZH pin       |
+======================+===========================+===============================+
| SPI SCK              | SCLK                      | PA5  (CN10 D13)               |
+----------------------+---------------------------+-------------------------------+
| SPI MOSI             | SDI                       | PA7  (CN10 D11)               |
+----------------------+---------------------------+-------------------------------+
| SPI MISO             | SDO                       | PA6  (CN10 D12)               |
+----------------------+---------------------------+-------------------------------+
| Chip Select          | CS                        | PB12 (CN12 D23)               |
+----------------------+---------------------------+-------------------------------+
| UART2 TX (console)   | RX input on USB/UART pod  | PA2  (CN10 D1)                |
+----------------------+---------------------------+-------------------------------+
| UART2 RX (optional)  | TX output from host       | PA3  (CN10 D0)                |
+----------------------+---------------------------+-------------------------------+

The ADE9430 reset, IRQ and power rails should be wired according to the evaluation board
user guide.  Chip select can be moved to a different GPIO by editing
``src/platform/stm32/parameters.h``.

Building
--------

1. Install STM32CubeIDE and STM32CubeMX, then export ``STM32CUBEIDE`` and ``STM32CUBEMX``
   environment variables so they point to the installations (see ``tools/scripts/stm32.mk``).
2. From the project directory run:

.. code-block:: bash

   make PLATFORM=stm32 HARDWARE=carrier/nucleo-f413zh-ade9430.ioc

The build system generates the HAL project from the supplied ``.ioc`` file, compiles the
no-OS sources and links them into a single ELF that can be flashed with the ``make run``
helper or directly from STM32CubeIDE.

UART output appears on USART2 (PA2) at 115200-8-N-1 and prints the ADE9430 temperature
as well as raw RMS/power values for phases A/B/C.
