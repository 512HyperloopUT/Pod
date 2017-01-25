TARGET = podex
TPRINT = [$(TARGET)]:\t

# include directory
IDIR = ./include
# src directory
SDIR = ./cpp
# object directory
ODIR = ./obj
# binary directory
BDIR = ./bin

# cpp compiler flags
CC = g++
CFLAGS = -Wall -I$(IDIR)

# cpp linker flags
LINKER = g++ -o
LFLAGS = -Wall -I$(IDIR)

# add all .h and .cpp files
INCLUDE := $(wildcard $(IDIR)/*.h)
SOURCES := $(wildcard $(SDIR)/*.cpp)
OBJECTS := $(SOURCES:$(SDIR)/%.cpp=$(ODIR)/%.o)

# declare rmdir to remove nonempty directories
rmdir = rm -rf

# default target
all: init $(BDIR)/$(TARGET)

# init compiler directories
init:
	@mkdir -p $(ODIR)
	@mkdir -p $(BDIR)

# compile .cpp to .o
$(BDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $@ $(LFLAGS) $(OBJECTS)
	@echo "$(TPRINT)Linking "$@

# compile .o to podex
$(OBJECTS): $(ODIR)/%.o : $(SDIR)/%.cpp
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(TPRINT)Compiled "$<

# clean .o files
.PHONY: clean
clean:
	@$(rmdir) $(ODIR)
	@echo "$(TPRINT)Cleanup complete"

# remove executable
.PHONY: remove
remove: clean
	@$(rmdir) $(BDIR)
	@echo "$(TPRINT)Executable directory removed"
