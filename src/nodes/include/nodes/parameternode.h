#pragma once

#include <nodes/node.h>

class Parser;
class ExpressionNode;

class ParameterNode : public Node {
public:
    std::string name;

    bool isNameless();
    ExpressionNode *getValueNode();

    ParameterNode(Node *parent, Parser &parser, bool typeOnly = false);
};
