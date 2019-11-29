#pragma once

#include <nodes/node.h>

class Parser;

class StatementNode : public Node {
public:
    enum class StatementType {
        Unknown,
        AddEquals,
        SubtractEquals,
        Assignment
    };

    StatementType statementType = StatementType::Unknown;

    void build(NodeBuildJS *output, NodeBuildJSMethod *method) override;

    StatementNode(Node *parent, Parser &parser);
};
