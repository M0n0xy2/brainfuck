#include "bf_llvm_generator.hpp"

bf_llvm_generator::bf_llvm_generator()
    : _context(llvm::getGlobalContext()),
      _main_module("brainf**k program", _context),
      _builder(_context),
      _byte_type(llvm::Type::getInt8Ty(_context))
{
    _putchar_function = llvm::cast<llvm::Function>(
            _main_module.getOrInsertFunction(
                "putchar",
                llvm::Type::getVoidTy(_context),
                _byte_type,
                NULL
            )
    );
    _getchar_function = llvm::cast<llvm::Function>(
            _main_module.getOrInsertFunction(
                "getchar",
                _byte_type,
                NULL
            )
    );
    _main_function = llvm::cast<llvm::Function>(
            _main_module.getOrInsertFunction(
                "main",
                llvm::Type::getVoidTy(_context),
                NULL
            )
    );

    _putchar_function->setCallingConv(llvm::CallingConv::C);
    _getchar_function->setCallingConv(llvm::CallingConv::C);
    _main_function->setCallingConv(llvm::CallingConv::C);

    _memory_type = llvm::ArrayType::get(_byte_type, 30000);
    _memory = new llvm::GlobalVariable(
            _main_module,
            _memory_type,
            false,
            llvm::GlobalVariable::InternalLinkage,
            llvm::Constant::getNullValue(_memory_type),
            "m"
    );
    _memory_ptr = _builder.CreateConstInBoundsGEP2_32(_memory, 0, 0, "ptr");
    _main_block = llvm::BasicBlock::Create(_context, "entry", _main_function);
    _builder.SetInsertPoint(_main_block);
}

bf_llvm_generator::~bf_llvm_generator() {
}

void bf_llvm_generator::start() {
}

void bf_llvm_generator::finish() {
    _builder.CreateRetVoid();
    
    llvm::FunctionPassManager pm(&_main_module);
    pm.add(llvm::createPromoteMemoryToRegisterPass());
    pm.add(llvm::createBasicAliasAnalysisPass());
    pm.add(llvm::createInstructionCombiningPass());
    pm.add(llvm::createLICMPass());
    pm.add(llvm::createLoopDeletionPass());
    pm.add(llvm::createReassociatePass());
    pm.add(llvm::createConstantPropagationPass());
    pm.add(llvm::createConstantHoistingPass());
    
    pm.add(llvm::createIndVarSimplifyPass());
    pm.add(llvm::createBreakCriticalEdgesPass());
    pm.add(llvm::createDeadCodeEliminationPass());
    pm.add(llvm::createDeadInstEliminationPass());
    
    for(auto i = 0u; i < 3u; ++i) //just repeat
    {
        pm.add(llvm::createGVNPass());
        pm.add(llvm::createSCCPPass());
        pm.add(llvm::createCFGSimplificationPass());
        pm.add(llvm::createInstructionCombiningPass());
        pm.add(llvm::createAggressiveDCEPass());
        pm.add(llvm::createCFGSimplificationPass());
        pm.add(llvm::createDeadStoreEliminationPass());
    }
    
    pm.add(llvm::createConstantPropagationPass());
    pm.add(llvm::createInstructionCombiningPass());
    pm.add(llvm::createDeadCodeEliminationPass());
    pm.add(llvm::createDeadInstEliminationPass());
    


    llvm::verifyFunction(*_main_function);
    
    pm.doInitialization();
    pm.doFinalization();
    pm.run(*_main_function);
    _main_module.print(llvm::outs(), NULL);
}

void bf_llvm_generator::op_ptr_plus(size_t s) {
    _memory_ptr = _builder.CreateConstGEP1_32(_memory_ptr, s, "ptr");
}

void bf_llvm_generator::op_ptr_minus(size_t s) {
    _memory_ptr = _builder.CreateConstGEP1_32(_memory_ptr, -s, "ptr");
}

void bf_llvm_generator::op_value_plus(size_t s) {
    llvm::Value* val = _builder.CreateLoad(_memory_ptr);
    val = _builder.CreateAdd(val, llvm::ConstantInt::get(_byte_type, s));
    _builder.CreateStore(val, _memory_ptr);
}

void bf_llvm_generator::op_value_minus(size_t s) {
    llvm::Value* val = _builder.CreateLoad(_memory_ptr);
    val = _builder.CreateSub(val, llvm::ConstantInt::get(_byte_type, s));
    _builder.CreateStore(val, _memory_ptr);
}

void bf_llvm_generator::op_get() {
    llvm::Value* val = _builder.CreateCall(_getchar_function);
    _builder.CreateStore(val, _memory_ptr);
}

void bf_llvm_generator::op_put() {
    llvm::Value* val = _builder.CreateLoad(_memory_ptr);
    _builder.CreateCall(_putchar_function, val);
}

void bf_llvm_generator::op_while_open() {
    loop_layout loop;
    loop.Entry = _builder.GetInsertBlock();
    loop.Body = llvm::BasicBlock::Create(_context, "loop", _main_function);
    loop.Exit = llvm::BasicBlock::Create(_context, "exit", _main_function);

    llvm::Value* val = _builder.CreateLoad(_memory_ptr);
    val = _builder.CreateIsNotNull(val);
    _builder.CreateCondBr(val, loop.Body, loop.Exit);

    _builder.SetInsertPoint(loop.Exit);
    loop.DataPtrExit = _builder.CreatePHI(_memory_ptr->getType(), 2, "ptr");
    loop.DataPtrExit->addIncoming(_memory_ptr, loop.Entry);

    _builder.SetInsertPoint(loop.Body);
    loop.DataPtrBody = _builder.CreatePHI(_memory_ptr->getType(), 2, "ptr");
    loop.DataPtrBody->addIncoming(_memory_ptr, loop.Entry);

    _memory_ptr = loop.DataPtrBody;
    _loops.push(loop);
}

void bf_llvm_generator::op_while_end() {
    loop_layout loop = _loops.top();
    _loops.pop();

    loop.DataPtrBody->addIncoming(_memory_ptr, _builder.GetInsertBlock());
    loop.DataPtrExit->addIncoming(_memory_ptr, _builder.GetInsertBlock());

    llvm::Value* val = _builder.CreateLoad(_memory_ptr);
    val = _builder.CreateIsNotNull(val);
    _builder.CreateCondBr(val, loop.Body, loop.Exit);

    loop.Exit->moveAfter(_builder.GetInsertBlock());
    _memory_ptr = loop.DataPtrExit;
    _builder.SetInsertPoint(loop.Exit);
}
