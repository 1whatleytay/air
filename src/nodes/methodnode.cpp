#include <nodes/methodnode.h>

#include <nodes/pagenode.h>
#include <nodes/statementnode.h>

#include <utils/parser.h>

#include <fmt/format.h>

bool MethodNode::needsThis() {
    if (!parent)
        return false;

    return parent->type == Type::Page;
}

void MethodNode::build(NodeBuildWeb *output, NodeBuildWebMethod *method) {
    if (!method)
        return;

    if (!parent || parent->type != Type::Page)
        return;

    auto *page = dynamic_cast<PageNode *>(parent);

    std::string methodLinkName = fmt::format("{}${}", page->name, name);
    NodeBuildWebMethod *thisMethod = output->createMethod(methodLinkName, { });

    searchThis([&](Node *node) {
        if (node->type != Type::Statement)
            return false;

        auto *statement = dynamic_cast<StatementNode *>(node);
        statement->build(output, thisMethod);

        return false;
    });

    method->addLine(fmt::format("this.{} = {};", name, methodLinkName));
}

MethodNode::MethodNode(Node *parent, Parser &parser) : Node(Node::Type::Method, parent) {
    name = parser.nextWord();

    std::string openingParams = parser.nextWord();
    assert(openingParams == "(");

    while (!parser.reachedEnd()) {
        std::string next = parser.nextWord();

        if (next == ")") {
            break;
        } else {
            // Parameters must work like method(param1: number, param2: string, param3: type) { }
            throw std::exception();
        }
    }

    std::string opening = parser.nextWord();
    if (opening != "{")
        throw std::runtime_error(fmt::format("Unexpected symbol by method body {}.", opening));

    while (!parser.reachedEnd()) {
        std::string next = parser.nextWord();

        if (next == "}") {
            break;
        } else {
            parser.rollback();
            children.push_back(std::make_shared<StatementNode>(this, parser));
        }
    }
}
