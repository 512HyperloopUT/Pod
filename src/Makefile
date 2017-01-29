# setup

TARGET = podex
TPRINT = [$(TARGET)]:\t

SDIR = cpp
ODIR = obj
BDIR = bin
IDIR = include

CFlags = -Wall -O3
LDFlags =
libs =
libDir =

# COMPILE BASED ON THE PARAMETERS DEFINED ABOVE

# set up compiler options
inc := $(addprefix -I,$(IDIR))
libs := $(addprefix -l,$(libs))
libDir := $(addprefix -L,$(libDir))
CFlags += -c $(inc) $(libDir) $(libs)

# find all source files
sources := $(shell find $(SDIR) -name '*.cpp')
srcDirs := $(shell find . -name '*.cpp' -exec dirname {} \; | uniq)
objects := $(patsubst %.cpp,$(ODIR)/%.o,$(sources))

# using cpp
CC = $(CXX)

# delete folder recursively
rmf = rm -rf
print = echo -e "[$(TARGET)]:\t"

.phony: all clean buildrepo

# default target
all: $(BDIR)/$(TARGET) finish

$(BDIR)/$(TARGET): buildrepo $(objects)
	@mkdir -p `dirname $@`
	@echo "$(TPRINT)Linking $@..."
	@$(CC) $(objects) $(LDFlags) -o $@

$(ODIR)/%.o: %.cpp
	@echo "$(TPRINT)Generating dependencies for $<..."
	@$(call make-depend,$<,$@,$(subst .o,.d,$@))
	@echo "$(TPRINT)Compiling $<..."
	@$(CC) $(CFlags) $< -o $@

# remove object directory
clean:
	@$(rmf) *~
	@$(rmf) $(ODIR)

# clean and remove executable directory
remove: clean
	@$(rmf) $(BDIR)

# state that podex is up to date when make has finished
finish:
	@echo "$(TPRINT)podex is up to date"

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
