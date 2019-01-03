#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = d_meters

# --------------------------------------------------------------
# Files to build

FILES_DSP = \
	ExamplePluginMeters.cpp

FILES_UI  = \
	ExampleUIMeters.cpp

# --------------------------------------------------------------
# Do some magic

include ../../Makefile.plugins.mk

# --------------------------------------------------------------
# Enable all possible plugin types

ifeq ($(HAVE_DGL),true)
ifeq ($(HAVE_JACK),true)
TARGETS += jack
endif
endif

ifeq ($(HAVE_DGL),true)
TARGETS += lv2_sep
endif

TARGETS += vst

all: $(TARGETS)

# --------------------------------------------------------------
