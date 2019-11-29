#include <nodes/statementnode.h>

#include <nodes/expressionnode.h>

#include <utils/parser.h>

void StatementNode::build(NodeBuildJS *output, NodeBuildJSMethod *method) {
    switch (statementType) {
    case StatementType::AddEquals: {
        method->startLine();
        children[0]->build(output, method);
        *method->mainCodeStream << " += ";
        children[1]->build(output, method);
        method->endLine(";");
        if (dynamic_cast<ExpressionNode *>(children[0].get())->needsRefresh())
            method->addLine("refresh(this);");
        break;
    }
    case StatementType::SubtractEquals: {
        method->startLine();
        children[0]->build(output, method);
        *method->mainCodeStream << " -= ";
        children[1]->build(output, method);
        method->endLine(";");
        if (dynamic_cast<ExpressionNode *>(children[0].get())->needsRefresh())
            method->addLine("refresh(this);");
        break;
    }
    default: {
        throw std::exception();
    }
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
    } else {
        throw std::exception();
    }
}
