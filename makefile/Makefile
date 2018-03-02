CC = gcc
LINK = gcc
CFLAGS = -g -pipe -Wall -Werror -Wextra -Wshadow
INCPATH = -Iinclude/ \
		  -I../common/libevent-2.0.22/include/ \
		  -I../common/libfastcommon-1.0.7/src/
LIBS = -L../common/libevent-2.0.22/.libs -levent -Wl,-rpath=../common/libevent-2.0.22/.libs \
	   -L../common/libfastcommon-1.0.7/src -lfastcommon -Wl,-rpath=../common/libfastcommon-1.0.7/src
DEFS =
BUILD = .tmp

SOURCES = $(wildcard *.c)
OBJECTS = $(patsubst %.c, $(BUILD)/%.o, $(notdir $(SOURCES)))
TARGET = LiveEMS

first: all

$(BUILD)/%.o: %.c
	$(CC) -c $(CFLAGS) $(DEFS) $(INCPATH) -o $@ $<

all: $(BUILD) $(TARGET) 

$(BUILD):
	@mkdir -p $(BUILD)

$(TARGET) : $(OBJECTS)
	$(LINK) -o $(TARGET) $(CFLAGS) $(DEFS) $(INCPATH) $(OBJECTS) $(LIBS)

clean :
	rm -rf $(OBJECTS) $(TARGET) $(BUILD)
