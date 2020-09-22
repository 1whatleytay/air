#include <nodes/parameternode.h>

#include <nodes/expressionnode.h>

#include <utils/parser.h>

bool ParameterNode::isNameless() {
    return name.empty();
}

ExpressionNode* ParameterNode::getValueNode() {
    return dynamic_cast<ExpressionNode *>(children[0].get());
}

ParameterNode::ParameterNode(Node *parent, Parser &parser, bool typeOnly) : Node(Node::Type::Parameter, parent) {
    assert(!typeOnly);

    std::string maybeName = parser.nextWord();
    if (parser.peekWord() == ":") {
        parser.nextWord();

        name = maybeName;
    } else {
        parser.rollback();
    }

    children.push_back(std::make_shared<ExpressionNode>(this, parser));
}
