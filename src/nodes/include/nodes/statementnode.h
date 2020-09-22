#pragma once

#include <nodes/node.h>

class Parser;

class StatementNode : public Node {
    void generateOperatorStatement(NodeBuildWeb *output, NodeBuildWebMethod *method, const std::string &op);
public:
    enum class StatementType {
        Unknown,
        AddEquals,
        SubtractEquals,
        Assignment,
    };

    StatementType statementType = StatementType::Unknown;

    void build(NodeBuildWeb *output, NodeBuildWebMethod *method) override;

    StatementNode(Node *parent, Parser &parser);
};
