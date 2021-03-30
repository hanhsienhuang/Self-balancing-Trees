
all: main

main: main.cpp vector.h vector.cpp
	g++ -o main main.cpp -Wall

tests/test_vector: tests/test_vector.cpp vector.h vector.cpp
	g++ -o tests/test_vector test_vector.cpp -Wall

tests/test_trees: test_trees.cpp trees/*.h
	g++ -o tests/test_trees test_trees.cpp -Wall


test_vector: tests/test_vector
	tests/test_vector

test_trees: tests/test_trees
	tests/test_trees

test: test_vector test_trees

