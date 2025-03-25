CC = gcc
CFLAGS = -Wall $(shell pkg-config --cflags gtk+-3.0)
LDFLAGS = $(shell pkg-config --libs gtk+-3.0)

SRC = main.c gui/gui.c gui/callbacks.c book_manager.c files.c utils/*.c
OBJ = $(SRC:.c=.o)

EXEC = book_manager

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

%.o: %.c
    $(CC) -c $< -o $@ $(CFLAGS)



clean:
	rm -f $(OBJ) $(EXEC)
