#include <nodes/elementnode.h>

#include <nodes/stylenode.h>
#include <nodes/parameternode.h>
#include <nodes/expressionnode.h>

#include <utils/parser.h>

#include <fmt/format.h>

#include <sstream>

bool ElementNode::hasNamedParameter(const std::string &name) {
    return getNamedParameter(name) != nullptr;
}
ParameterNode *ElementNode::getNamedParameter(const std::string &name) {
    Node *resolved = searchThis([name](Node *node) {
        if (node->type == Type::Parameter) {
            if (dynamic_cast<ParameterNode *>(node)->name == name)
                return true;
        }

        return false;
    });

    if (resolved)
        return dynamic_cast<ParameterNode *>(resolved);
    return nullptr;
}
bool ElementNode::hasNamelessParameter(uint32_t index) {
    return getNamelessParameter(index) != nullptr;
}
ParameterNode *ElementNode::getNamelessParameter(uint32_t index) {
    uint32_t currentIndex = 0;
    Node *resolved = searchThis([&currentIndex, index](Node *node) {
        if (node->type == Type::Parameter) {
            auto *parameterNode = dynamic_cast<ParameterNode *>(node);
            if (parameterNode->isNameless()) {
                if (currentIndex == index)
                    return true;
                currentIndex++;
            }
        }

        return false;
    });

    if (resolved)
        return dynamic_cast<ParameterNode *>(resolved);
    return nullptr;
}

std::string ElementNode::buildStyleClasses(NodeBuildWeb *output) {
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

void ElementNode::build(NodeBuildWeb *output, NodeBuildWebMethod *method) {
    if (type == "Text") {
        if (!hasNamelessParameter(0))
            throw std::runtime_error("Missing text parameter for Text element.");

        method->addLine(fmt::format("<div class=\"{}\">", buildStyleClasses(output)));
        method->startLine("${");
        getNamelessParameter(0)->build(output, method);
        method->endLine("}");
        method->addLine("</div>");
    } else if (type == "Button") {
        std::string id = fmt::format("a{}", output->getNextUUID());

        if (hasNamedParameter("onClick")) {
            method->startSetupLine(fmt::format("document.getElementById(\"{}\").onclick = ", id));
            method->mainCodeStream = &method->setup;
            getNamedParameter("onClick")->build(output, method);
            method->mainCodeStream = &method->code;
            method->endSetupLine(".bind(this);"); // for methods
        }

        if (hasNamelessParameter(0)) {
            method->addLine(fmt::format("<button id=\"{}\" class=\"{}\">", id, buildStyleClasses(output)));
            method->startLine("${");
            getNamelessParameter(0)->build(output, method);
            method->endLine("}");
            method->addLine("</button>");
        } else {
            throw std::runtime_error("Missing name parameter for Button element.");
        }
    } else if (type == "Box") {
        method->addLine(fmt::format("<div class=\"{}\">", buildStyleClasses(output)));
        searchThis([output, method](Node *node) {
            if (node->type == Type::Element)
                node->build(output, method);

            return false;
        });
        method->addLine("</div>");
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
        std::string next = parser.peekWord();

        if (next == ")") {
            parser.nextWord();
            break;
        }

        children.push_back(std::make_shared<ParameterNode>(this, parser));
    }

    std::string closing = parser.nextWord();
    if (closing == ":") {
        std::string next = parser.nextWord();
        while (!parser.reachedEnd()) {
            if (next == ")" || next == "}")
                break;

            parser.rollback();
            if (StyleNode::isStyleKeyword(next)) {
                children.push_back(std::make_shared<StyleNode>(this, parser));
                next = parser.nextWord();
            } else {
                break;
            }
        }
    } else {
        parser.rollback();
    }
}
