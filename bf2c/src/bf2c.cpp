#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <algorithm>

#include "bfgenerator.h"


bool is_bf_char(char c) {
    return c == '+'
        || c == '-'
        || c == '<'
        || c == '>'
        || c == '['
        || c == ']'
        || c == '.'
        || c == ',';
}

std::vector<std::string> group_by_type(const std::string& input) {
    std::vector<std::string> groups;
    std::string buf;
    
    for(auto& c : input) {
        if(!buf.empty() && buf[0] != c) {
            groups.push_back(buf);
            buf.clear();
        }
        buf.push_back(c);
    }

    if(!buf.empty())
        groups.push_back(buf);

    return groups;
}

int main(int argc, char **argv) {
    if(argc == 1)
       return 1;
    
    std::ifstream file(argv[1]);
    if(!file.is_open()) {
        std::cerr << "Can't open file \'" << argv[1] << "\'\n";
        return 1;
    }

    std::string input_bf;
    std::copy_if(
            std::istream_iterator<char>(file),
            std::istream_iterator<char>(),
            std::back_inserter(input_bf),
            is_bf_char
    );

    auto groups_bf = group_by_type(input_bf);

    std::string out_path = (argc >= 3 ? argv[2] : "a.c");
    BFGenerator gen{ out_path };
    for(auto& group : groups_bf)
        gen.generate_next_opcode(group);
}
