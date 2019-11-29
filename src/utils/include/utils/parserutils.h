#pragma once

#include <string>

class Parser;

std::string parseString(Parser &parser, const std::string &quote = "\'");
