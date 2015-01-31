#ifndef BF_USER_HG
#define BF_USER_HG

#include <string>
#include <vector>

class bf_user {
public:
    virtual void start() = 0;
    virtual void finish() = 0;
    virtual void op_ptr_plus(size_t s) = 0;
    virtual void op_ptr_minus(size_t s) = 0;
    virtual void op_value_plus(size_t s) = 0;
    virtual void op_value_minus(size_t s) = 0;
    virtual void op_get() = 0;
    virtual void op_put() = 0;
    virtual void op_while_open() = 0;
    virtual void op_while_end() = 0;
};

void run_brainfuck(bf_user& user, const std::vector<std::string>& codes);

#endif //BF_USER_HG
