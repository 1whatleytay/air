#pragma once

#include <nodes/node.h>

class Parser;
class ParameterNode;

class ElementNode : public Node {
    bool hasNamedParameter(const std::string &name);
    ParameterNode *getNamedParameter(const std::string &name);
    bool hasNamelessParameter(uint32_t index);
    ParameterNode *getNamelessParameter(uint32_t index);

    std::string buildStyleClasses(NodeBuildWeb *output);

public:
    std::string type;

    void build(NodeBuildWeb *output, NodeBuildWebMethod *method) override;

    ElementNode(Node *parent, Parser &parser);
};
