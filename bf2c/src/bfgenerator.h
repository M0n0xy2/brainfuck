#ifndef BFGENERATOR_HG
#define BFGENERATOR_HG

#include <string>
#include <fstream>
#include <stdexcept>
#include <utility>

class BFGenerator {
public:
    BFGenerator(std::string path);
    ~BFGenerator();

    BFGenerator(const BFGenerator&) = delete;
    BFGenerator(BFGenerator&&) = delete;
    BFGenerator& operator=(const BFGenerator&) = delete;
    BFGenerator& operator=(BFGenerator&&) = delete;

    void generate_next_opcode(const std::string& opcode);
    bool ready() const;
private:
    void _op_ptr_plus(size_t s);
    void _op_ptr_minus(size_t s);
    void _op_value_plus(size_t s);
    void _op_value_minus(size_t s);
    void _op_get();
    void _op_put();
    void _op_while_open();
    void _op_while_end();
        
    std::string _indent_string() const;
    size_t _indent_level;
    std::ofstream _output_stream;
};

#endif //BFGENERATOR_HG
