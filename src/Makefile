TARGET = podex

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

# redeclare rm to force removal
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
	@echo "Linking complete!"

# compile .o to podex
$(OBJECTS): $(ODIR)/%.o : $(SDIR)/%.cpp
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

# clean .o files
.PHONY: clean
clean:
	@$(rmdir) $(ODIR)
	@echo "Cleanup complete!"

# remove executable
.PHONY: remove
remove: clean
	@$(rmdir) $(BDIR)
	@echo "Executable directory removed!"
