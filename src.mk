include $(PROJECT)/src/platform/$(PLATFORM)/platform_src.mk

SRCS += $(PROJECT)/src/platform/$(PLATFORM)/main.c \
        $(PROJECT)/src/platform/$(PLATFORM)/parameters.c

INCS += $(PROJECT)/src/platform/$(PLATFORM)/parameters.h

INCS += $(INCLUDE)/no_os_delay.h     \
        $(INCLUDE)/no_os_error.h     \
        $(INCLUDE)/no_os_spi.h       \
        $(INCLUDE)/no_os_uart.h      \
        $(INCLUDE)/no_os_util.h      \
        $(INCLUDE)/no_os_units.h     \
        $(INCLUDE)/no_os_alloc.h     \
        $(INCLUDE)/no_os_gpio.h      \
        $(INCLUDE)/no_os_dma.h       \
        $(INCLUDE)/no_os_irq.h       \
        $(INCLUDE)/no_os_lf256fifo.h \
        $(DRIVERS)/meter/ade9430/ade9430.h

SRCS += $(DRIVERS)/api/no_os_spi.c   \
        $(DRIVERS)/api/no_os_uart.c  \
        $(DRIVERS)/api/no_os_gpio.c  \
        $(DRIVERS)/api/no_os_dma.c   \
        $(DRIVERS)/api/no_os_irq.c   \
        $(NO-OS)/util/no_os_alloc.c  \
        $(NO-OS)/util/no_os_util.c   \
        $(NO-OS)/util/no_os_list.c   \
        $(NO-OS)/util/no_os_mutex.c  \
        $(NO-OS)/util/no_os_units.c  \
        $(NO-OS)/util/no_os_lf256fifo.c \
        $(DRIVERS)/meter/ade9430/ade9430.c
