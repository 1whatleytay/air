#pragma once

#include <nodes/node.h>

class Parser;

class CodeNode : public Node {
    void parse(Parser &parser);
public:

    CodeNode(Node *parent, Parser &parser);
};
