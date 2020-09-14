##############
### MACRO
##############
#$(1) Compiler
#$(2) Object file to generate
#$(3) Source file
#$(4) Dependencies
#$(5) Compiler flags
define COMPILE
$(2) : $(3) $(4)
	$(1) -c -o $(2) $(3) $(5)
endef

#$(1) Source file
define C2O
$(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(patsubst $(SRC)%,$(OBJ)%,$(1))))
endef

#$(1) Source file
define C2H
$(patsubst %.c,%.h,$(patsubst %.cpp,%.hpp,$(1)))
endef

##############
### CONFIG
##############
APP 	:= game
CCFLAGS	:= $(CFLAGS) -std=c++17
CFLAGS	:= -Wall -pedantic
CC 		:= g++
C 		:= gcc
MKDIR 	:= mkdir -p
SRC 	:= src
OBJ 	:= obj
LIBDIR	:= lib
LIB 	:= $(LIBDIR)/picoPNG/libpicopng.a $(LIBDIR)/tinyPTC/libtinyptc.a -lX11 -lXext
INCDIR	:= -I$(SRC) -I$(LIBDIR)

ifdef DEBUG
	CCFLAGS += -g
else
	CCFLAGS += -O3
endif

ALLCPPS 	:= $(shell find src/ -type f -iname *.cpp)
ALLCS 		:= $(shell find src/ -type f -iname *.c)
ALLOBJ 		:= $(foreach FILE,$(ALLCPPS) $(ALLCS),$(call C2O,$(FILE)))
SUBDIRS 	:= $(shell find $(SRC) -type d)
OBJSUBDIRS 	:= $(patsubst $(SRC)%,$(OBJ)%,$(SUBDIRS))

.PHONY: info, libs, libs-clean, libs-cleanall

$(APP) : $(OBJSUBDIRS) $(ALLOBJ)
	$(CC) -o $(APP) $(ALLOBJ) $(LIB) $(CCFLAGS)

$(foreach FILE,$(ALLCPPS),$(eval $(call COMPILE,$(CC),$(call C2O,$(FILE)),$(FILE),$(call C2H,$(FILE)),$(CCFLAGS) $(INCDIR))))
$(foreach FILE,$(ALLCS),$(eval $(call COMPILE,$(C),$(call C2O,$(FILE)),$(FILE),$(call C2H,$(FILE)),$(CFLAGS) $(INCDIR))))

info :
	$(info $(SUBDIRS))
	$(info $(OBJSUBDIRS))
	$(info $(ALLCPPS))
	$(info $(ALLCS))
	$(info $(ALLCSOBJ))
 
$(OBJSUBDIRS) : 
	$(MKDIR) $(OBJSUBDIRS)

clean:
	rm -r "./$(OBJ)"

cleanall: clean
	rm "./$(APP)"

##Lib makefiles
libs:
	$(MAKE) -C $(LIBDIR)

libs-clean:
	$(MAKE) -C $(LIBDIR) clean

libs-cleanall:
	$(MAKE) -C $(LIBDIR) cleanall