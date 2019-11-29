#pragma once

#include <nodes/nodebuildjs.h>

#include <string>
#include <vector>
#include <memory>
#include <functional>

typedef const std::vector<std::string> &Attributes;

class Node {
public:
    enum class Type {
        Code,
        Page,
        Variable,
        View,
        Element,
        Expression,
        Method,
        Statement,
    };

    Type type;
    Node *parent;
    std::vector<std::shared_ptr<Node>> children;

    Node *searchThis(const std::function<bool(Node *)> &checker);
    Node *searchParent(const std::function<bool(Node *)> &checker);

    virtual void verify();
    virtual void build(NodeBuildJS *output, NodeBuildJSMethod *method);

    Node(Type type, Node *parent);
    virtual ~Node();
};
