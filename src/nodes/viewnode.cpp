#include <nodes/viewnode.h>

#include <nodes/stylenode.h>
#include <nodes/elementnode.h>

#include <utils/parser.h>

#include <fmt/format.h>

std::string ViewNode::buildStyleClasses(NodeBuildWeb *output) {
    std::stringstream stream;

    bool first = true;
    searchThis([&first, &stream, output](Node *node) {
        if (node->type == Type::Style) {
            auto *style = dynamic_cast<StyleNode *>(node);

            if (!first)
                stream << " ";
            first = false;

            style->build(output, nullptr);
            stream << style->getStyleClass();
        }

        return false;
    });

    return stream.str();
}

void ViewNode::build(NodeBuildWeb *output, NodeBuildWebMethod *method) {
    if (!parent)
        return;

    if (parent->type != Type::View) {
        method->addSetupLine("this.finalize = function() {");
        method->addLine("return `");
    }

    method->addLine(fmt::format("<div class=\"{}\">", buildStyleClasses(output)));
    searchThis([&](Node *node) {
        if (node->type == Type::Element)
            node->build(output, method);

        return false;
    });
    method->addLine("</div>");

    if (parent->type != Type::View) {
        method->addLine("`;");
        method->addSetupLine("}");
    }
}

ViewNode::ViewNode(Node *parent, Parser &parser) : Node(Node::Type::View, parent) {
    std::string self = parser.nextWord();
    assert(self == "view");

    if (parser.peekWord() == ":") {
        parser.nextWord();
        std::string next = parser.peekWord();
        while (!parser.reachedEnd() && next != "{" && StyleNode::isStyleKeyword(next)) {
            children.push_back(std::make_shared<StyleNode>(this, parser));
            next = parser.peekWord();
        }
    }

    std::string opening = parser.nextWord();

    if (opening != "{")
        throw std::runtime_error(fmt::format("Unexpected symbol {} in view node.", opening));

    std::string next = parser.peekWord();
    while (!parser.reachedEnd() && next != "}") {
        if (next == "view")
            children.push_back(std::make_shared<ViewNode>(this, parser));
        else
            children.push_back(std::make_shared<ElementNode>(this, parser));

        next = parser.peekWord();
    }

    parser.nextWord();
}
