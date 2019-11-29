#include <nodes/elementnode.h>

#include <nodes/expressionnode.h>

#include <utils/parser.h>

#include <fmt/format.h>

void ElementNode::build(NodeBuildJS *output, NodeBuildJSMethod *method) {
    if (type == "Text") {
        method->addLine("<div>");
        if (children.size() != 1)
            throw std::exception();
        method->startLine("${");
        dynamic_cast<ExpressionNode *>(children[0].get())->build(output, method);
        method->endLine("}");
        method->addLine("</div>");
    } else if (type == "Button") {
        std::string id = fmt::format("a{}", output->getNextUUID());
        method->startSetupLine(fmt::format("document.getElementById(\"{}\").onclick = ", id));
        method->mainCodeStream = &method->setup;
        dynamic_cast<ExpressionNode *>(children[1].get())->build(output, method);
        method->mainCodeStream = &method->code;
        method->endSetupLine(".bind(this);"); // for methods

        method->addLine(fmt::format("<button id=\"{}\">", id));
        method->startLine("${");
        dynamic_cast<ExpressionNode *>(children[0].get())->build(output, method);
        method->endLine("}");
        method->addLine("</button>");
    } else {
        throw std::exception();
    }
}

ElementNode::ElementNode(Node *parent, Parser &parser) : Node(Node::Type::Element, parent) {
    type = parser.nextWord();

    std::string opening = parser.nextWord();
    if (opening != "(")
        throw std::runtime_error(fmt::format("Unexpected symbol {} in element.", opening));

    while (!parser.reachedEnd()) {
        std::string next = parser.nextWord();

        if (next == ")")
            break;
        if (next == ",")
            continue;

        parser.rollback();

        children.push_back(std::make_shared<ExpressionNode>(this, parser));
    }

    std::string closing = parser.nextWord();
    if (closing != ",")
        parser.rollback();
}
