#include <nodes/pagenode.h>

#include <nodes/viewnode.h>
#include <nodes/methodnode.h>
#include <nodes/variablenode.h>
#include <nodes/expressionnode.h>

#include <utils/parser.h>

#include <fmt/format.h>

void PageNode::build(NodeBuildJS *output, NodeBuildJSMethod *method) {
    name = fmt::format("a{}", output->getNextUUID());

    NodeBuildJSMethod *constructor = output->createMethod(name, {});
    NodeBuildJSMethod *render = output->createMethod(fmt::format("{}$render", name), { });

    searchThis([&](Node *node) {
        switch (node->type) {
        case Type::Variable: {
            auto *variable = dynamic_cast<VariableNode *>(node);
            variable->build(output, constructor);
            break;
        }
        case Type::Method: {
            auto *methodNode = dynamic_cast<MethodNode *>(node);
            methodNode->build(output, constructor);
            break;
        }
        case Type::View: {
            auto *view = dynamic_cast<ViewNode *>(node);
            view->build(output, render);
            break;
        }
        default: {
            break;
        }
        }
        return false;
    });

    constructor->addLine(fmt::format("this.render = {}$render;", name));
}

PageNode::PageNode(Node *parent, Parser &parser, Attributes attributes) : Node(Node::Type::Page, parent) {
    std::string self = parser.nextWord();
    assert(self == "page");

    if (parser.nextWord() != "{") {
        name = parser.lastWord();
        parser.rollback();
        if (parser.nextWord() != "{") {
            throw std::runtime_error("Missing { on page component.");
        }
    }

    std::string next = parser.nextWord();
    while (!parser.reachedEnd() && next != "}") {
        parser.rollback();

        if (next == "view")
            children.push_back(std::make_shared<ViewNode>(this, parser));
        else {
            parser.select();
            parser.nextWord();
            std::string symbol = parser.nextWord();
            parser.back();

            if (symbol == "=" || symbol == ":")
                children.push_back(std::make_shared<VariableNode>(this, parser));
            else if (symbol == "(")
                children.push_back(std::make_shared<MethodNode>(this, parser));
            else
                throw std::runtime_error(fmt::format("Unexpected declaration for symbol {}.", next));
        }

        next = parser.nextWord();
    }

    for (const std::string &attribute : attributes) {
        if (attribute == "main")
            isMain = true;
        else
            throw std::runtime_error(fmt::format("Unknown attribute {} on page component.", attribute));
    }
}
