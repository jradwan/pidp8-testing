VPATH=/opt/pidp8/src

CC=gcc
CFLAGS=-std=c99 -U__STRICT_ANSI__  -Wno-unused-result -D_GNU_SOURCE -DUSE_READER_THREAD -DHAVE_DLOPEN=so -I . -I PDP8
DEPS = gpio.h
OBJ =  matrix.o gpio.o 
LIBS =  -lm -lrt -lpthread -ldl 


%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

matrix: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f *.o


