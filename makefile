CC=g++
CFLAGS=-std=c++17 -pthread
OUTPUT=oop_exercise_08

all:
	$(CC) $(CFLAGS) Figure.h Figure.cpp PubSub.h main.cpp -o $(OUTPUT)
run:
	./$(OUTPUT) 3
