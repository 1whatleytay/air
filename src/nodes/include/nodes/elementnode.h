#pragma once

#include <nodes/node.h>

class Parser;

class ElementNode : public Node {
public:
    std::string type;

    void build(NodeBuildJS *output, NodeBuildJSMethod *method);

    ElementNode(Node *parent, Parser &parser);
};
