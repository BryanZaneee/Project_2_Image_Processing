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
	./project2.out multiply "input/layer1.tga" "input/pattern2.tga" "temp1.tga"
	./project2.out screen "input/text.tga" "temp1.tga" "output/part3.tga"
	./project2.out multiply "input/layer2.tga" "input/circles.tga" "temp2.tga"
	./project2.out subtract "input/pattern2.tga" "temp2.tga" "output/part4.tga"
	./project2.out overlay "input/layer1.tga" "input/pattern1.tga" "output/part5.tga"
	./project2.out addGreen "input/car.tga" 200 "output/part6.tga"
	./project2.out scaleRedBlue "input/car.tga" 4 0 "output/part7.tga"
	./project2.out writeChannel "input/car.tga" 'r' "output/part8_r.tga"
	./project2.out writeChannel "input/car.tga" 'g' "output/part8_g.tga"
	./project2.out writeChannel "input/car.tga" 'b' "output/part8_b.tga"
	./project2.out combineChannels "input/layer_red.tga" "input/layer_green.tga" "input/layer_blue.tga" "output/part9.tga"
	./project2.out rotate180 "input/text2.tga" "output/part10.tga"

# Rule to clean the project
clean:
	rm -f project2.out
