LIBPATH=/data/zhengxufei/local
CC=g++
LIB=
SHELL=/bin/sh
OBJS=
RANLIB=ranlib

INCLUDES= -I$(LIBPATH)/boost/include -I./
CPPFLAGS= -g -finline-functions -Wall -Winline -pipe -march = native
LDFLAGS= -L$(LIBPATH)/boost/lib -lboost_thread -lboost_system -lpthread
LIBFLAGS=

EXEC=main

OUTPUT_PATH=./output

all: clean $(EXEC)

main: main.cpp
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.cpp %.h 
	$(CC) $(CPPFLAGS) -c $< -o $@ $(INCLUDES)

%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@ $(INCLUDES)


clean:
	rm -f *.o $(EXEC)
	rm -rf $(OUTPUT_PATH)

