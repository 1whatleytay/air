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
        StringTemplateLiteral,
        Lambda,
        Reference,
    };

    ExpressionType expressionType = ExpressionType::Unknown;
    std::string value;
    std::vector<uint32_t> indices;

    bool needsRefresh();
    std::string evaluateType();

    void build(NodeBuildWeb *output, NodeBuildWebMethod *method) override;

    ExpressionNode(Node *parent, Parser &parser);
};
