#include "File.h"

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

// Function to perform multiply blend operation
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

// Function to perform subtract blend operation
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

// Function to perform screen blend operation
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

// Function to perform overlay blend operation
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
