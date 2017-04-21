LIBPATH=/data/zhengxufei/local

CC=g++
LIB=
SHELL=/bin/sh
OBJS=
RANLIB=ranlib

INCLUDES= -I./
CPPFLAGS= -g -finline-functions -Wall -Winline -pipe
LDFLAGS= 
LIBFLAGS= 

EXEC=main

OUTPUT_PATH=./output

all: clean $(EXEC)

main: tc_base64.cpp main.cpp
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.cpp %.h 
	$(CC) $(CPPFLAGS) -c $< -o $@ $(INCLUDES)

%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@ $(INCLUDES)


clean:
	rm -f *.o $(EXEC)
	rm -rf $(OUTPUT_PATH)

