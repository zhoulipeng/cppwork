CC = g++
CFLAGS = -Wall -Wextra -ggdb
LIBS = -lcrypto
SRC = aes_file.c crypto_ssl.c

TARGET = main

.PHONY: all file test clean

all: file


file:
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LIBS)

test:
	./$(TARGET)

clean:
	rm $(TARGET) $(TARGET)

