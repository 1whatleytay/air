#include <nodes/codenode.h>

#include <nodes/pagenode.h>

#include <utils/parser.h>

#include <fmt/format.h>

std::string attributeKeywords[] = {
    "main"
};

void CodeNode::parse(Parser &parser) {
    while (!parser.reachedEnd()) {
        std::vector<std::string> attributes;
        std::string next = parser.nextWord();

        while (std::find(std::begin(attributeKeywords), std::end(attributeKeywords), next)
            != std::end(attributeKeywords)) {
            attributes.push_back(next);
            next = parser.nextWord();
        }

        parser.rollback();

        if (next == "page")
            children.push_back(std::make_shared<PageNode>(this, parser, attributes));
        else
            throw std::runtime_error(fmt::format("Unexpected keyword {}.", next));
    }
}

CodeNode::CodeNode(Node *parent, Parser &parser) : Node(Node::Type::Code, parent) {
    parse(parser);
}
