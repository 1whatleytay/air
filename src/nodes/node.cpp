#include <nodes/node.h>

Node *Node::searchThis(const std::function<bool(Node *)> &checker) {
    for (std::shared_ptr<Node> &child : children) {
        if (checker(child.get()))
            return child.get();
    }

    return nullptr;
}

Node *Node::searchParent(const std::function<bool(Node *)> &checker) {
    if (!parent)
        return nullptr;

    for (const std::shared_ptr<Node> &child : parent->children) {
        if (checker(child.get()))
            return child.get();
    }

    Node *parentSearch = parent->searchParent(checker);
    if (parentSearch)
        return parentSearch;

    return nullptr;
}

void Node::verify() {
    for (const std::shared_ptr<Node> &child : children) {
        child->verify();
    }
}

void Node::build(NodeBuildWeb *output, NodeBuildWebMethod *method) {
    for (const std::shared_ptr<Node> &child : children) {
        child->build(output, method);
    }
}

Node::Node(Node::Type type, Node *parent) : type(type), parent(parent) { }
Node::~Node() = default;
