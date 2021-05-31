GCC = gcc -g -std=c99 -Wall -Wshadow -Wvla -pedantic -O3
VAL = valgrind -s --leak-check=full --leak-check=full --show-leak-kinds=all --track-origins=yes

SRCS = pa5.c
OBJS = $(SRCS:%.c=%.o)

pa5: $(OBJS)
	$(GCC) $(OBJS) -o pa5
	
.c.o:
	$(GCC) -c $*.c

testmemory: pa5
	$(VAL) --log-file=memcheck0.txt ./pa5 -q 100.b output1
	# $(VAL) --log-file=memcheck1.txt ./pa5 -b examples/ops1.b treeOutput1
	# $(VAL) --log-file=memcheck2.txt ./pa5 -b examples/ops2.b treeOutput1
	# $(VAL) --log-file=memcheck3.txt ./pa5 -b examples/ops3.b treeOutput1

testall: test1 test2 test3 test4 test5 test6 test7 test9 test10 test11 test12

test1: pa5
	./pa5 -q 7.b output1

testM: pa5
	./pa5 -q 1M.b outputM

testM2: pa5
	./pa5 -q 1.5M.b outputM

testB: pa5
	./pa5 -q 1B.b outputB

clean: # remove all machine generated files
	rm -f *.o memcheck?.txt output?
	
zip:
	zip pa5.zip *.c *.h Makefile 
