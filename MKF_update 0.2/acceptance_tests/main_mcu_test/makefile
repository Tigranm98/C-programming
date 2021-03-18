#
# Copyright (c) 2005-2015 Instigate CJSC
#

tst_src_dir := $(shell pwd)

tst_name := $(notdir $(tst_src_dir))

tst_root_dir := $(tst_src_dir)/../..

tst_res_dir := $(tst_root_dir)/$(acceptance_tests_results_dir)/$(tst_name)

final_test_result := $(tst_res_dir)/result.txt

shell_bin :=  $(tst_root_dir)/$(bin_dir)/main_mcu_test

golden_outputs := $(wildcard *.gout)

ifeq ($(golden_outputs),)
error_case:
	@echo "FAIL: no *.gout files are found" > $(final_test_result)
else

test_result := $(patsubst %.gout, %.tr, $(golden_outputs))
test_result := $(addprefix $(tst_res_dir)/, $(test_result))

outputs := $(patsubst %.gout, %.out, $(golden_outputs))
outputs := $(addprefix $(tst_res_dir)/, $(outputs))

.PHONY: result
result : $(final_test_result)

$(final_test_result) : $(test_result)
	@rm -rf $@ 
	@touch $@
	@for A in $^; do \
		if [ -n "`cat $${A}`" ]; then \
			echo `cat $${A}` >> $@; \
		fi; \
	done
	@if [ -z "`cat $@`" ]; then \
		echo "PASS" > $@; \
	fi;

$(tst_res_dir)/%.out : $(shell_bin)
	@echo "running $(notdir $(shell_bin)) ..."
	$(MAKE) flash $(notdir $(shell_bin)) -C $(project_root)
	@echo "0" > $@

$(tst_res_dir)/%.tr : $(tst_res_dir)/%.out $(tst_src_dir)/%.gout
	@rm -f $@ $(basename $<).diff
	@touch $@
	@if [ -f $< ]; then \
		echo -n "comparing $(notdir $^) ... " ;\
		diff -iqu $^ > "$(basename $<).diff";\
		if [ -n "`cat $(basename $<).diff`" ]; then \
			echo "FAIL" > $@; \
		fi;\
		echo " done"; \
	else \
		echo "FAIL: $< file is missing" > $@; \
	fi;

endif
