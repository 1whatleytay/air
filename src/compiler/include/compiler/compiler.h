#pragma once

#include <map>
#include <string>

class Compiler {
    std::string source;

public:
    std::map<std::string, std::string> build();

    explicit Compiler(std::string source);
};
