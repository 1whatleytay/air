#include <nodes/node.h>

class Parser;

class LambdaNode : public Node {
public:
    void build(NodeBuildWeb *output, NodeBuildWebMethod *method);

    LambdaNode(Node *parent, Parser &parser);
};
