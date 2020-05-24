CFLAGS=-std=c11 -g -static -fno-common

nncc: main.o
	$(CC) -o $@ $? $(LDFLAGS)

test: nncc
	./test.sh

clean:
	rm -f nncc *.o *~ tmp*

.PHONY: test clean
