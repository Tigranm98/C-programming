#
# Copyright © 2013-2017 Instigate Robotics CJSC, Armenia
#

# The name of the project
# 
project_name := "ATS"

# The version of the project
#
project_version := 0.1

# The absolute path to the directory where the current project is located 
#
project_root := $(shell pwd)

# Install path prefix
#
prefix := $(HOME)

# The projects should be listed here
#
# Example:
# projects := src/project1 src/project2 #
projects := \
	src/debug \
	src/CMSIS \
	src/STM32F10x_StdPeriph_Driver \
	src/main \
	src/display \
	src/servo \
	src/MPU6050 \
	#src/blink_f1
	#src/FreeRTOS \
	#src/RELAY \

# Project name that should be flashed on MCU
flash_bin := main

# Project name that should be flashed on MCU
# STM32F1X
# STM32F2
# STM32F3
# STM32F4X
# STM32F7
# STM32L1
# STM32L2
mcu := STM32F1X

# Prerequisite tools
preconditions := \
	gcc \
	doxygen \
	ar \
	ln \
	pkg-config \
	lcov \
	arm-none-eabi-gcc \
	arm-none-eabi-objcopy \
	st-flash \
	openocd \
	qemu-system-arm \

# Prerequisite libraries
library_preconditions := \

# Tests which are run after each modification. 
#
continuous_tests := \
	#continuous_tests/static_two_outputs \

# Includes the tests to check the basic principles of the
# application.  Should be called after each modification to
# check the regression of application.
#
regression_tests := 

# Tests set which are run after working day
#
nightly_tests := 

# Tests set which are run after working week
#
weekly_tests := 

test_type := continuous_tests

# List of the projects to collect coverage data
#
coverage_projects := $(projects)

# Defined the path where the package will be installed
#
install_path := $(prefix)/$(project_name)/$(project_version)

# Directories which should be installed/packaged in release mode
#
export opt_package := mkf ReleaseNotes.pdf

# Directories which should be installed/packaged in debug mode
export dbg_package := $(opt_package)

# Run time search path, used by patchELF tool while installing the project.
export RPATHS :=

export PKG_CONFIG_PATH := $(project_root)/lib/pkgconfig:/usr/lib/pkgconfig:$(PKG_CONFIG_PATH)

# The path to the mkf should be assigned to this variable
#
mkf_path := $(project_root)/mkf

include $(mkf_path)/main.mk
include $(mkf_path)/doc.mk

# Dependencies between projects
#
#src/FreeRTOS: src/STM32F10x_StdPeriph_Driver
src/main: src/CMSIS src/STM32F10x_StdPeriph_Driver src/debug src/display src/servo src/MPU6050
src/STM32F10x_StdPeriph_Driver: src/CMSIS
src/debug: src/CMSIS src/STM32F10x_StdPeriph_Driver
src/display: src/CMSIS src/STM32F10x_StdPeriph_Driver
src/servo: src/CMSIS src/STM32F10x_StdPeriph_Driver
src/MPU6050: src/CMSIS src/STM32F10x_StdPeriph_Driver
