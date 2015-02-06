#include "bf_user.hpp"

void run_brainfuck(bf_user& user, const std::vector<std::string>& codes) {
    user.start();
    for(auto& code : codes) {
        if(code.empty())
            continue;

        auto op = code.front();
        auto size = code.size();
        switch(op) {
            case '+':
                user.op_value_plus(size);
                break;
            case '-':
                user.op_value_minus(size);
                break;
            case '>':
                user.op_ptr_plus(size);
                break;
            case '<':
                user.op_ptr_minus(size);
                break;
            case ',':
                for(auto i = 0u; i < size; ++i)
                    user.op_get();
                break;
            case '.':
                for(auto i = 0u; i < size; ++i)
                    user.op_put();
                break;
            case '[':
                for(auto i = 0u; i < size; ++i)
                    user.op_while_open();
                break;
            case ']':
                for(auto i = 0u; i < size; ++i)
                    user.op_while_end();
                break;
        }
    }
    user.finish();
}
