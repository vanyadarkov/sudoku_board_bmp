CC = gcc

CFLAGS = -Wall -Wextra

SRCS = src/sudoku.c src/write_digit.c src/cJSON.c

OBJS = $(SRCS:.c=.o)

MAIN = sudoku

.PHONY: clean

all: build

run: build
	./$(MAIN)

build: $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)
	
.c.o:
	$(CC) $(CFLAGS) -c $<  -o $@
	
clean:
	$(RM) src/*.o *~ $(MAIN)
