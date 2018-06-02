CC=$$armcc
CFLAGS=-Wall -Wformat -pedantic -std=c11

OBJDIR=obj
SRCDIR=src
BDIR=build

# Gets all c files in src and turns them into .o and .d files in obj/
OBJ=$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(wildcard $(SRCDIR)/*.c))
DEP=$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.d, $(wildcard $(SRCDIR)/*.c))

PROG=$(BDIR)/disp

######################################################################
# Top level
.PHONY: all
all: disp

# Main program
.PHONY: disp
disp: $(PROG)
$(PROG): $(OBJ) 
	$(CC) $^ -o $(PROG)

# Map all c files to obj file of same name with a rule for each
# Compiler generates .d dependency for each c file and puts it into obj/
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c -MMD -MP -MT '$@' $(CFLAGS) $< -o $@

# Clean all .o and .d files and the build directory
.PHONY: clean
clean:
	rm -r *.o *.d build/*

-include $(DEP)
