#
# Copyright © 2005-2010 Instigate CJSC, Armenia
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

$(call check_variable,project_root)
$(call check_variable,bin_dir)

ifeq ($(words $(MAKECMDGOALS)), 1)
$(call check_variable,flash_bin)
else
ifeq ($(words $(MAKECMDGOALS)), 2)
flash_bin := $(basename $(lastword $(MAKECMDGOALS)))
else
$(error "Error: 'make flash' accepts only one argument")
endif
endif

bin := $(project_root)/$(bin_dir)/$(flash_bin).bin

.PHONY: flash
flash:
	@if [ ! -f $(bin) ]; then \
		echo "Error: File '$(bin_dir)/$(flash_bin).bin' does not exists"; \
		exit 1;\
	fi
	st-flash --reset write $(bin) 0x8000000

.PHONY: erase
erase:
	st-flash erase

.PHONY: flash_jtag
flash_jtag:
	@if [ ! -f $(bin) ]; then \
		echo "Error: File '$(bin_dir)/$(flash_bin).bin' does not exists"; \
		exit 1;\
	fi
	openocd -f jtag/openocd.cfg -c "program $(main_bin_file) 0x08000000 verify reset exit"

# For suppressing error 'make flash *' case
.PHONY: $(flash_bin)
$(flash_bin):
	@echo ""

