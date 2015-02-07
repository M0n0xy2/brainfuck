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
    virtual ~bf_c_generator();

    bool ready() const;

    void op_ptr_plus(size_t s) override;
    void op_ptr_minus(size_t s) override;
    void op_value_plus(size_t s) override;
    void op_value_minus(size_t s) override;
    void op_get() override;
    void op_put() override;
    void op_while_open() override;
    void op_while_end() override;

  private:
    std::string _indent_string() const;
    size_t _indent_level;
    std::ofstream _output_stream;
};

#endif // BFGENERATOR_HG
