# Variables to hold the compiler and flags
CXX = g++
CXXFLAGS = -std=c++11

# Default rule to build the project
all: project2.out

# Rule to build the executable
project2.out: src/main.cpp src/File.cpp
	$(CXX) $(CXXFLAGS) -o project2.out src/main.cpp src/File.cpp

# Rule to run all tasks
run: project2.out
	./project2.out multiply "input/layer1.tga" "input/pattern1.tga" "output/part1.tga"
	./project2.out subtract "input/layer2.tga" "input/car.tga" "output/part2.tga"
	./project2.out screen "input/layer1.tga" "input/pattern2.tga" "output/part3.tga"
	./project2.out overlay "input/layer1.tga" "input/pattern1.tga" "output/part4.tga"
	./project2.out combine "input/layer1.tga" "input/layer2.tga" "input/layer3.tga" "output/part5.tga"
	./project2.out flip "input/car.tga" "output/part6.tga"
	./project2.out onlyred "input/car.tga" "output/part7.tga"
	./project2.out onlygreen "input/car.tga" "output/part8.tga"
	./project2.out onlyblue "input/car.tga" "output/part9.tga"
	./project2.out addred "input/car.tga" 200 "output/part10.tga"
	./project2.out addgreen "input/car.tga" 200 "output/part11.tga"
	./project2.out addblue "input/car.tga" 200 "output/part12.tga"
	./project2.out scalered "input/car.tga" 4 "output/part13.tga"
	./project2.out scalegreen "input/car.tga" 4 "output/part14.tga"
	./project2.out scaleblue "input/car.tga" 4 "output/part15.tga"

# Rule to clean the project
clean:
	rm -f project2.out
