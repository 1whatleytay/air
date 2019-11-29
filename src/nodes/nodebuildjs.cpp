#include <nodes/nodebuildjs.h>

#include <fmt/format.h>

#include <utility>

void NodeBuildJSMethod::addSetupLine(const std::string &line) {
    startSetupLine(line);
    endSetupLine();
}
void NodeBuildJSMethod::startSetupLine(const std::string &line) {
    setup << "\t" << line;
}
void NodeBuildJSMethod::endSetupLine(const std::string &line) {
    setup << line << "\n";
}

void NodeBuildJSMethod::addLine(const std::string &line) {
    startLine(line);
    endLine();
}

void NodeBuildJSMethod::startLine(const std::string &line) {
    *mainCodeStream << "\t" << line;
}
void NodeBuildJSMethod::endLine(const std::string &line) {
    *mainCodeStream << line << "\n";
}

NodeBuildJSMethod::NodeBuildJSMethod(std::string name, std::vector<std::string> parameters)
    : name(std::move(name)), parameters(std::move(parameters)) { }

std::string NodeBuildJSMethod::build() {
    std::stringstream output;
    output << "function " << name << "(";

    bool first = true;
    for (const std::string &param : parameters) {
        if (!first) {
            output << ", ";
        }
        first = false;

        output << param;
    }

    output << ") {\n" << setup.str() << code.str() << "}\n\n";

    return output.str();
}

NodeBuildJSMethod *NodeBuildJS::createMethod(const std::string &name, const std::vector<std::string> &parameters) {
    std::shared_ptr<NodeBuildJSMethod> pointer = std::make_shared<NodeBuildJSMethod>(name, parameters);

    methods.push_back(pointer);

    return pointer.get();
}

uint64_t NodeBuildJS::getNextUUID() {
    return uuid++;
}

std::string NodeBuildJS::build() {
    std::stringstream output;

    for (const std::shared_ptr<NodeBuildJSMethod> &method : methods) {
        output << method->build();
    }

    return output.str();
}
