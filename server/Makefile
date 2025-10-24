CCFLAGS = -std=c++23

INCLUDE_DIRS =  -I./include  \
				-I./include/* \

all: main.cpp
	g++ -o server main.cpp src/*/*.cpp src/*.cpp ${INCLUDE_DIRS} ${CCFLAGS}

run:
	./server