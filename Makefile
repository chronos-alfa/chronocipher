CC=g++
FLAGS=-Wall -Wextra -Wpedantic -Werror -std=c++17 -g
BUILD_FLAGS=-Wall -Wextra -Wpedantic -Werror -std=c++17 -DNDEBUG -s
all: objs/lib.o test/test.o test/test.out testrunner

objs/lib.o: src/lib.cpp
	mkdir -p objs
	$(CC) src/lib.cpp -c -o objs/lib.o $(FLAGS)

test/test.o: test/test.cpp
	$(CC) test/test.cpp -c -o test/test.o $(FLAGS)

test/test.out: objs/lib.o test/test.o
	$(CC) objs/lib.o test/test.o -o test/test.out $(FLAGS)

testrunner: test/test.out
	./test/test.out

clean:
	rm -rf build/
	rm -rf objs/
	rm test/test.o
	rm test/test.out

build: src/lib.cpp src/chronocipher.cpp
	mkdir -p build
	$(CC) src/lib.cpp src/chronocipher.cpp -o build/chronocipher $(BUILD_FLAGS)
