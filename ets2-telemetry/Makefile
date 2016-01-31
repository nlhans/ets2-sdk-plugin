shell = /bin/sh

CC=g++ # C compiler
CFLAGS=-fPIC -g -Wall # C flags
LDFLAGS= -shared  # linking flags
RM=rm -f  # rm command
TARGET_LIB=libets2plugin.so # target lib
INCDIRS=-I../scs_sdk_1_5/include/ -I../scs_sdk_1_5/include/common/ -I../scs_sdk_1_5/include/eurotrucks2/

SRCS = $(shell echo src/*.cpp) # source files 
OBJS = $(SRCS:.cpp=.o)

.PHONY: all
all: ${TARGET_LIB}

$(TARGET_LIB): $(OBJS)
	$(CC) ${LDFLAGS} -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCDIRS) -c $< -o $@

clean:
	$(RM) $(OBJS)
