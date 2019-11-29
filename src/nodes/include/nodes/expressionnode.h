#pragma once

#include <nodes/node.h>

class Parser;

class ExpressionNode : public Node {
    Node *evaluateReference();

public:
    enum class ExpressionType {
        Unknown,
        NumberLiteral,
        StringLiteral,
        Reference,
    };

    ExpressionType expressionType = ExpressionType::Unknown;
    std::string value;

    bool needsRefresh();
    std::string evaluateType();

    void build(NodeBuildJS *output, NodeBuildJSMethod *method) override;

    ExpressionNode(Node *parent, Parser &parser);
};
