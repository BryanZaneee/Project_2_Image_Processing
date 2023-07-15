#include "File.h"
#include <functional>
#include <map>
#include <string>

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
    return std::max(0.0, P2 - P1);  // Ensure the result is not less than 0
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
    for (Pixel& pixel : img.pixels) {
        pixel.blue = file.get();
        pixel.green = file.get();
        pixel.red = file.get();
    }

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
    for (const Pixel& pixel : img.pixels) {
        file.put(pixel.blue);
        file.put(pixel.green);
        file.put(pixel.red);
    }
}

// Function to perform multiply blend operation
Image multiplyBlend(const Image& top, const Image& bottom) {
    Image result = top; // Start with a copy of the top image
    for (size_t i = 0; i < result.pixels.size(); ++i) {
        // Calculate the new red, green, and blue values as doubles
        double red = multiply(top.pixels[i].red / 255.0, bottom.pixels[i].red / 255.0) * 255;
        double green = multiply(top.pixels[i].green / 255.0, bottom.pixels[i].green / 255.0) * 255;
        double blue = multiply(top.pixels[i].blue / 255.0, bottom.pixels[i].blue / 255.0) * 255;

        // Round the resulting red, green, and blue values to the nearest integer
        // This is done by adding 0.5 to the value before casting it to an unsigned char
        result.pixels[i].red = static_cast<unsigned char>(std::min(std::max(red + 0.5, 0.0), 255.0));
        result.pixels[i].green = static_cast<unsigned char>(std::min(std::max(green + 0.5, 0.0), 255.0));
        result.pixels[i].blue = static_cast<unsigned char>(std::min(std::max(blue + 0.5, 0.0), 255.0));
    }
    return result;
}

// Function to perform subtract blend operation
// Function to perform subtract blend operation
Image subtractBlend(const Image& top, const Image& bottom) {
    Image result = top;
    for (size_t i = 0; i < result.pixels.size(); ++i) {
        int red = static_cast<int>(bottom.pixels[i].red) - static_cast<int>(top.pixels[i].red);
        int green = static_cast<int>(bottom.pixels[i].green) - static_cast<int>(top.pixels[i].green);
        int blue = static_cast<int>(bottom.pixels[i].blue) - static_cast<int>(top.pixels[i].blue);

        result.pixels[i].red = static_cast<unsigned char>(std::min(std::max(red, 0), 255));
        result.pixels[i].green = static_cast<unsigned char>(std::min(std::max(green, 0), 255));
        result.pixels[i].blue = static_cast<unsigned char>(std::min(std::max(blue, 0), 255));
    }
    return result;
}



// Function to perform screen blend operation
Image screenBlend(const Image& top, const Image& bottom) {
    Image result = top;
    for (size_t i = 0; i < result.pixels.size(); ++i) {
        double red = screen(top.pixels[i].red / 255.0, bottom.pixels[i].red / 255.0) * 255;
        double green = screen(top.pixels[i].green / 255.0, bottom.pixels[i].green / 255.0) * 255;
        double blue = screen(top.pixels[i].blue / 255.0, bottom.pixels[i].blue / 255.0) * 255;

        result.pixels[i].red = static_cast<unsigned char>(std::min(std::max(red + 0.5, 0.0), 255.0));
        result.pixels[i].green = static_cast<unsigned char>(std::min(std::max(green + 0.5, 0.0), 255.0));
        result.pixels[i].blue = static_cast<unsigned char>(std::min(std::max(blue + 0.5, 0.0), 255.0));
    }
    return result;
}

// Function to perform overlay blend operation
Image overlayBlend(const Image& top, const Image& bottom) {
    Image result = top;
    for (size_t i = 0; i < result.pixels.size(); ++i) {
        double red = overlay(top.pixels[i].red / 255.0, bottom.pixels[i].red / 255.0) * 255;
        double green = overlay(top.pixels[i].green / 255.0, bottom.pixels[i].green / 255.0) * 255;
        double blue = overlay(top.pixels[i].blue / 255.0, bottom.pixels[i].blue / 255.0) * 255;

        result.pixels[i].red = static_cast<unsigned char>(std::min(std::max(red + 0.5, 0.0), 255.0));
        result.pixels[i].green = static_cast<unsigned char>(std::min(std::max(green + 0.5, 0.0), 255.0));
        result.pixels[i].blue = static_cast<unsigned char>(std::min(std::max(blue + 0.5, 0.0), 255.0));
    }
    return result;
}

Image addRed(const Image& img, int value) {
    Image result = img;
    for (Pixel& pixel : result.pixels) {
        int red = pixel.red + value;
        pixel.red = static_cast<unsigned char>(std::min(std::max(red, 0), 255));
    }
    return result;
}

// Function to add green to an image
Image addGreen(const Image& img, int value) {
    Image result = img;
    for (Pixel& pixel : result.pixels) {
        int green = pixel.green + value;
        pixel.green = static_cast<unsigned char>(std::min(std::max(green, 0), 255));
    }
    return result;
}

// Function to scale red and blue in an image
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

Image scaleRed(const Image& img, double scale) {
    Image result = img;
    for (Pixel& pixel : result.pixels) {
        int red = pixel.red * scale;
        pixel.red = static_cast<unsigned char>(std::min(std::max(red, 0), 255));
    }
    return result;
}

Image scaleGreen(const Image& img, double scale) {
    Image result = img;
    for (Pixel& pixel : result.pixels) {
        int green = pixel.green * scale;
        pixel.green = static_cast<unsigned char>(std::min(std::max(green, 0), 255));
    }
    return result;
}

Image scaleBlue(const Image& img, double scale) {
    Image result = img;
    for (Pixel& pixel : result.pixels) {
        int blue = pixel.blue * scale;
        pixel.blue = static_cast<unsigned char>(std::min(std::max(blue, 0), 255));
    }
    return result;
}


// Function to write a single channel to an image
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

// Function to combine channels from three images
Image combineChannels(const Image& red, const Image& green, const Image& blue) {
    Image result = red;
    for (size_t i = 0; i < result.pixels.size(); ++i) {
        result.pixels[i].green = green.pixels[i].red;
        result.pixels[i].blue = blue.pixels[i].red;
    }
    return result;
}

// Function to rotate an image by 180 degrees
Image rotate180(const Image& img) {
    Image result = img;
    std::reverse(result.pixels.begin(), result.pixels.end());
    return result;
}
