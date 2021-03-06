#include <nodes/expressionnode.h>

#include <nodes/nodeutils.h>
#include <nodes/lambdanode.h>
#include <nodes/methodnode.h>
#include <nodes/variablenode.h>

#include <utils/parser.h>
#include <utils/charprops.h>

#include <fmt/format.h>

Node *ExpressionNode::evaluateReference() {
    if (expressionType != ExpressionType::Reference)
        return nullptr;

    return searchParent([this](Node *node) {
        switch (node->type) {
        case Type::Variable: {
            auto *variable = dynamic_cast<VariableNode *>(node);
            if (variable->name == value)
                return true;
            break;
        }
        case Type::Method: {
            auto *method = dynamic_cast<MethodNode *>(node);
            if (method->name == value)
                return true;
            break;
        }
        default: {
            break;
        }
        }

        return false;
    });
}

bool ExpressionNode::needsRefresh() {
    if (expressionType != ExpressionType::Reference)
        return false;

    Node *reference = evaluateReference();

    if (reference->type == Type::Variable)
        return dynamic_cast<VariableNode *>(reference)->needsThis();

    return false;
}

std::string ExpressionNode::evaluateType() {
    switch (expressionType) {
    case ExpressionType::NumberLiteral:
        return "number";
    case ExpressionType::StringLiteral:
    case ExpressionType::StringTemplateLiteral:
        return "string";
    case ExpressionType::Lambda:
        return "function";
    case ExpressionType::Reference: {
        Node *referenced = evaluateReference();

        if (!referenced)
            throw std::runtime_error(fmt::format("Unsure to what {} is referring to.", value));

        switch (referenced->type) {
        case Type::Variable: {
            auto *variable = dynamic_cast<VariableNode *>(referenced);
            return variable->evaluateType();
        }
        default:
            throw std::exception();
        }
    }
    default:
        return "";
    }
}

void ExpressionNode::build(NodeBuildWeb *output, NodeBuildWebMethod *method) {
    if (!method)
        return;

    // Can't do anything without a method to append to.

    switch (expressionType) {
    case ExpressionType::NumberLiteral: {
        *method->mainCodeStream << value;
        break;
    }
    case ExpressionType::StringLiteral: {
        *method->mainCodeStream << fmt::format("\"{}\"", value);
        break;
    }
    case ExpressionType::StringTemplateLiteral: {
        *method->mainCodeStream << "`";
        uint32_t lastIndex = 0;
        for (size_t a = 0; a < indices.size(); a++) {
            *method->mainCodeStream << value.substr(lastIndex, indices[a] - lastIndex) << "${";
            children[a]->build(output, method);
            *method->mainCodeStream << "}";
            lastIndex = indices[a];
        }
        *method->mainCodeStream << value.substr(lastIndex) << "`";
        break;
    }
    case ExpressionType::Lambda: {
        children[0]->build(output, method);
        break;
    }
    case ExpressionType::Reference: {
        Node *referenced = evaluateReference();

        if (!referenced)
            throw std::runtime_error(fmt::format("Unsure to what {} is referring to.", value));

        switch (referenced->type) {
        case Type::Variable: {
            auto *variableNode = dynamic_cast<VariableNode *>(referenced);
            *method->mainCodeStream
                << fmt::format("{}{}", variableNode->needsThis() ? "this." : "", variableNode->name);
            break;
        }
        case Type::Method: {
            auto *methodNode = dynamic_cast<MethodNode *>(referenced);
            *method->mainCodeStream
                << fmt::format("{}{}", methodNode->needsThis() ? "this." : "", methodNode->name);
            break;
        }
        default:
            throw std::exception();
        }
        break;
    }
    default:
        throw std::exception();
    }
}

ExpressionNode::ExpressionNode(Node *parent, Parser &parser) : Node(Node::Type::Expression, parent) {
    std::string next = parser.nextWord();

    if (next.empty())
        throw std::runtime_error("Unexpected end of file.");

    if (next == "'") {
        parser.rollback();
        QuotesValue text = parseString(this, parser);
        value = text.text;
        if (text.inserts.empty()) {
            expressionType = ExpressionType::StringLiteral;
        } else {
            expressionType = ExpressionType::StringTemplateLiteral;
            for (const QuotesInsertPoint &point : text.inserts) {
                indices.push_back(point.insertPoint);
                children.push_back(point.expressionNode);
            }
        }
    } else if (next == "{") {
        parser.rollback();
        expressionType = ExpressionType::Lambda;
        children.push_back(std::make_shared<LambdaNode>(this, parser));
    } else if (next == "-" && isDigits(parser.peekWord())) {
        expressionType = ExpressionType::NumberLiteral;
        value = "-" + parser.nextWord();
    } else if (isDigits(next)) {
        expressionType = ExpressionType::NumberLiteral;
        value = next;
    } else {
        expressionType = ExpressionType::Reference;
        value = next;
    }
}
