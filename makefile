OBJS = ftserver.o
CXX = g++
DEBUG = -g
CFLAGS = -O0 -c -std=c++0x -Wall -pedantic ${DEBUG}
LFLAGS = -Wall ${DEBUG}

ftserver : ${OBJS}
	${CXX} ${LFLAGS} ${OBJS} -o ftserver

ftserver.o :  ftserver.cpp
	${CXX} ${CFLAGS} ftserver.cpp


clean:
	rm -f *.o ftserver
