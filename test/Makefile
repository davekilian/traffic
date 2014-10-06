
# Output
#
TARGET = ../runtests

# Libraries
#
INCLUDES = -I.. -I../lib

LIBS = -L.. -ltraffic

# Sources
#
HEADERS = ../traffic.h		\
		  test.h			\
		  ../lib/list.h 	\
		  ../lib/hash.h 	\
		  ../lib/set.h 		\
		  ../lib/vector.h 	\
		  ../lib/network.h	\
		  ../lib/node.h 	\
		  ../lib/iface.h 	\
		  ../lib/link.h 	\
		  ../lib/conf.h		\

OBJECTS = main.o					\
		  vector.o					\
		  list.o					\
		  hash.o					\
		  set.o						\
          ../lib/err.o 				\
		  ../lib/util/memory.o 		\
		  ../lib/util/list.o 		\
		  ../lib/util/vector.o 		\
		  ../lib/util/hash.o		\
		  ../lib/util/set.o 		\
		  ../lib/network/create.o 	\
		  ../lib/network/uniqueid.o	\
		  ../lib/network/model.o 	\
		  ../lib/node/create.o 		\
		  ../lib/node/model.o		\

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

uninstall:
