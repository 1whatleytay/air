#pragma once

#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <cstdint>

class NodeBuildWebMethod {
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

    NodeBuildWebMethod(std::string name, std::vector<std::string> parameters);
};

class NodeBuildWebStyle {
public:
    std::string name;

    std::map<std::string, std::string> parameters;

    std::string build();

    NodeBuildWebStyle(std::string name);
};

class NodeBuildWeb {
    uint64_t uuid = 1;

    std::vector<std::shared_ptr<NodeBuildWebMethod>> methods;
    std::vector<std::shared_ptr<NodeBuildWebStyle>> styles;
public:
    uint64_t getNextUUID();

    bool hasStyle(const std::string &name);

    NodeBuildWebStyle *createStyle(const std::string &name);
    NodeBuildWebMethod *createMethod(const std::string &name, const std::vector<std::string> &parameters);

    std::string buildJS();
    std::string buildCSS();
};
