#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

// Assuming P1 and P2 are normalized pixel values in the range [0, 1]
// Multiply
double multiply(double P1, double P2) {
    return P1 * P2;
}

// Screen
double screen(double P1, double P2) {
    return 1.0 - (1.0 - P1) * (1.0 - P2);
}

// Subtract
double subtract(double P1, double P2) {
    return std::max(0.0, P1 / 255.0 - P2 / 255.0);
}

// Addition
double addition(double P1, double P2) {
    return std::min(1.0, P1 + P2);  // Ensure the result is not more than 1
}

// Overlay
double overlay(double P1, double P2) {
    if (P2 <= 0.5) {
        return 2.0 * P1 * P2;
    } else {
        return 1.0 - 2.0 * (1.0 - P1) * (1.0 - P2);
    }
}


// Pixel structure to hold the blue, green, and red components of a pixel
struct Pixel {
    unsigned char blue, green, red;
};

// Image structure to hold the TGA header and pixel data of an image
struct Image {
    char idLength;                  // Length of the ID field that follows the header (usually 0)
    char colorMapType;              // Whether a color map is included (usually 0)
    char dataTypeCode;              // The type of image (usually 2 for uncompressed RGB)
    short colorMapOrigin;           // The first entry index for the color map (usually 0)
    short colorMapLength;           // The length of the color map (usually 0)
    char colorMapDepth;             // The size of each entry in the color map (usually 0)
    short xOrigin;                  // The horizontal pixel coordinate for the lower left corner of the image (usually 0)
    short yOrigin;                  // The vertical pixel coordinate for the lower left corner of the image (usually 0)
    short width;                    // The width of the image in pixels
    short height;                   // The height of the image in pixels
    char bitsPerPixel;              // The number of bits per pixel (usually 24 for RGB)
    char imageDescriptor;           // Extra data about the image (usually 0)
    vector<Pixel> pixels;           // The pixel data of the image
};


// Function to read a TGA file and return an Image structure
Image readTGA(const string& filename) {
    ifstream file(filename, ios::binary); // Open the file in binary mode
    Image img;

    // Read the TGA header from the file
    img.idLength = file.get();
    img.colorMapType = file.get();
    img.dataTypeCode = file.get();
    img.colorMapOrigin = file.get() + (file.get() << 8);
    img.colorMapLength = file.get() + (file.get() << 8);
    img.colorMapDepth = file.get();
    img.xOrigin = file.get() + (file.get() << 8);
    img.yOrigin = file.get() + (file.get() << 8);
    img.width = file.get() + (file.get() << 8);
    img.height = file.get() + (file.get() << 8);
    img.bitsPerPixel = file.get();
    img.imageDescriptor = file.get();

    // Resize the pixel vector to hold all the pixels in the image
    img.pixels.resize(img.width * img.height);

    // Read the pixel data from the file
    file.read(reinterpret_cast<char*>(img.pixels.data()), img.pixels.size() * sizeof(Pixel));

    return img; // Return the image
}

// Function to write an Image structure to a TGA file
void writeTGA(const string& filename, const Image& img) {
    ofstream file(filename, ios::binary); // Open the file in binary mode

    // Write the TGA header to the file
    file.put(img.idLength);
    file.put(img.colorMapType);
    file.put(img.dataTypeCode);
    file.put(img.colorMapOrigin & 0xFF);
    file.put((img.colorMapOrigin >> 8) & 0xFF);
    file.put(img.colorMapLength & 0xFF);
    file.put((img.colorMapLength >> 8) & 0xFF);
    file.put(img.colorMapDepth);
    file.put(img.xOrigin & 0xFF);
    file.put((img.xOrigin >> 8) & 0xFF);
    file.put(img.yOrigin & 0xFF);
    file.put((img.yOrigin >> 8) & 0xFF);
    file.put(img.width & 0xFF);
    file.put((img.width >> 8) & 0xFF);
    file.put(img.height & 0xFF);
    file.put((img.height >> 8) & 0xFF);
    file.put(img.bitsPerPixel);
    file.put(img.imageDescriptor);

    // Write the pixel data to the file
    file.write(reinterpret_cast<const char*>(img.pixels.data()), img.pixels.size() * sizeof(Pixel));
}


// Task 1
Image multiplyBlend(const Image& top, const Image& bottom) {
    Image result = top; // Start with a copy of the top image
    for (size_t i = 0; i < result.pixels.size(); ++i) {
        // Calculate the new red, green, and blue values as doubles
        // This is done by normalizing the pixel values (dividing by 255.0), performing the multiply operation,
        // and then denormalizing the result (multiplying by 255)
        double red = multiply(top.pixels[i].red / 255.0, bottom.pixels[i].red / 255.0) * 255;
        double green = multiply(top.pixels[i].green / 255.0, bottom.pixels[i].green / 255.0) * 255;
        double blue = multiply(top.pixels[i].blue / 255.0, bottom.pixels[i].blue / 255.0) * 255;

        // Clamp the resulting red, green, and blue values to the range 0-255
        // This is done using the std::min and std::max functions to ensure the values are within this range
        // This prevents any issues related to overflow or underflow of the unsigned char data type
        result.pixels[i].red = static_cast<unsigned char>(std::min(std::max(red, 0.0), 255.0));
        result.pixels[i].green = static_cast<unsigned char>(std::min(std::max(green, 0.0), 255.0));
        result.pixels[i].blue = static_cast<unsigned char>(std::min(std::max(blue, 0.0), 255.0));
    }
    return result;
}

