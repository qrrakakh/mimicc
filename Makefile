CFLAGS=-std=c11 -g -static
SRCS=codegen.c container.c main.c parse.c lex.c
OBJS=$(SRCS:.c=.o)

mimicc: $(OBJS)
	$(CC) -o mimicc $(OBJS) $(LDFLAGS)

$(OBJS): mimicc.h

test: mimicc
	./test.sh

clean:
	rm -f mimicc *.o *~ tmp*

.PHONY: test clean