CFLAGS := -c -I../

.PHONY: all clean

all: main
	@./main

main: main.o vector.o stream.o
	gcc $^ -o $@

%.o: %.c
	gcc $(CFLAGS) $< -o $@

clean:
	rm -rf *.o main
