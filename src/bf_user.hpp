#ifndef BF_USER_HG
#define BF_USER_HG

#include <string>
#include <vector>
#include <type_traits>

class bf_user {
public:
    virtual ~bf_user() = default;

    virtual void op_ptr_plus(size_t s) = 0;
    virtual void op_ptr_minus(size_t s) = 0;
    virtual void op_value_plus(size_t s) = 0;
    virtual void op_value_minus(size_t s) = 0;
    virtual void op_get() = 0;
    virtual void op_put() = 0;
    virtual void op_while_open() = 0;
    virtual void op_while_end() = 0;
};

template<class User, class... Args>
void run_brainfuck(const std::vector<std::string>& codes, Args&&... args) {
    static_assert(
            std::is_base_of<bf_user, User>(),
            "User template arg must be a dervating class of bf_user"
    );

    User user{ std::forward(args)... };
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
}

#endif //BF_USER_HG
