#include "File.h"
#include <map>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <output file> <input file> <operation> [additional arguments...]\n";
        return 1;
    }

    std::string outputFile = argv[1];
    std::string inputFile = argv[2];
    std::string operation = argv[3];

    Image img = readTGA(inputFile);

    std::map<std::string, std::function<void(const std::string&, const Image&, const std::vector<std::string>&)>> operations = {
            {"multiply", [](const std::string& outputFile, const Image& img, const std::vector<std::string>& args) {
                Image img2 = readTGA(args[0]);
                writeTGA(outputFile, multiplyBlend(img, img2));
            }},
            {"subtract", [](const std::string& outputFile, const Image& img, const std::vector<std::string>& args) {
                Image img2 = readTGA(args[0]);
                writeTGA(outputFile, subtractBlend(img, img2));
            }},
            {"overlay", [](const std::string& outputFile, const Image& img, const std::vector<std::string>& args) {
                Image img2 = readTGA(args[0]);
                writeTGA(outputFile, overlayBlend(img, img2));
            }},
            {"screen", [](const std::string& outputFile, const Image& img, const std::vector<std::string>& args) {
                Image img2 = readTGA(args[0]);
                writeTGA(outputFile, screenBlend(img, img2));
            }},
            {"combine", [](const std::string& outputFile, const Image& img, const std::vector<std::string>& args) {
                Image imgGreen = readTGA(args[0]);
                Image imgBlue = readTGA(args[1]);
                writeTGA(outputFile, combineChannels(img, imgGreen, imgBlue));
            }},
            {"flip", [](const std::string& outputFile, const Image& img, const std::vector<std::string>& args) {
                writeTGA(outputFile, rotate180(img));
            }},
            {"onlyred", [](const std::string& outputFile, const Image& img, const std::vector<std::string>& args) {
                writeChannel(outputFile, img, 'r');
            }},
            {"onlygreen", [](const std::string& outputFile, const Image& img, const std::vector<std::string>& args) {
                writeChannel(outputFile, img, 'g');
            }},
            {"onlyblue", [](const std::string& outputFile, const Image& img, const std::vector<std::string>& args) {
                writeChannel(outputFile, img, 'b');
            }},
            {"addred", [](const std::string& outputFile, const Image& img, const std::vector<std::string>& args) {
                int value = std::stoi(args[0]);
                writeTGA(outputFile, addRed(img, value));
            }},
            {"addgreen", [](const std::string& outputFile, const Image& img, const std::vector<std::string>& args) {
                int value = std::stoi(args[0]);
                writeTGA(outputFile, addGreen(img, value));
            }},
            {"addblue", [](const std::string& outputFile, const Image& img, const std::vector<std::string>& args) {
                int value = std::stoi(args[0]);
                writeTGA(outputFile, addBlue(img, value));
            }},
            {"scalered", [](const std::string& outputFile, const Image& img, const std::vector<std::string>& args) {
                double scale = std::stod(args[0]);
                writeTGA(outputFile, scaleRed(img, scale));
            }},
            {"scalegreen", [](const std::string& outputFile, const Image& img, const std::vector<std::string>& args) {
                double scale = std::stod(args[0]);
                writeTGA(outputFile, scaleGreen(img, scale));
            }},
            {"scaleblue", [](const std::string& outputFile, const Image& img, const std::vector<std::string>& args) {
                double scale = std::stod(args[0]);
                writeTGA(outputFile, scaleBlue(img, scale));
            }}
    };

    if (operations.count(operation) == 0) {
        std::cout << "Invalid operation. Available operations are: multiply, subtract, screen, overlay, combine, flip, onlyred, onlygreen, onlyblue, addred, addgreen, addblue, scalered, scalegreen, scaleblue\n";
        return 1;
    }

    std::vector<std::string> additionalArgs(argv + 4, argv + argc);
    operations[operation](outputFile, img, additionalArgs);

    return 0;
}
