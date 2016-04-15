#
# Makefile to make the file libclientserver.a, containing
# connection.o and server.o
#
# Define the compiler. g++ can be
# changed to clang++.
CXX = g++
CC  = g++

# Define preprocessor, compiler, and linker flags. Uncomment the # lines
# if you use clang++ and wish to use libc++ instead of libstdc++.
CPPFLAGS =  -std=c++11
CPPFLAGS += -I./headers

CXXFLAGS =  -O2 -Wall -Wextra -pedantic-errors -Wold-style-cast 
CXXFLAGS += -Wmissing-braces -Wparentheses 
CXXFLAGS += -std=c++11 
CXXFLAGS += -g

LDFLAGS   = -g -L./lib

VPATH = src/
BUILDDIR = src/


#CXXFLAGS =  -stdlib=libc++
#CPPFLAGS =  -stdlib=libc++
#CXXFLAGS += -stdlib=libc++

# Libraries
LDLIBS = -lclientserver

# Targets
PROGS = projserverdb client


all: libclientserver.a $(PROGS)

# Create the library; ranlib is for Darwin (OS X) and maybe other systems.
# Doesn't seem to do any damage on other systems.

libclientserver.a: ./src/connection.o ./src/server.o 
	ar rv lib/libclientserver.a  src/connection.o src/server.o
	ranlib lib/libclientserver.a

projserverdb: ./src/projserverdb.o ./src/messageHandlerdb.o ./src/database.o ./src/newsgroup.o ./src/article.o

client: ./src/client.o ./src/messageHandlerdb.o ./src/database.o ./src/newsgroup.o ./src/article.o


# Phony targets
.PHONY: all clean

# Standard clean
clean:
	rm -f *.o libclientserver.a
	rm -f *.o $(PROGS)

# Generate dependencies in *.d files


%.d: %.cc
	@set -e; rm -f $@; \
         $(CPP) -MM $(CPPFLAGS) $< > $@.$$$$; \
         sed 's,\($*)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
         rm -f $@.$$$$

# Include the *.d files
SRC = $(wildcard *.cc)
include $(SRC:.cc=.d)

all: $(PROGS)
	mv projserverdb bin/
	mv client bin/
