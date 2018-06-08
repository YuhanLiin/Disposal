CC:=$$armcc
CFLAGS:=-Wall -Wextra -Wformat=2 -Wundef -Wpointer-arith -Wcast-align\
		-Wstrict-prototypes -Wwrite-strings -Wswitch-default -Werror\
		-pedantic -std=c11 -g -Isrc
DEPFLAGS:=-MMD -MP -MT '$@'

OBJDIR:=obj
SRCDIR:=src
TSTDIR:=src/test
BDIR:=build

SRC:=$(wildcard $(SRCDIR)/*.c)
INC:=$(wildcard $(SRCDIR)/*.h)
# Gets all c files in src and turns them into .o and .d files in obj/
OBJ:=$(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
DEP:=$(OBJ:$(OBJDIR)/%.o=$(OBJDIR)/%.d)

SRC_TST:=$(wildcard $(TSTDIR)/*.c)
INC_TST:=$(wildcard $(TSTDIR)/*.h)
# Object files for tests share the obj/ directory with object files for sources,
# and have tst_ prefix
OBJ_TST:=$(SRC_TST:$(TSTDIR)/%.c=$(OBJDIR)/tst_%.o)
DEP_TST:=$(OBJ_TST:$(OBJDIR)/%.o=$(OBJDIR)/%.d)

PROG:=$(BDIR)/disp
TEST:=$(BDIR)/test

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

# Map all c files to obj file of same name with a rule for each.
# Compiler generates .d dependency for each obj file and puts it into same place
# as the obj file. The -MT flag ensures that the dependency files have the right targets.
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $(DEPFLAGS) $(CFLAGS) $< -o $@

$(OBJDIR)/tst_%.o: $(TSTDIR)/%.c
	$(CC) -c $(DEPFLAGS) '$@' $(CFLAGS) $< -o $@

# Makes it so dependency errors are suppressed when a .h file is deleted/renamed
$(INC) $(INC_TST):

# Clean all .o and .d files and the build directory
.PHONY: clean
clean:
	rm obj/* && rm build/* 

-include $(DEP) $(DEP_TST)

.DELETE_ON_ERROR:
