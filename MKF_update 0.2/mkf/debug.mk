#
# Copyright © 2005-2019 Instigate CJSC, Armenia
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

ifneq ($(strip $(build_type)),debug)
$(error Error: build_type is set to 'release')
endif

$(call check_variable,project_root)
$(call check_variable,bin_dir)
$(call check_variable,mkf_path)
dbg_level:= -d2
dbg_file := /usr/local/share/openocd/scripts/interface/stlink-v2.cfg

simulation_flags := -M lm3s6965evb -S -gdb tcp::1234 -nographic -kernel
ifeq ($(mcu),STM32F1X)	
mcu_file := /usr/local/share/openocd/scripts/target/stm32f1x.cfg
simulation_flags += -cpu cortex-m3
ifeq ($(mcu),STM32F4X)
simulation_flags += -cpu cortex-m4
mcu_file := /usr/local/share/openocd/scripts/target/stm32f4x.cfg
ifeq ($(mcu),STM32L1)
simulation_flags += -cpu cortex-m3
mcu_file := /usr/local/share/openocd/scripts/target/stm32l1.cfg
else ($(error "Not stated board name"))
endif
endif
endif

gdb_file := $(project_root)/utils/debug.gdb
gdb_sim := $(project_root)/utils/debug_sim.gdb

ifeq ($(words $(MAKECMDGOALS)), 1)
$(call check_variable,flash_bin)
else
ifeq ($(words $(MAKECMDGOALS)), 2)
flash_bin := $(basename $(lastword $(MAKECMDGOALS)))
else
$(error "Error: 'make flash' accepts only one argument")
endif
endif

elf := $(project_root)/$(bin_dir)/$(flash_bin)
bin := $(project_root)/$(bin_dir)/$(flash_bin).bin

.PHONY: sim 
sim:
	@if [ ! -f $(dbg_file) -o ! -f $(mcu_file) ]; then \
		echo "Error: OpenOCD config file does not exists" \
	fi
	@if [ ! -f $(elf) ]; then \
		echo "Error: File '$(elf)' doesn't exists " \
		exit 1;\
	fi
	@gnome-terminal -x sh -c "sleep 2; \
	arm-none-eabi-gdb -d $(project_root)/src/main/ -x $(gdb_sim) $(elf)"
	#TODO
	qemu-system-arm -M lm3s6965evb $(simulation_flags) -S -gdb tcp::1234 -nographic -kernel $(elf)

.PHONY: debug
debug:
	@if [ ! -f $(dbg_file) -o ! -f $(mcu_file) ]; then \
		echo "Error: OpenOCD config file does not exists" \
		"(see mkf/debug.mk file) "; \
		exit 1;\
	fi
	@if [ ! -f $(elf) ]; then \
		echo "Error: File '$(elf)' doesn't exists " \
		exit 1;\
	fi
	@gnome-terminal -x sh -c "sleep 2; \
	arm-none-eabi-gdb -d $(project_root)/src/main/ -x $(gdb_file) $(elf) -tui "
	@openocd $(dbg_level) -f $(dbg_file) -f $(mcu_file)
	@#arm-none-eabi-objdump -x bin/main # for disassemblink
