CC = gcc
CFLAGS = -O1 -Wall -std=c99 -Wno-missing-braces -I include\ -L lib\ -lraylib -lopengl32 -lgdi32 -lwinmm

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

all: output

output: $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) -o main.exe

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o output

rebuild: clean all

.PHONY : clean
.SILENT : clean
