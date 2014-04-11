
# Platform detection
#
ifeq ($(shell uname),Darwin)
	so=dylib
else
	so=so
endif

# Output
#
TARGET = ../libsns.$(so)

# Libraries
#
INCLUDES = -I../inc

LIBS = 

# Sources
#
HEADERS = ../inc/sns.h \
		  ../inc/snserr.h

OBJECTS = err.o

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
	mkdir /usr/include/sns
	cp -r ../inc/* /usr/include/sns

uninstall:
	rm /usr/lib/libsns.$(so)
	rm -fr /usr/include/sns

