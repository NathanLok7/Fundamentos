CC = gcc
CFLAGS = -Wall -Wextra -pthread -lm
TARGET = mercator

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)
