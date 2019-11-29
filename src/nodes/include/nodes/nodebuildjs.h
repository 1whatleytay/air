#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <cstdint>

class NodeBuildJSMethod {
public:
    std::string name;
    std::vector<std::string> parameters;

    std::stringstream setup;
    std::stringstream code;

    std::stringstream *mainCodeStream = &code;

    void addSetupLine(const std::string &line);
    void startSetupLine(const std::string &line = "");
    void endSetupLine(const std::string &line = "");

    void addLine(const std::string &line);
    void startLine(const std::string &line = "");
    void endLine(const std::string &line = "");

    std::string build();

    NodeBuildJSMethod(std::string name, std::vector<std::string> parameters);
};

class NodeBuildJS {
    uint64_t uuid = 1;

    std::vector<std::shared_ptr<NodeBuildJSMethod>> methods;
public:
    uint64_t getNextUUID();

    NodeBuildJSMethod *createMethod(const std::string &name, const std::vector<std::string> &parameters);

    std::string build();
};
