APP = voidserver

CCFLAGS = -std=c++23 -Wall -Wno-unused-function

INCLUDE_DIRS =  -I./include  \
				-I./include/* \

debug: main.cpp
	g++ -o ${APP} main.cpp src/*/*.cpp src/*.cpp ${INCLUDE_DIRS} ${CCFLAGS} -DDEBUG

release: main.cpp
	g++ -o ${APP} main.cpp src/*/*.cpp src/*.cpp ${INCLUDE_DIRS} ${CCFLAGS} -O1

clean:
	rm ${APP}

run:
	./${APP}