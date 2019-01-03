#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = midimetermon

# --------------------------------------------------------------
# Files to build

FILES_DSP = \
	MidiMeterMonPlugin.cpp

FILES_UI  = \
	MidiMeterMonUI.cpp
# --------------------------------------------------------------
# Do some magic

include ../../Makefile.plugins.mk

# --------------------------------------------------------------
# Enable all possible plugin types

# ifeq ($(LINUX),true)
# TARGETS += jack
# endif

# TARGETS += lv2_dsp
# TARGETS += vst

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
