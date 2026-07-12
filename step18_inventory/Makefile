
CXX := g++

CXXFLAGS := -std=c++20 -Wall -Wextra -pedantic  -Iinclude

TARGET := inventory

OBJECTS := build/main.o build/inventory.o

.PHONY : all clean

all: $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CXX) $(OBJECTS)  -o $(TARGET)

build/main.o: src/main.cpp include/inventory.hpp include/product.hpp
	mkdir -p build
	$(CXX) $(CXXFLAGS) -c src/main.cpp -o build/main.o

build/inventory.o: src/inventory.cpp include/inventory.hpp include/product.hpp
	mkdir -p build
	$(CXX) $(CXXFLAGS) -c src/inventory.cpp -o build/inventory.o

clean:
	rm -rf build $(TARGET)