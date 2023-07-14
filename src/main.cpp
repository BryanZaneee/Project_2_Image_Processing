#include "File.h"

int main() {
    // Read images from a TGA file
    Image layer1 = readTGA("input/layer1.tga");
    Image pattern1 = readTGA("input/pattern1.tga");
    Image layer2 = readTGA("input/layer2.tga");
    Image car = readTGA("input/car.tga");
    Image pattern2 = readTGA("input/pattern2.tga");
    Image text = readTGA("input/text.tga");
    Image circles = readTGA("input/circles.tga");
    Image layer_red = readTGA("input/layer_red.tga");
    Image layer_green = readTGA("input/layer_green.tga");
    Image layer_blue = readTGA("input/layer_blue.tga");
    Image text2 = readTGA("input/text2.tga");

    // Task 1
    writeTGA("output/part1.tga", multiplyBlend(layer1, pattern1));

    // Task 2
    writeTGA("output/part2.tga", subtractBlend(layer2, car));

    // Task 3
    Image temp1 = multiplyBlend(layer1, pattern2);
    writeTGA("output/part3.tga", screenBlend(text, temp1));

    // Task 4
    Image temp2 = multiplyBlend(layer2, circles);
    writeTGA("output/part4.tga", subtractBlend(pattern2, temp2));

    // Task 5
    writeTGA("output/part5.tga", overlayBlend(layer1, pattern1));

    // Task 6
    writeTGA("output/part6.tga", addGreen(car, 200));

    // Task 7
    writeTGA("output/part7.tga", scaleRedBlue(car, 4, 0));

    // Task 8
    writeChannel("output/part8_r.tga", car, 'r');
    writeChannel("output/part8_g.tga", car, 'g');
    writeChannel("output/part8_b.tga", car, 'b');

    // Task 9
    writeTGA("output/part9.tga", combineChannels(layer_red, layer_green, layer_blue));

    // Task 10
    writeTGA("output/part10.tga", rotate180(text2));

    return 0;
}