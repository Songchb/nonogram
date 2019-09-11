CC=g++
# CFLAGS = -W -WALL
# TARGET=nemo.exe

nemo.exe: main.o nemo.o clearScreen.o
	g++ -o nemo.exe main.o nemo.o clearScreen.o

# all: $(TARGET)
main.o : main.cpp
	g++ -c -o main.o main.cpp

nemo.o : nemo.cpp
	g++ -c -o nemo.o nemo.cpp

# fileReader.o : fileReader.cpp
# 	g++ -c -o fileReader.o fileReader.cpp

clearScreen.o : clearScreen.cpp
	g++ -c -o clearScreen.o clearScreen.cpp


clean:
	rm *.o
	# rm -f $(TARGET)
