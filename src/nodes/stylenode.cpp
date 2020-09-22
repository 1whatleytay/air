#include <nodes/stylenode.h>

#include <utils/parser.h>

#include <fmt/format.h>

std::string styleKeywords[] = {
    "center",
    "hcenter",
    "vcenter",
    "tcenter",
    "width",
    "height",
    "p",
    "m",
    "rounded",
    "size",
    "color",
    "bg",
};

void StyleNode::takeFlavor(Parser &parser, bool required) {
    if (parser.peekWord() == "(") {
        parser.nextWord();
        flavor = parser.nextWord();
        if (parser.nextWord() != ")")
            throw std::exception();
    } else if (required) {
        throw std::runtime_error("Missing flavor for style attribute.");
    }
}

void StyleNode::addParameters(NodeBuildWebStyle *style) {
    switch (styleType) {
    case StyleType::HCenter:
        style->parameters["margin-left"] = "auto";
        style->parameters["margin-right"] = "auto";
        break;
    case StyleType::TCenter:
        style->parameters["text-align"] = "center";
        break;
    case StyleType::Width:
        if (flavor.empty())
            style->parameters["width"] = "100%";
        else
            style->parameters["width"] = fmt::format("{}px", flavor);
        break;
    case StyleType::Height:
        if (flavor.empty())
            style->parameters["height"] = "100%";
        else
            style->parameters["height"] = fmt::format("{}px", flavor);
        break;
    case StyleType::Padding:
        style->parameters["padding"] = fmt::format("{}px", flavor);
        break;
    case StyleType::Margin:
        style->parameters["magin"] = fmt::format("{}px", flavor);
        break;
    case StyleType::Rounded:
        if (flavor.empty())
            style->parameters["border-radius"] = "4px";
        else
            style->parameters["border-radius"] = fmt::format("{}px", flavor);
        break;
    case StyleType::Size:
        style->parameters["font-size"] = fmt::format("{}px", flavor);
        break;
    case StyleType::Background:
        style->parameters["background-color"] = fmt::format("{}", flavor);
        break;
    case StyleType::Color:
        style->parameters["color"] = fmt::format("{}", flavor);
        break;
    default:
        throw std::exception();
    }
}

bool StyleNode::isStyleKeyword(const std::string &keyword) {
    return std::find(std::begin(styleKeywords), std::end(styleKeywords), keyword) != std::end(styleKeywords);
}

std::string StyleNode::getStyleClass() {
    switch (styleType) {
    case StyleType::HCenter:
        return "hcenter";
    case StyleType::TCenter:
        return "tcenter";
    case StyleType::Width:
        if (flavor.empty())
            return "w-full";
        else
            return fmt::format("w-{}", flavor);
    case StyleType::Height:
        if (flavor.empty())
            return "h-full";
        else
            return fmt::format("h-{}", flavor);
    case StyleType::Padding:
        return fmt::format("p-{}", flavor);
    case StyleType::Margin:
        return fmt::format("m-{}", flavor);
    case StyleType::Rounded:
        return "rounded";
    case StyleType::Size:
        return fmt::format("size-{}", flavor);
    case StyleType::Background:
        return fmt::format("bg-{}", flavor);
    case StyleType::Color:
        return fmt::format("color-{}", flavor);
    default:
        throw std::exception();
    }
}

void StyleNode::build(class NodeBuildWeb *output, class NodeBuildWebMethod *method) {
    std::string styleClass = getStyleClass();

    if (!output->hasStyle(styleClass)) {
        NodeBuildWebStyle *style = output->createStyle(styleClass);
        addParameters(style);
    }
}

StyleNode::StyleNode(Node *parent, Parser &parser) : Node(Node::Type::Style, parent) {
    std::string style = parser.nextWord();

    if (style == "center") {
        styleType = StyleType::Center;
    } else if (style == "hcenter") {
        styleType = StyleType::HCenter;
    } else if (style == "vcenter") {
        styleType = StyleType::VCenter;
    } else if (style == "tcenter") {
        styleType = StyleType::TCenter;
    } else if (style == "width") {
        styleType = StyleType::Width;
        takeFlavor(parser);
    } else if (style == "height") {
        styleType = StyleType::Height;
        takeFlavor(parser);
    } else if (style == "p") {
        styleType = StyleType::Padding;
        takeFlavor(parser, true);
    } else if (style == "m") {
        styleType = StyleType::Margin;
        takeFlavor(parser, true);
    } else if (style == "rounded") {
        styleType = StyleType::Rounded;
    } else if (style == "size") {
        styleType = StyleType::Size;
        takeFlavor(parser, true);
    } else if (style == "color") {
        styleType = StyleType::Color;
        takeFlavor(parser, true);
    } else if (style == "bg") {
        styleType = StyleType::Background;
        takeFlavor(parser, true);
    } else {
        throw std::exception();
    }
}
