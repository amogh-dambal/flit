# ------------------------------------------------
# flit Project Makefile
# author: amogh dambal
# email: amoghdambal@utexas.edu
# sourced by makefile from: https://stackoverflow.com/questions/7004702/how-can-i-create-a-makefile-for-c-projects-with-src-obj-and-bin-subdirectories
# ------------------------------------------------

## executable build

# project name (generate executable with this name)
TARGET   = flit
CC       = gcc
# compiling flags here
CFLAGS   = -std=c11 -Wall -I. -Wextra -pedantic -fsanitize=address

LINKER   = gcc
# linking flags here
LFLAGS   = -Wall -I. -lm -fsanitize=address

# file directories
SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin
INCDIR 	 = inc

# files
SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(INCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm       = rm -f

## test build
TTARGET = flittest
TCC     = gcc

# compiler flags
TCFLAGS = -std=c11 -Wall -I. -Wextra -pedantic -fsanitize=address

# linker
TLINKER = gcc

# linker flags
TLFLAGS = -Wall -I. -lm -fsanitize-address

# test file directories
TESTDIR = test

# files
TSOURCES 	:= $(wildcard $(TESTDIR)/*.c)
TINCLUDES 	:= $(wildcard $(INCDIR)/*.h)
TOBJECTS 	:= $(TSOURCES:$(TESTDIR)/%.c=$(OBJDIR)/%.o)

.PHONY: test
test:
	$(TCC) $(TCFLAGS) -o $(BINDIR)/$(TTARGET) $(TESTDIR)/test-main.c
	./$(BINDIR)/$(TTARGET)

$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $(OBJECTS) $(LFLAGS) -o $@
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

.PHONY: clean
clean:
	@$(rm) $(OBJECTS)
	@echo "Cleanup complete!"

.PHONY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
	@$(rm) $(BINDIR)/$(TTARGET)
	@echo "Executable removed!"