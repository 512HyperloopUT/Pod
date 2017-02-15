# SETUP

# target name
TARGET = podex

# directories
SDIR = cpp
ODIR = obj
BDIR = bin
IDIR = include

# flags
CFlags = -Wall -O3 -std=c++11
LDFlags =
libs =
libDir =

# compiler
CC = g++
# src file extension
FEX = cpp

# COMPILE BASED ON THE PARAMETERS DEFINED ABOVE

# print standard
TPRINT = "[$(TARGET)]: "

# set up compiler options
inc := $(addprefix -I,$(IDIR))
libs := $(addprefix -l,$(libs))
libDir := $(addprefix -L,$(libDir))
CFlags += -c $(inc) $(libDir) $(libs)

# find all source files
sources := $(shell find $(SDIR) -name '*.$(FEX)')
srcDirs := $(shell find . -name '*.$(FEX)' -exec dirname {} \; | uniq)
objects := $(patsubst %.$(FEX),$(ODIR)/%.o,$(sources))

# delete folder recursively
rmf = rm -rf

.phony: all clean buildrepo

# default target
all: $(BDIR)/$(TARGET) finish

$(BDIR)/$(TARGET): buildrepo $(objects)
	@mkdir -p `dirname $@`
	@echo "$(TPRINT)Linking $@..."
	@$(CC) $(objects) $(LDFlags) -o $@

$(ODIR)/%.o: %.$(FEX)
	@echo "$(TPRINT)Generating dependencies for $<..."
	@$(call make-depend,$<,$@,$(subst .o,.d,$@))
	@echo "$(TPRINT)Compiling $<..."
	@$(CC) $(CFlags) $< -o $@

# remove object directory
clean:
	@$(rmf) **/*~
	@$(rmf) $(ODIR)

# clean and remove executable directory
remove: clean
	@$(rmf) $(BDIR)

exec: all
	@echo $(TPRINT)Starting $(TARGET)...
	@./$(BDIR)/$(TARGET)
	@echo $(TPRINT)Finished

# state that podex is up to date when make has finished
finish:
	@echo "$(TPRINT)$(TARGET) is up to date"

# create obj subdirectories for compiling
buildrepo:
	@$(call make-repo)

define make-repo
	for dir in $(srcDirs); \
	do \
		mkdir -p $(ODIR)/$$dir; \
	done
endef

# usage: $(call make-depend,src,obj,dependency)
define make-depend
  $(CC) -MM       \
        -MF $3    \
        -MP       \
        -MT $2    \
        $(CFlags) \
        $1
endef
