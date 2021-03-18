#
# Copyright (c) 2005-2009 Instigate CJSC, Armenia
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free
# Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#

# The source files under the project.
cpp_files := \
	misc.c \
	stm32f10x_gpio.c \
	stm32f10x_flash.c \
	stm32f10x_rcc.c \
	stm32f10x_adc.c \
	stm32f10x_bkp.c \
	stm32f10x_can.c \
	stm32f10x_cec.c \
	stm32f10x_crc.c \
	stm32f10x_dac.c \
	stm32f10x_dbgmcu.c \
	stm32f10x_dma.c \
	stm32f10x_exti.c \
	stm32f10x_fsmc.c \
	stm32f10x_i2c.c \
	stm32f10x_iwdg.c \
	stm32f10x_pwr.c \
	stm32f10x_rtc.c \
	stm32f10x_sdio.c \
	stm32f10x_spi.c \
	stm32f10x_tim.c \
	stm32f10x_usart.c \
	stm32f10x_wwdg.c \
	system_stm32f10x.c \

asm_files := \

# The header files under the project, which are supposed to be used by other
# projects.
public_headers := \
	misc.h \
	stm32f10x_gpio.h \
	stm32f10x_flash.h \
	stm32f10x_rcc.h \
	stm32f10x_wwdg.h \
	stm32f10x_adc.h \
	stm32f10x_can.h \
	stm32f10x_cec.h \
	stm32f10x_bkp.h \
	stm32f10x_crc.h \
	stm32f10x_dac.h \
	stm32f10x_dbgmcu.h \
	stm32f10x_exti.h \
	stm32f10x_dma.h \
	stm32f10x_fsmc.h \
	stm32f10x_i2c.h \
	stm32f10x_iwdg.h \
	stm32f10x_pwr.h \
	stm32f10x_rtc.h \
	stm32f10x_sdio.h \
	stm32f10x_spi.h \
	stm32f10x_tim.h \
	stm32f10x_usart.h \
	stm32f10x.h \
	system_stm32f10x.h \
	stm32f10x_conf.h \

# Building QT project.
# TODO: Add detailed comments.
qt_headers :=

moc_flags :=

qt_ui_files :=

uic_flags :=

# The compiler flags can be updated/overwritten for the project.
# 
# Example:
# compiler_flags += -s -Wno-long-long -fno-strict-aliasing
# compiler_flags := $(filter-out -Wshadow -Wconversion, $(compiler_flags))
# 
# Note: It is not recommended to overwrite the value (don't use := operator).
# 
compiler_flags +=
# The linker flags can be updated/overwritten for the project
#
# Example:
# linker_flags += -i 
#
# Note: It is not recommended to overwrite the value (don't use := operator).
#
linker_flags +=

# The libraries which should be linked to the current project. The link type is
# specified as a global parameter, except the libraries, which are built as a
# either shared or static library. For those libraries the appropriate linking
# type is selected, e.g. if current project is being linked to lib_project,
# which is static library, then current project will be always linked to
# lib_project statically, despite global link type is shared or static.
# 
# Example:
# libs := project1 tcl gtkmm-2.4 
# 
# Note: Specifying the library version is not supported.
#
libs := CMSIS

# Library version
#
# Note: If the library version is not specified, then the program_version is
# taken for this library.
#
lib_version :=

# For each project either executable file or a library can be generated.
#
# To generate the executable file the exe.mk should be included, which is
# located in mkf directory.
#
# Two types of libraries can be generated for each project:
#       dynamic (shared objects) and static (archives).
#
# To generate the shared object the dynamic.mk should be included here.
# To generate the archive the static.mk file should be included here.
# To generate either (shared object or archive) depending on the global link
# type the lib.mk should be included.
#
# Note that the generated library will be located in local $lib_dir directory
# (by default $(project_root)/lib/).
#

include $(mkf_path)/lib.mk
