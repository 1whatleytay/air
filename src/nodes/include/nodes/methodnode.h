#pragma once

#include <nodes/node.h>

class Parser;

class MethodNode : public Node {
public:
    std::string name;
    std::vector<std::string> parameters;

    bool needsThis();

    void build(NodeBuildJS *output, NodeBuildJSMethod *method) override;

    MethodNode(Node *parent, Parser &parser);
};
