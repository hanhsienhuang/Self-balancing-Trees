
all: main

main: main.cpp vector.h vector.cpp
	g++ -o main main.cpp -Wall


TEST_OPTION = -lgtest -pthread -I./ -Wall

test/test_main: test/test_main.cpp test/*.h *.cpp *.h  trees/*.h
	g++ -o test/test_main test/test_main.cpp ${TEST_OPTION}

test: test/test_main
	test/test_main

