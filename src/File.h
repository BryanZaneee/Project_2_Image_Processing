#ifndef PROJECT2_FILE_H
#define PROJECT2_FILE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

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

double multiply(double P1, double P2);
double screen(double P1, double P2);
double subtract(double P1, double P2);
double addition(double P1, double P2);
double overlay(double P1, double P2);

Image readTGA(const string& filename);
void writeTGA(const string& filename, const Image& img);
Image multiplyBlend(const Image& top, const Image& bottom);
Image subtractBlend(const Image& top, const Image& bottom);
Image screenBlend(const Image& top, const Image& bottom);
Image overlayBlend(const Image& top, const Image& bottom);
Image addGreen(const Image& img, int value);
Image scaleRedBlue(const Image& img, double redScale, double blueScale);
void writeChannel(const string& filename, const Image& img, char channel);
Image combineChannels(const Image& red, const Image& green, const Image& blue);
Image rotate180(const Image& img);

#endif //PROJECT2_FILE_H
