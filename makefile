CC=$$armcc
CFLAGS=-Wall -Wformat -pedantic -std=c11

OBJDIR=obj
SRCDIR=src
BDIR=build

OBJ=$(addprefix $(OBJDIR)/, $(patsubst $(SRCDIR)/%.c, %.o, $(wildcard $(SRCDIR)/*.c)))

$(BDIR)/disp: $(OBJ) 
	$(CC) $^ -o $(BDIR)/disp

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

# TODO handle headers
