#include <nodes/viewnode.h>

#include <nodes/elementnode.h>

#include <utils/parser.h>

#include <fmt/format.h>

void ViewNode::build(NodeBuildJS *output, NodeBuildJSMethod *method) {
    // please forgive me for using ecma6 template literals
    method->addSetupLine("this.finalize = function() {");
    method->addLine("return `");
    searchThis([&](Node *node) {
        node->build(output, method);
        return false;
    });
    method->addLine("`;");
    method->addSetupLine("}");
}

ViewNode::ViewNode(Node *parent, Parser &parser) : Node(Node::Type::View, parent) {
    std::string self = parser.nextWord();
    assert(self == "view");

    std::string opening = parser.nextWord();
    if (opening != "{")
        throw std::runtime_error(fmt::format("Unexpected symbol {} in view node.", opening));

    std::string next = parser.nextWord();
    while (!parser.reachedEnd() && next != "}") {
        parser.rollback();
        children.push_back(std::make_shared<ElementNode>(this, parser));
        next = parser.nextWord();
    }
}
