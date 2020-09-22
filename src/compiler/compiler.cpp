#include <compiler/compiler.h>

#include <nodes/codenode.h>
#include <nodes/pagenode.h>

#include <utils/file.h>
#include <utils/parser.h>

#include <fmt/format.h>

std::string filesToCopy[] = {
    "jet.js",
    "index.html"
};

std::map<std::string, std::string> Compiler::build() {
    Parser parser(source);

    CodeNode topNode(nullptr, parser);
    topNode.verify();

    NodeBuildWeb output;
    topNode.build(&output, nullptr);

    PageNode *mainPage = dynamic_cast<PageNode *>(topNode.searchThis([](Node *node) {
        return (node->type == Node::Type::Page) && (dynamic_cast<PageNode *>(node)->isMain);
    }));

    if (!mainPage)
        throw std::runtime_error("Missing main page.");

    NodeBuildWebMethod *mainMethod = output.createMethod("main", { });
    mainMethod->addLine(fmt::format("return new {}();", mainPage->name));

    std::map<std::string, std::string> files;
    files["app.js"] = output.buildJS();
    files["app.css"] = output.buildCSS();

    for (const std::string &file : filesToCopy) {
        // load from binary location
        files[file] = loadText(file);
    }

    return files;
}

Compiler::Compiler(std::string source) : source(std::move(source)) { }
