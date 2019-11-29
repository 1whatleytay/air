#pragma once

#include <nodes/node.h>

class Parser;

class ViewNode : public Node {
public:
    void build(NodeBuildJS *output, NodeBuildJSMethod *method) override;

    ViewNode(Node *parent, Parser &parser);
};
