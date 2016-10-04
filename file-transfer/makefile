DIR1 = include/
DIR2 = src/
INC=$(DIR1) $(DIR2)
INC_PARAMS=$(foreach d, $(INC), -I$d)
OBJS = ftserver.o main.o
CXX = g++
DEBUG = -g
CFLAGS = -O0 -c -std=c++0x -Wall -pedantic ${DEBUG}
LFLAGS = -Wall ${DEBUG}

ftserver : ${OBJS}
	${CXX} ${LFLAGS} ${OBJS} -o ftserver

ftserver.o :  src/ftserver.cpp
	${CXX} ${CFLAGS} src/ftserver.cpp

main.o : src/main.cpp
	$(CXX) ${CFLAGS} src/main.cpp

clean:
	rm -f *.o ftserver