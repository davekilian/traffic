
# Platform detection
#
ifeq ($(shell uname),Darwin)
	so=dylib
else
	so=so
endif

# Output
#
TARGET = ../libtraffic.$(so)

# Libraries
#
INCLUDES = -I.. -I.

LIBS = 

# Sources
#
HEADERS = ../traffic.h \
		  list.h \
		  hash.h \
		  vector.h \
		  network.h \
		  node.h \
		  iface.h \
		  link.h \
		  conf.h

OBJECTS = err.o \
		  util/memory.o \
		  util/list.o \
		  util/vector.o \
		  util/hash.o \
		  util/set.o

# Flags
#
DEBUGFLAGS = -g -Wall
CFLAGS = -std=c99 -fpic -fno-common $(DEBUGFLAGS)
LDFLAGS = 

# Plumbing
# http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
# 
CC = gcc

all: $(TARGET)

%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS) $(INCLUDES)

$(TARGET): $(OBJECTS)
	$(CC) -shared -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)

clean:
	rm -f $(OBJECTS)

distclean:
	rm -f $(OBJECTS) $(TARGET)

install: $(TARGET) $(HEADERS)
	cp $(TARGET) /usr/lib
	cp ../traffic.h /usr/include

uninstall:
	rm /usr/lib/libtraffic.$(so)
	rm /usr/include/traffic.h

