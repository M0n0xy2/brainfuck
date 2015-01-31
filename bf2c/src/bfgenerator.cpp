#include "bfgenerator.h"

BFGenerator::BFGenerator(std::string path)
    : _indent_level(1), _output_stream(path)
{
    if(!ready())
        return;

    _output_stream << "/* generated by bf2c */\n"
                   << "#include <stdio.h>\n"
                   << "#include <stdint.h>\n\n"
                   << "static uint8_t m[30000], *ptr = m;\n\n"
                   << "void main(void)\n"
                   << "{\n";
}

BFGenerator::~BFGenerator() {
    _output_stream << "}";
}

void BFGenerator::generate_next_opcode(const std::string& opcode) {
    if(opcode.empty())
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

bool BFGenerator::ready() const {
    return _output_stream.is_open();
}

void BFGenerator::_op_ptr_plus(size_t s) {
    _output_stream << _indent_string();
    if(s != 1)
        _output_stream << "ptr += " << s << ";\n";
    else
        _output_stream << "ptr++;\n";
}

void BFGenerator::_op_ptr_minus(size_t s) {
    _output_stream << _indent_string();
    if(s != 1)
        _output_stream << "ptr -= " << s << ";\n";
    else
        _output_stream << "ptr--;\n";

}

void BFGenerator::_op_value_plus(size_t s) {
    _output_stream << _indent_string();
    if(s != 1)
        _output_stream << "*ptr += " << s << ";\n";
    else
        _output_stream << "(*ptr)++;\n";
}

void BFGenerator::_op_value_minus(size_t s) {
    _output_stream << _indent_string();
    if(s != 1)
        _output_stream << "*ptr -= " << s << ";\n";
    else
        _output_stream << "(*ptr)--;\n";
}

void BFGenerator::_op_get() {
    _output_stream << _indent_string() << "(*ptr) = getchar();\n";
}

void BFGenerator::_op_put() {
    _output_stream << _indent_string() << "putchar(*ptr);\n";
}

void BFGenerator::_op_while_open() {
    _output_stream << _indent_string() << "while(*ptr) {\n";
    _indent_level++;
}
void BFGenerator::_op_while_end() {
    _indent_level--;
    _output_stream << _indent_string() << "}\n";
}

std::string BFGenerator::_indent_string() const{
    return std::string(_indent_level * 4, ' ');
}
