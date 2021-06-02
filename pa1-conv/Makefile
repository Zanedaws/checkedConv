WARNING = -Wall -Wshadow --pedantic
ERROR = -Wvla
CLANG = clang -g $(WARNING) $(ERROR) 
VAL = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes

TESTFLAGS =

SRCS = pa1.c shell_array.c
OBJS = $(SRCS:%.c=%.o)

pa1: $(OBJS)
	$(CLANG) $(TESTFLAGS) $(OBJS) -o pa1
	
.c.o:
	$(CLANG) $(TESTFLAGS) -c $*.c

clean:
	rm -f *.o output?? errlog pa1

test15: pa1
	./pa1 -a examples/15.b output15
