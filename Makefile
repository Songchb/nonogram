CXX = g++
CFLAGS = -W -Wall
TARGET = nemo.exe
OBJECTS = main.o nemo.o clearScreen.o

# all: $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CXX) -o $(TARGET) $(OBJECTS)

# $(TARGET) : $(OBJECTS)
# 	$(CXX) $(CFLAGS) -o $(TARGET) $(OBJECTS)

main.o : main.cpp
	$(CXX) -c -o main.o main.cpp

nemo.o : nemo.cpp
	$(CXX) -c -o nemo.o nemo.cpp

# fileReader.o : fileReader.cpp
# 	g++ -o fileReader.o fileReader.cpp

clearScreen.o : clearScreen.cpp
	$(CXX) -c -o clearScreen.o clearScreen.cpp


clean:
	rm -f *.o
	rm -f $(TARGET)
