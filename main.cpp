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
    return std::max(0.0, P1 - P2);  // Ensure the result is not less than 0
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
        result.pixels[i].red = multiply(top.pixels[i].red, bottom.pixels[i].red);
        result.pixels[i].green = multiply(top.pixels[i].green, bottom.pixels[i].green);
        result.pixels[i].blue = multiply(top.pixels[i].blue, bottom.pixels[i].blue);
    }
    return result;
}

int main() {
    // Read images from a TGA file
    Image layer1 = readTGA("layer1.tga");
    Image pattern1 = readTGA("pattern1.tga");
    Image layer2 = readTGA("layer2.tga");
    Image car = readTGA("car.tga");

    // Write the image to another TGA file
    writeTGA("output/part1.tga", multiplyBlend(layer1, pattern1));
    return 0;
}