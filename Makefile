CFLAGS=-std=c11 -g -static -Wall -Wno-switch
SRCS=codegen.c container.c main.c parse.c lex.c
OBJS=$(SRCS:.c=.o)
gen2-tmp=./build-gen2

mimicc: $(OBJS)
	$(CC) -o mimicc $(OBJS) $(LDFLAGS)

$(OBJS): mimicc.h

test: mimicc
	./tests/test.sh ./mimicc

build-gen2: mimicc
	$(gen2-tmp)/build-gen2.sh

test-gen2: build-gen2
	./tests/test.sh ./mimicc-gen2

clean:
	rm -f mimicc *.o *~ tmp*
	rm -f mimicc-gen2 $(gen2-tmp)/*.o $(gen2-tmp)/*.s $(gen2-tmp)/*.c

.PHONY: test clean