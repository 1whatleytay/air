#include <nodes/nodebuildweb.h>

#include <fmt/format.h>

#include <utility>

void NodeBuildWebMethod::addSetupLine(const std::string &line) {
    startSetupLine(line);
    endSetupLine();
}
void NodeBuildWebMethod::startSetupLine(const std::string &line) {
    setup << "\t" << line;
}
void NodeBuildWebMethod::endSetupLine(const std::string &line) {
    setup << line << "\n";
}

void NodeBuildWebMethod::addLine(const std::string &line) {
    startLine(line);
    endLine();
}

void NodeBuildWebMethod::startLine(const std::string &line) {
    *mainCodeStream << "\t" << line;
}
void NodeBuildWebMethod::endLine(const std::string &line) {
    *mainCodeStream << line << "\n";
}

std::string NodeBuildWebMethod::build() {
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

NodeBuildWebMethod::NodeBuildWebMethod(std::string name, std::vector<std::string> parameters)
    : name(std::move(name)), parameters(std::move(parameters)) { }

std::string NodeBuildWebStyle::build() {
    std::stringstream stream;

    stream << "." << name << " {";
    for (const auto &param : parameters) {
        stream << " " << param.first << ": " << param.second;
    }
    stream << " }\n";

    return stream.str();
}

NodeBuildWebStyle::NodeBuildWebStyle(std::string name) : name(std::move(name)) { }

bool NodeBuildWeb::hasStyle(const std::string &name) {
    for (const std::shared_ptr<NodeBuildWebStyle> &style : styles) {
        if (style->name == name)
            return true;
    }

    return false;
}

class NodeBuildWebStyle *NodeBuildWeb::createStyle(const std::string & name) {
    std::shared_ptr<NodeBuildWebStyle> pointer = std::make_shared<NodeBuildWebStyle>(name);

    styles.push_back(pointer);

    return pointer.get();
}

NodeBuildWebMethod *NodeBuildWeb::createMethod(const std::string &name, const std::vector<std::string> &parameters) {
    std::shared_ptr<NodeBuildWebMethod> pointer = std::make_shared<NodeBuildWebMethod>(name, parameters);

    methods.push_back(pointer);

    return pointer.get();
}

uint64_t NodeBuildWeb::getNextUUID() {
    return uuid++;
}

std::string NodeBuildWeb::buildJS() {
    std::stringstream output;

    for (const std::shared_ptr<NodeBuildWebMethod> &method : methods) {
        output << method->build();
    }

    return output.str();
}

std::string NodeBuildWeb::buildCSS() {
    std::stringstream output;

    for (const std::shared_ptr<NodeBuildWebStyle> &style : styles) {
        output << style->build();
    }

    return output.str();
}
