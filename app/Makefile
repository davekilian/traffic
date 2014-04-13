
# Output
#
TARGET = ../traffic

# Libraries
#
INCLUDES = -I..

LIBS = -L.. -ltraffic

# Sources
#
HEADERS = ../traffic.h

OBJECTS = main.o

# Flags
#
DEBUGFLAGS = -g -Wall
CFLAGS = -std=c99 $(DEBUGFLAGS)
LDFLAGS = 

# Plumbing
# http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
# 
CC = gcc

all: $(TARGET)

%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS) $(INCLUDES)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)

clean:
	rm -f $(OBJECTS)

distclean:
	rm -f $(OBJECTS) $(TARGET)

install: $(TARGET)
	cp $(TARGET) /usr/bin

uninstall:
	rm /usr/bin/traffic
