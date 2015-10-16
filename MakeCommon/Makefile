
ifeq "$(OPTION_LOADED)" ""
include MakeOption.txt
endif

module_App := $(dir $(wildcard ../Application/Makefile))
module_AppExt := $(dir $(wildcard ../AppExt/Makefile))
module_Lib := $(dir $(wildcard ../LIB/Makefile))
module_LibExt := $(dir $(wildcard ../LIBExt/Makefile))
# Find the first folder in ../Project/ that contains "Makefile"
module_Prj := $(dir $(firstword $(sort $(shell find ../Project -name \'Makefile\'))))

module_all := \
	$(module_App) \
	$(module_AppExt) \
	$(module_Lib) \
	$(module_LibExt) \
	$(module_Prj)

define copy_log
	@-cp $(module_App)log*.txt \
	$(module_AppExt)log*.txt \
	$(module_Lib)log*.txt \
	$(module_LibExt)log*.txt \
	$(module_Prj)log*.txt \
	.
endef

ifeq "$(ISOLATE_DEP)" "ON"
.PHONY: all rm_log $(module_all) clean rebuild debug release dep

all: rm_log
	@make dep
	@make debug
	@make release
	$(call copy_log)
else
.PHONY: all rm_log $(module_all) clean rebuild debug release

all: rm_log $(module_all)
	$(call copy_log)
endif

$(module_all):
	@$(MAKE) --directory=$@ $(MAKECMDGOALS)

rm_log:
	@-rm -f log*.txt

clean: rm_log $(module_all)

ifeq "$(ISOLATE_DEP)" "ON"
rebuild:
	@make clean
	@make dep
	@make debug
	@make release
	$(call copy_log)

dep: $(module_all)
else
rebuild: rm_log $(module_all)
	$(call copy_log)
endif

debug: rm_log $(module_all)
	$(call copy_log)

release: rm_log $(module_all)
	$(call copy_log)

rm_chk_log:
	@-rm -f chk*.txt

chk: rm_chk_log $(module_all)

chkclean: $(module_all)
