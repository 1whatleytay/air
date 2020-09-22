#pragma once

#include <nodes/node.h>

class Parser;

class ViewNode : public Node {
    std::string buildStyleClasses(NodeBuildWeb *output);
public:
    void build(NodeBuildWeb *output, NodeBuildWebMethod *method) override;

    ViewNode(Node *parent, Parser &parser);
};
