#pragma once

#include <nodes/node.h>

class Parser;
class ExpressionNode;

class VariableNode : public Node {
    std::string type;

public:
    std::string name;
    std::string evaluateType();

    bool needsThis();
    bool hasInitialValue();
    ExpressionNode *getValueNode();

    void build(NodeBuildJS *output, NodeBuildJSMethod *method) override;

    VariableNode(Node *parent, Parser &parser);
};
