#ifndef PROJECT2_FILE_H
#define PROJECT2_FILE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

struct Pixel {
    unsigned char blue, green, red;
};

struct Image {
    char idLength;
    char colorMapType;
    char dataTypeCode;
    short colorMapOrigin;
    short colorMapLength;
    char colorMapDepth;
    short xOrigin;
    short yOrigin;
    short width;
    short height;
    char bitsPerPixel;
    char imageDescriptor;
    vector<Pixel> pixels;
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
Image addRed(const Image& img, int value);
Image addGreen(const Image& img, int value);
Image addBlue(const Image& img, int value);
Image scaleRed(const Image& img, double scale);
Image scaleGreen(const Image& img, double scale);
Image scaleBlue(const Image& img, double scale);
void writeChannel(const string& filename, const Image& img, char channel);
Image combineChannels(const Image& red, const Image& green, const Image& blue);
Image rotate180(const Image& img);

#endif //PROJECT2_FILE_H
