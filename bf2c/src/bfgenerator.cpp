#include "bfgenerator.h"

BFGenerator::BFGenerator(const std::string& path)
    : _indent_level(1), _out_file(path)
{
    if(!_out_file.is_open())
        throw std::runtime_error("can't open file " + path);

    _out_file << "#include <stdio.h>\n"
              << "#include <stdint.h>\n\n"
              << "uint8_t tab[30000];\n"
              << "uint8_t* ptr = &(tab[0]);\n\n"
              << "int main(void)\n"
              << "{\n";
}

BFGenerator::~BFGenerator() {
    _out_file << "}";
}

void BFGenerator::generate_next_opcode(const std::string& opcode) {
    if(opcode.empty() || !_out_file.is_open())
        return;

    auto op = opcode.front();
    auto size = opcode.size();
    switch(op) {
        case '+':
            _op_value_plus(size);
            break;
        case '-':
            _op_value_minus(size);
            break;
        case '>':
            _op_ptr_plus(size);
            break;
        case '<':
            _op_ptr_minus(size);
            break;
        case ',':
            for(auto i = 0u; i < size; ++i)
                _op_get();
            break;
        case '.':
            for(auto i = 0u; i < size; ++i)
                _op_put();
            break;
        case '[':
            for(auto i = 0u; i < size; ++i)
                _op_while_open();
            break;
        case ']':
            for(auto i = 0u; i < size; ++i)
                _op_while_end();
            break;
    }
}


void BFGenerator::_op_ptr_plus(size_t s) {
    _out_file << _indent_string();
    if(s != 1)
        _out_file << "ptr += " << s << ";\n";
    else
        _out_file << "ptr++;\n";
}

void BFGenerator::_op_ptr_minus(size_t s) {
    _out_file << _indent_string();
    if(s != 1)
        _out_file << "ptr -= " << s << ";\n";
    else
        _out_file << "ptr--;\n";

}

void BFGenerator::_op_value_plus(size_t s) {
    _out_file << _indent_string();
    if(s != 1)
        _out_file << "*ptr += " << s << ";\n";
    else
        _out_file << "(*ptr)++;\n";
}

void BFGenerator::_op_value_minus(size_t s) {
    _out_file << _indent_string();
    if(s != 1)
        _out_file << "*ptr -= " << s << ";\n";
    else
        _out_file << "(*ptr)--;\n";
}

void BFGenerator::_op_get() {
    _out_file << _indent_string() << "(*ptr) = getchar();\n";
}

void BFGenerator::_op_put() {
    _out_file << _indent_string() << "putchar(*ptr);\n";
}

void BFGenerator::_op_while_open() {
    _out_file << _indent_string() << "while(*ptr) {\n";
    _indent_level++;
}
void BFGenerator::_op_while_end() {
    _indent_level--;
    _out_file << _indent_string() << "}\n";
}

std::string BFGenerator::_indent_string() const{
    return std::string(_indent_level * 4, ' ');
}

