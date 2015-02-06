#ifndef BF_LLVM_GENERATOR
#define BF_LLVM_GENERATOR

#include <stack>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/PassManager.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/JIT.h>
#include <llvm/LinkAllPasses.h>

#include "bf_user.hpp"

struct loop_layout {
    llvm::BasicBlock *Entry, *Body, *Exit;
    llvm::PHINode *DataPtrBody, *DataPtrExit;
};

class bf_llvm_generator : public bf_user {
public:
    bf_llvm_generator();
    virtual ~bf_llvm_generator();

    void op_ptr_plus(size_t s) override;
    void op_ptr_minus(size_t s) override;
    void op_value_plus(size_t s) override;
    void op_value_minus(size_t s) override;
    void op_get() override;
    void op_put() override;
    void op_while_open() override;
    void op_while_end() override;
private:
    llvm::LLVMContext& _context;
    llvm::Module _main_module;
    llvm::IRBuilder<> _builder;
    llvm::Type* _byte_type;
    llvm::BasicBlock* _main_block;

    llvm::Function* _putchar_function;
    llvm::Function* _getchar_function;
    llvm::Function* _main_function;

    llvm::ArrayType* _memory_type;
    llvm::GlobalVariable* _memory;
    llvm::Value* _memory_ptr;

    std::stack<loop_layout> _loops; 
};

#endif //BF_LLVM_GENERATOR
