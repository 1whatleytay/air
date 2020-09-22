#pragma once

#include <string>

class Interface {
    std::string inputFile;
    std::string outputFile;

    void parse(int count, char **arguments);

public:
    void start();

    Interface(int count, char **arguments);
};
