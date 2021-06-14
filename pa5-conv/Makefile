GCC = clang -g -std=c99 -Wall -Wshadow -Wvla -pedantic -O3
VAL = valgrind -s --leak-check=full --leak-check=full --show-leak-kinds=all --track-origins=yes

SRCS = pa5.c sorting.c
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

testall: test1 test2 test3 test4 test5 test6 test7 test8

test1: pa5
	./pa5 -q 7.b output1

test2: pa5
	./pa5 -q 1M.b outputM

test3: pa5
	./pa5 -q 100.b output3

test4: pa5
	./pa5 -q 10.b output4

test5: pa5
	./pa5 -q 10s.b output5

test6: pa5
	./pa5 -q 30.b output5

test7: pa5
	./pa5 -q 5.b output5

test8: pa5
	./pa5 -q 100s.b output5

clean: # remove all machine generated files
	rm -f *.o memcheck?.txt output?
	
zip:
	zip pa5.zip *.c *.h Makefile 
