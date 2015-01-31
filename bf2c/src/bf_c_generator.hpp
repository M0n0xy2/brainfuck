#ifndef BFGENERATOR_HG
#define BFGENERATOR_HG

#include <string>
#include <fstream>
#include <stdexcept>
#include <utility>

#include "bf_user.hpp"

class bf_c_generator : public bf_user {
public:
    bf_c_generator(std::string path);

    bool ready() const;

    virtual void start();
    virtual void finish();

    virtual void op_ptr_plus(size_t s);
    virtual void op_ptr_minus(size_t s);
    virtual void op_value_plus(size_t s);
    virtual void op_value_minus(size_t s);
    virtual void op_get();
    virtual void op_put();
    virtual void op_while_open();
    virtual void op_while_end();

private:
    std::string _indent_string() const;
    size_t _indent_level;
    std::ofstream _output_stream;
};

#endif //BFGENERATOR_HG
