#include "File.h"
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Invalid number of arguments." << std::endl;
        return 1;
    }

    std::string outputFilename = argv[1];
    if (outputFilename.substr(outputFilename.find_last_of(".") + 1) != "tga") {
        std::cout << "Invalid file name." << std::endl;
        return 1;
    }

    std::string inputFilename = argv[2];
    if (inputFilename.substr(inputFilename.find_last_of(".") + 1) != "tga") {
        std::cout << "Invalid file name." << std::endl;
        return 1;
    }

    Image img = readTGA(inputFilename);

    for (int i = 3; i < argc; i += 2) {
        std::string method = argv[i];

        if (method == "multiply" || method == "subtract" || method == "overlay" || method == "screen") {
            if (i + 1 >= argc) {
                std::cout << "Missing argument." << std::endl;
                return 1;
            }

            std::string secondFilename = argv[i + 1];
            if (secondFilename.substr(secondFilename.find_last_of(".") + 1) != "tga") {
                std::cout << "Invalid argument, invalid file name." << std::endl;
                return 1;
            }

            Image secondImg = readTGA(secondFilename);

            if (method == "multiply") {
                img = multiplyBlend(img, secondImg);
            } else if (method == "subtract") {
                img = subtractBlend(img, secondImg);
            } else if (method == "overlay") {
                img = overlayBlend(img, secondImg);
            } else if (method == "screen") {
                img = screenBlend(img, secondImg);
            }
        } else if (method == "combine") {
            if (i + 2 >= argc) {
                std::cout << "Missing argument." << std::endl;
                return 1;
            }

            std::string greenFilename = argv[i + 1];
            std::string blueFilename = argv[i + 2];
            if (greenFilename.substr(greenFilename.find_last_of(".") + 1) != "tga" || blueFilename.substr(blueFilename.find_last_of(".") + 1) != "tga") {
                std::cout << "Invalid argument, invalid file name." << std::endl;
                return 1;
            }

            Image greenImg = readTGA(greenFilename);
            Image blueImg = readTGA(blueFilename);

            img = combineChannels(img, greenImg, blueImg);
            i++; // Skip an extra argument
        } else if (method == "flip") {
            img = flip(img);
        } else if (method == "onlyred" || method == "onlygreen" || method == "onlyblue") {
            img = onlyColor(img, std::string(1, method[4]));
        } else if (method == "addred" || method == "addgreen" || method == "addblue") {
            if (i + 1 >= argc) {
                std::cout << "Missing argument." << std::endl;
                return 1;
            }

            int value = std::stoi(argv[i + 1]);
            img = addColor(img, std::string(1, method[3]), value);
        } else if (method == "scalered" || method == "scalegreen" || method == "scaleblue") {
            if (i + 1 >= argc) {
                std::cout << "Missing argument." << std::endl;
                return 1;
            }

            int value = std::stoi(argv[i + 1]);
            img = scaleColor(img, std::string(1, method[5]), value);
        } else {
            std::cout << "Invalid method name." << std::endl;
            return 1;
        }
    }

    writeTGA(outputFilename, img);

    return 0;
}
