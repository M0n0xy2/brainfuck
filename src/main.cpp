#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <algorithm>

#include "bf_c_generator.hpp"
#include "bf_llvm_generator.hpp"

bool is_bf_char(char c) {
    return c == '+' || c == '-' || c == '<' || c == '>' || c == '[' ||
           c == ']' || c == '.' || c == ',';
}

std::vector<std::string> group_by_type(const std::string &input) {
    std::vector<std::string> groups;
    std::string buf;

    for (auto &c : input) {
        if (!buf.empty() && buf[0] != c) {
            groups.push_back(buf);
            buf.clear();
        }
        buf.push_back(c);
    }

    if (!buf.empty())
        groups.push_back(buf);

    return groups;
}

int main(int argc, char **argv) {
    if (argc == 1)
        return 1;

    std::string input_path{argv[1]};
    std::string output_path{argc >= 3 ? argv[2] : "a.ir"};

    std::ifstream input_file(argv[1]);

    if (!input_file.is_open()) {
        std::cerr << "Can't open file \'" << input_path << "\'\n";
        return 2;
    }

    std::string brainfuck_program;
    std::copy_if(std::istream_iterator<char>(input_file),
                 std::istream_iterator<char>(),
                 std::back_inserter(brainfuck_program), is_bf_char);

    run_brainfuck<bf_llvm_generator>(group_by_type(brainfuck_program));
}
