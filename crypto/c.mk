CC = g++
INCPATH = -I../build/local/include/

CFLAGS = -Wall -Wextra -ggdb
LIBS =-lm -lcrypto
SRC = aes_file.c crypto_ssl.c


TARGET = main

.PHONY: all file test clean

all: file


file:
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LIBS) $(INCPATH)

test:
	./$(TARGET)

clean:
	rm $(TARGET) $(TARGET)