// Task 2
Image subtractBlend(const Image& top, const Image& bottom) {
    Image result = top;
    for (size_t i = 0; i < result.pixels.size(); ++i) {
        double red = subtract(top.pixels[i].red, bottom.pixels[i].red) * 255;
        double green = subtract(top.pixels[i].green, bottom.pixels[i].green) * 255;
        double blue = subtract(top.pixels[i].blue, bottom.pixels[i].blue) * 255;
        result.pixels[i].red = static_cast<unsigned char>(std::min(std::max(red, 0.0), 255.0));
        result.pixels[i].green = static_cast<unsigned char>(std::min(std::max(green, 0.0), 255.0));
        result.pixels[i].blue = static_cast<unsigned char>(std::min(std::max(blue, 0.0), 255.0));
    }
    return result;
}

// Task 3
Image screenBlend(const Image& top, const Image& bottom) {
    Image result = top;
    for (size_t i = 0; i < result.pixels.size(); ++i) {
        double red = screen(top.pixels[i].red / 255.0, bottom.pixels[i].red / 255.0) * 255;
        double green = screen(top.pixels[i].green / 255.0, bottom.pixels[i].green / 255.0) * 255;
        double blue = screen(top.pixels[i].blue / 255.0, bottom.pixels[i].blue / 255.0) * 255;
        result.pixels[i].red = static_cast<unsigned char>(std::min(std::max(red, 0.0), 255.0));
        result.pixels[i].green = static_cast<unsigned char>(std::min(std::max(green, 0.0), 255.0));
        result.pixels[i].blue = static_cast<unsigned char>(std::min(std::max(blue, 0.0), 255.0));
    }
    return result;
}

// Task 4
// This task is a combination of multiply and subtract

// Task 5
Image overlayBlend(const Image& top, const Image& bottom) {
    Image result = top;
    for (size_t i = 0; i < result.pixels.size(); ++i) {
        double red = overlay(top.pixels[i].red / 255.0, bottom.pixels[i].red / 255.0) * 255;
        double green = overlay(top.pixels[i].green / 255.0, bottom.pixels[i].green / 255.0) * 255;
        double blue = overlay(top.pixels[i].blue / 255.0, bottom.pixels[i].blue / 255.0) * 255;
        result.pixels[i].red = static_cast<unsigned char>(std::min(std::max(red, 0.0), 255.0));
        result.pixels[i].green = static_cast<unsigned char>(std::min(std::max(green, 0.0), 255.0));
        result.pixels[i].blue = static_cast<unsigned char>(std::min(std::max(blue, 0.0), 255.0));
    }
    return result;
}

// Task 6
Image addGreen(const Image& img, int value) {
    Image result = img;
    for (Pixel& pixel : result.pixels) {
        int green = pixel.green + value;
        pixel.green = static_cast<unsigned char>(std::min(std::max(green, 0), 255));
    }
    return result;
}

// Task 7
Image scaleRedBlue(const Image& img, double redScale, double blueScale) {
    Image result = img;
    for (Pixel& pixel : result.pixels) {
        int red = pixel.red * redScale;
        int blue = pixel.blue * blueScale;
        pixel.red = static_cast<unsigned char>(std::min(std::max(red, 0), 255));
        pixel.blue = static_cast<unsigned char>(std::min(std::max(blue, 0), 255));
    }
    return result;
}

// Task 8
void writeChannel(const string& filename, const Image& img, char channel) {
    Image result = img;
    for (Pixel& pixel : result.pixels) {
        if (channel == 'r') {
            pixel.green = pixel.red;
            pixel.blue = pixel.red;
        } else if (channel == 'g') {
            pixel.red = pixel.green;
            pixel.blue = pixel.green;
        } else if (channel == 'b') {
            pixel.red = pixel.blue;
            pixel.green = pixel.blue;
        }
    }
    writeTGA(filename, result);
}

// Task 9
Image combineChannels(const Image& red, const Image& green, const Image& blue) {
    Image result = red;
    for (size_t i = 0; i < result.pixels.size(); ++i) {
        result.pixels[i].green = green.pixels[i].red;
        result.pixels[i].blue = blue.pixels[i].red;
    }
    return result;
}

// Task 10
Image rotate180(const Image& img) {
    Image result = img;
    std::reverse(result.pixels.begin(), result.pixels.end());
    return result;
}

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
