#include <interface/interface.h>

#include <compiler/compiler.h>

#include <utils/file.h>

#include <string>
#include <filesystem>

namespace fs = std::filesystem;

void Interface::parse(int count, char **arguments) {
    for (int a = 1; a < count; a++) {
        std::string argument(arguments[a]);

        if (argument == "-o") {
            if (a + 1 < count) {
                outputFile = arguments[a + 1];
                a++;
            }
        } else {
            inputFile = arguments[a];
        }
    }
}

void Interface::start() {
    Compiler compiler(loadText(inputFile));
    std::map<std::string, std::string> fileData = compiler.build();

    fs::path outputPath = outputFile;

    if (!fs::is_directory(outputFile))
        fs::create_directory(outputFile);
    for (const auto &file : fileData) {
        writeText(outputPath / file.first, file.second);
    }
}

Interface::Interface(int count, char **arguments) {
    parse(count, arguments);
}
