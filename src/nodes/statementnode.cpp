#include <nodes/statementnode.h>

#include <nodes/expressionnode.h>

#include <utils/parser.h>

#include <fmt/format.h>

void StatementNode::generateOperatorStatement(NodeBuildWeb *output, NodeBuildWebMethod *method, const std::string &op) {
    method->startLine();
    children[0]->build(output, method);
    *method->mainCodeStream << fmt::format(" {} ", op);
    children[1]->build(output, method);
    method->endLine(";");
    if (dynamic_cast<ExpressionNode *>(children[0].get())->needsRefresh())
        method->addLine("refresh(this);");
}

void StatementNode::build(NodeBuildWeb *output, NodeBuildWebMethod *method) {
    std::string op;
    switch (statementType) {
    case StatementType::AddEquals:
        generateOperatorStatement(output, method, "+=");
        break;
    case StatementType::SubtractEquals:
        generateOperatorStatement(output, method, "-=");
        break;
    case StatementType::Assignment:
        generateOperatorStatement(output, method, "=");
        break;
    default:
        throw std::exception();
    }
}

StatementNode::StatementNode(Node *parent, Parser &parser) : Node(Node::Type::Statement, parent) {
    // Subject
    children.push_back(std::make_shared<ExpressionNode>(this, parser));

    std::string command = parser.nextWord();

    if (command == "+" && parser.peekWord() == "=") {
        parser.nextWord();
        statementType = StatementType::AddEquals;
        children.push_back(std::make_shared<ExpressionNode>(this, parser));
    } else if (command == "-" && parser.peekWord() == "=") {
        parser.nextWord();
        statementType = StatementType::SubtractEquals;
        children.push_back(std::make_shared<ExpressionNode>(this, parser));
    } else if (command == "=") {
        statementType = StatementType::Assignment;
        children.push_back(std::make_shared<ExpressionNode>(this, parser));
    } else {
        throw std::exception();
    }
}
