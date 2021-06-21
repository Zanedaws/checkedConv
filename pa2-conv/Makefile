GCC = clang -Wall -Wshadow -Wvla -pedantic -g
VAL = valgrind --leak-check=full

SRCS = pa2.c hcomp.c
OBJS = $(SRCS:%.c=%.o)

pa2: $(OBJS)
	$(GCC) $(OBJS) -o pa2
	
.c.o:
	$(GCC) -c $*.c

testmemory: pa2
	$(VAL) --log-file=memcheck1.txt ./pa2 original/lorum freqOutput1 codeOutput1 headerOutput1 compOutput1

testall: test1 test2 test3 test4 test5
	

test1: pa2
	./pa2 original/gophers freqOutput1 codeOutput1 headerOutput1 compOutput1
	diff freqOutput1 count/gophers.count

test2: pa2
	./pa2 original/binary1 freqOutput1 codeOutput1 headerOutput1 compOutput1
	diff freqOutput1 count/binary1.count

test3: pa2
	./pa2 original/woods freqOutput1 codeOutput1 headerOutput1 compOutput1
	diff freqOutput1 count/woods.count

test4: pa2
	./pa2 original/stone freqOutput1 codeOutput1 headerOutput1 compOutput1
	diff freqOutput1 count/stone.count

test5: pa2
	./pa2 original/lorum freqOutput1 codeOutput1 headerOutput1 compOutput1
	diff freqOutput1 count/lorum.count

test6: pa2
	./pa2 original/empty freqOutput1 codeOutput1 headerOutput1 compOutput1
	diff freqOutput1 count/empty.count
	
clean: # remove all machine generated files
	rm -f *.o freqOutput? codeOutput? headerOutput? compOutput? memcheck?.txt
	
zip:
	zip pa2.zip *.c *.h Makefile
