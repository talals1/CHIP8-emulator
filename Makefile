# Guidance from https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

CLFAGS = -Wall -O -fstack-protector
DEPS = chip8.h
OBJ = main.o chip8.o


%.o: %.c $(DEPS)
	x86_64-w64-mingw32-gcc -c -o $@ $< $(CFLAGS)


main: $(OBJ)
	x86_64-w64-mingw32-gcc -o chip.exe $^ $(CFLAGS)
	rm -f $(OBJ)

linux:
	gcc  $(CLFAGS) -o chip8 main.c chip8.c
	rm -f $(OBJ)


clean:
	rm -f chip.exe $(OBJ)

