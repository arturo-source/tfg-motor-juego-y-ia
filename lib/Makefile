LIBSDIRS := $(shell find . -maxdepth 1 -mindepth 1 -type d)

.PHONY: all clean cleanall

all: 
	$(foreach DIR,$(LIBSDIRS),$(MAKE) -C $(DIR);)

clean: 
	$(foreach DIR,$(LIBSDIRS),$(MAKE) -C $(DIR) clean;)

cleanall: 
	$(foreach DIR,$(LIBSDIRS),$(MAKE) -C $(DIR) cleanall;)