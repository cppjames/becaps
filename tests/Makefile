TARGET := main
CFLAGS := -c -I../

.PHONY: all clean

all: $(TARGET)
	@./$(TARGET)

$(TARGET): main.o vector.o stream.o
	gcc $^ -o $@

%.o: %.c
	gcc $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(TARGET)
