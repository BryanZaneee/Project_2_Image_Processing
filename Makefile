# Variables to hold the compiler and flags
CXX = g++
CXXFLAGS = -std=c++11

# Default rule to build the project
all: project2.out

# Rule to build the executable
project2.out: src/main.cpp src/File.cpp
	$(CXX) $(CXXFLAGS) -o project2.out src/main.cpp src/File.cpp

# rule to run the program
run: project2.out
	./project2.out

# Rule to clean the project
clean:
	rm -f project2.out

