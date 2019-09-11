CC = g++
CFLAGS = -W -Wall
TARGET = nemo.exe
OBJECTS = main.o nemo.o clearScreen.o


# all: $(TARGET)

$(TARGET) : main.cpp nemo.o clearScreen.o
	$(CC) -o $(TARGET) main.cpp nemo.o clearScreen.o

# $(TARGET) : $(OBJECTS)
# 	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# main.o : main.cpp
# 	$(CC) $(CFLAGS) -o main.o main.cpp

nemo.o : nemo.cpp
	$(CC) -c -o nemo.o nemo.cpp

# fileReader.o : fileReader.cpp
# 	g++ -o fileReader.o fileReader.cpp

clearScreen.o : clearScreen.cpp
	$(CC) -c -o clearScreen.o clearScreen.cpp


clean:
	rm -f *.o
	rm -f $(TARGET)
