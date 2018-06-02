CC=$$armcc
CFLAGS=-Wall -Wextra -Wformat=2 -Wundef -Wpointer-arith -Wcast-align\
		-Wstrict-prototypes -Wwrite-strings -Wswitch-default -Wswitch-enum\
		-pedantic -std=c11 -g

OBJDIR=obj
SRCDIR=src
BDIR=build

SRC=$(wildcard $(SRCDIR)/*.c)
# Gets all c files in src and turns them into .o and .d files in obj/
OBJ=$(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
DEP=$(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.d)

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
	rm obj/* && rm build/* 

-include $(DEP)
