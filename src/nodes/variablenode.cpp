#include <nodes/variablenode.h>

#include <nodes/expressionnode.h>

#include <utils/parser.h>

#include <fmt/format.h>

ExpressionNode* VariableNode::getValueNode() {
    return dynamic_cast<ExpressionNode *>(children[0].get());
}

std::string VariableNode::evaluateType() {
    if (type.empty())
        return getValueNode()->evaluateType();
    else
        return type;
}

bool VariableNode::needsThis() {
    if (!parent)
        return false;

    return parent->type == Type::Page;
}

bool VariableNode::hasInitialValue() {
    return !children.empty();
}

void VariableNode::build(NodeBuildWeb *output, NodeBuildWebMethod *method) {
    if (!method)
        return;

    // build for variables means add initializer, global is not supported yet

    method->startLine(fmt::format("{}{} = ", needsThis() ? "this." : "", name));
    getValueNode()->build(output, method);
    method->endLine();
}

VariableNode::VariableNode(Node *parent, Parser &parser) : Node(Node::Type::Variable, parent) {
    name = parser.nextWord();

    std::string declareSymbol = parser.nextWord();
    if (declareSymbol == "=") {
        children.push_back(std::make_shared<ExpressionNode>(this, parser));
    } else if (declareSymbol == ":") {
        type = parser.nextWord();
    }
}
