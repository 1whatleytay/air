#include <nodes/lambdanode.h>

#include <nodes/statementnode.h>

#include <utils/parser.h>

void LambdaNode::build(NodeBuildWeb *output, NodeBuildWebMethod *method) {
    method->endLine("function() {");
    searchThis([&](Node *node) {
        if (node->type != Type::Statement)
            return false;

        auto *statement = dynamic_cast<StatementNode *>(node);
        statement->build(output, method);

        return false;
    });
    method->startLine("}");
}

LambdaNode::LambdaNode(Node *parent, Parser &parser) : Node(Node::Type::Lambda, parent) {
    // lambdas don't support parameters yet, as with the rest of the language
    std::string self = parser.nextWord();
    assert(self == "{");

    while (!parser.reachedEnd()) {
        std::string next = parser.nextWord();

        if (next == "}")
            break;

        parser.rollback();
        children.push_back(std::make_shared<StatementNode>(this, parser));
    }
}
