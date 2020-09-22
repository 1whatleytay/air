#pragma once

#include <nodes/node.h>

#include <vector>

class Parser;

class PageNode : public Node {
public:
    std::string name;
    bool isMain = false;

    void build(NodeBuildWeb *output, NodeBuildWebMethod *method) override;

    PageNode(Node *parent, Parser &parser, Attributes attributes);
};
