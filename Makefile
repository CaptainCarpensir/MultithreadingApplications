CXX = g++
FLAGS = -W -Wall -Wextra -Wpedantic -Werror -std=c++11 -g
LIBRARIES = -lpthread

.PHONY: default run

default: run

run:
	${CXX} ${FLAGS} *.cpp ${LIBRARIES} -o program

debug:
	${CXX} ${FLAGS} -g *.cpp ${LIBRARIES} -o program

clean:
	-@rm -rf *.o program core
