#include <utils/file.h>

#include <vector>
#include <fstream>

std::string loadText(const std::string &path) {
    std::ifstream stream(path, std::ios::ate);
    assert(stream.is_open());
    std::vector<char> data(stream.tellg());
    stream.seekg(0, std::ios::beg);
    stream.read(data.data(), data.size());

    stream.close();

    return std::string(data.begin(), data.end());
}

void writeText(const std::string &path, const std::string &out) {
    std::ofstream stream(path, std::ios::trunc);
    assert(stream.is_open());
    stream.write(out.c_str(), out.size());
    stream.close();
}
