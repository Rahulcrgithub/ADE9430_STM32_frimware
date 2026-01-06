HARDWARE ?= carrier/nucleo-f413zh-ade9430.ioc

ifeq ($(OS),Windows_NT)
PROJECT := $(shell cygpath -m "$(CURDIR)")
NO-OS := $(shell cygpath -m "$(CURDIR)/../..")
endif

include ../../tools/scripts/generic_variables.mk

override INCLUDE := $(NO-OS)/include

include src.mk

include ../../tools/scripts/generic.mk
