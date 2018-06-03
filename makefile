CC=$$armcc
CFLAGS=-Wall -Wextra -Wformat=2 -Wundef -Wpointer-arith -Wcast-align\
		-Wstrict-prototypes -Wwrite-strings -Wswitch-default -Werror\
		-pedantic -std=c11 -g -Isrc

OBJDIR=obj
SRCDIR=src
TSTDIR=src/test
BDIR=build

SRC=$(wildcard $(SRCDIR)/*.c)
# Gets all c files in src and turns them into .o and .d files in obj/
OBJ=$(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
DEP=$(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.d)

SRC_TST=$(wildcard $(TSTDIR)/*.c)
# Object files for tests share the obj/ directory with object files for sources,
# and have tst_ prefix
OBJ_TST=$(SRC_TST:$(TSTDIR)/%.c=$(OBJDIR)/tst_%.o)
DEP_TST=$(SRC_TST:$(TSTDIR)/%.c=$(OBJDIR)/tst_%.d)

PROG=$(BDIR)/disp
TEST=$(BDIR)/test

######################################################################
# Top level
.PHONY: all
all: prog test

# Main program
.PHONY: prog
prog: $(PROG)
$(PROG): $(OBJ) 
	$(CC) $^ -o $(PROG)

# Test program
.PHONY: test
test: $(TEST)
$(TEST): $(OBJ_TST) $(filter-out $(OBJDIR)/main.o, $(OBJ))
	$(CC) $^ -o $(TEST)

# Map all c files to obj file of same name with a rule for each
# Compiler generates .d dependency for each c file and puts it into obj/
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c -MMD -MP -MT '$@' $(CFLAGS) $< -o $@

$(OBJDIR)/tst_%.o: $(TSTDIR)/%.c
	$(CC) -c -MMD -MP -MT '$@' $(CFLAGS) $< -o $@

# Clean all .o and .d files and the build directory
.PHONY: clean
clean:
	rm obj/* && rm build/* 

-include $(DEP) $(DEP_TST)

.DELETE_ON_ERROR:
