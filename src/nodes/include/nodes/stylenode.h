#pragma once

#include <nodes/node.h>

class Parser;

class StyleNode : public Node {
    void takeFlavor(Parser &parser, bool required = false);

    void addParameters(NodeBuildWebStyle *style);

public:
    enum class StyleType {
        Unknown,
        Center,
        HCenter,
        VCenter,
        TCenter,
        Width,
        Height,
        Padding,
        Margin,
        Rounded,
        Size,
        Color,
        Background,
    };

    StyleType styleType = StyleType::Unknown;
    std::string flavor;

    static bool isStyleKeyword(const std::string &keyword);

    std::string getStyleClass();

    void build(NodeBuildWeb *output, NodeBuildWebMethod *method) override;

    StyleNode(Node *parent, Parser &parser);
};
