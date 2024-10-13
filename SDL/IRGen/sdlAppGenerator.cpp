#include "../sim.h"

#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_os_ostream.h>
#include <llvm/Support/raw_ostream.h>

#include <fstream>
#include <string>

using namespace llvm;

namespace
{
struct GeneratedIR
{
    Module* module;
    Function* appFunction;
};

GeneratedIR generateIR(LLVMContext&);

void dumpModuleTo(std::string const&, Module*);

void interpretApp(GeneratedIR const&);
} // namespace

int main(int argc, char** argv)
{
    if (argc > 2)
    {
        errs() << "Usage: sdlAppGenerator [<output-file>]";
        return 0;
    }

    LLVMContext context;
    GeneratedIR generatedIR = generateIR(context);

    if (argc == 2)
    {
        dumpModuleTo(argv[1], generatedIR.module);
    }
    else
    {
        interpretApp(generatedIR);
    }

    return 0;
}

namespace
{
GeneratedIR generateIR(LLVMContext& context)
{
    Module* module = new Module("app.c", context);
    IRBuilder<> builder(context);

    FunctionType* simPutPixelFunctionType = FunctionType::get(
        builder.getVoidTy(),
        {Type::getInt32Ty(context), Type::getInt32Ty(context), Type::getInt32Ty(context)},
        false
    );
    FunctionCallee simPutPixelFunction =
        module->getOrInsertFunction("simPutPixel", simPutPixelFunctionType);

    FunctionType* simFlushFunctionType = FunctionType::get(builder.getVoidTy(), {}, false);
    FunctionCallee simFlushFunction = module->getOrInsertFunction("simFlush", simFlushFunctionType);

    FunctionType* memsetFunctionType = FunctionType::get(
        builder.getVoidTy(),
        {builder.getPtrTy(), builder.getInt8Ty(), builder.getInt64Ty(), builder.getInt1Ty()},
        false
    );
    FunctionCallee memsetFunction =
        module->getOrInsertFunction("llvm.memset.p0.i64", memsetFunctionType);

    FunctionType* memcpyFunctionType = FunctionType::get(
        builder.getVoidTy(),
        {builder.getPtrTy(), builder.getPtrTy(), builder.getInt64Ty(), builder.getInt1Ty()},
        false
    );
    FunctionCallee memcpyFunction =
        module->getOrInsertFunction("llvm.memcpy.p0.p0.i64", memcpyFunctionType);

    FunctionType* appFunctionType = FunctionType::get(builder.getVoidTy(), false);
    Function* appFunction =
        Function::Create(appFunctionType, Function::ExternalLinkage, "app", module);

    BasicBlock* bb0 = BasicBlock::Create(context, "", appFunction);
    BasicBlock* bb8 = BasicBlock::Create(context, "", appFunction);
    BasicBlock* bb15 = BasicBlock::Create(context, "", appFunction);
    BasicBlock* bb17 = BasicBlock::Create(context, "", appFunction);
    BasicBlock* bb19 = BasicBlock::Create(context, "", appFunction);
    BasicBlock* bb23 = BasicBlock::Create(context, "", appFunction);
    BasicBlock* bb29 = BasicBlock::Create(context, "", appFunction);
    BasicBlock* bb30 = BasicBlock::Create(context, "", appFunction);
    BasicBlock* bb32 = BasicBlock::Create(context, "", appFunction);
    BasicBlock* bb38 = BasicBlock::Create(context, "", appFunction);
    BasicBlock* bb47 = BasicBlock::Create(context, "", appFunction);
    BasicBlock* bb54 = BasicBlock::Create(context, "", appFunction);
    BasicBlock* bb55 = BasicBlock::Create(context, "", appFunction);
    BasicBlock* bb56 = BasicBlock::Create(context, "", appFunction);
    BasicBlock* bb64 = BasicBlock::Create(context, "", appFunction);
    BasicBlock* bb71 = BasicBlock::Create(context, "", appFunction);
    BasicBlock* bb84 = BasicBlock::Create(context, "", appFunction);
    BasicBlock* bb92 = BasicBlock::Create(context, "", appFunction);
    BasicBlock* bb93 = BasicBlock::Create(context, "", appFunction);
    BasicBlock* bb96 = BasicBlock::Create(context, "", appFunction);
    BasicBlock* bb102 = BasicBlock::Create(context, "", appFunction);
    BasicBlock* bb118 = BasicBlock::Create(context, "", appFunction);
    BasicBlock* bb122 = BasicBlock::Create(context, "", appFunction);
    BasicBlock* bb123 = BasicBlock::Create(context, "", appFunction);
    BasicBlock* bb126 = BasicBlock::Create(context, "", appFunction);

    builder.SetInsertPoint(bb0);
    Type* arrayElementType = builder.getInt32Ty();
    ArrayType* arrayType = ArrayType::get(ArrayType::get(arrayElementType, 64), 32);
    Value* val1 = builder.CreateAlloca(arrayType);
    Value* val2 = builder.CreateAlloca(arrayType);
    builder.CreateCall(
        memsetFunction,
        {val1, builder.getInt8(0), builder.getInt64(8192), builder.getInt1(false)}
    );
    builder.CreateCall(
        memsetFunction,
        {val2, builder.getInt8(0), builder.getInt64(8192), builder.getInt1(false)}
    );

    Value* val3 = builder.CreateGEP(
        arrayType,
        val1,
        {builder.getInt64(0), builder.getInt64(15), builder.getInt64(32)},
        "",
        true
    );
    Value* val4 = builder.CreateGEP(
        arrayType,
        val1,
        {builder.getInt64(0), builder.getInt64(15), builder.getInt64(33)},
        "",
        true
    );
    Value* val5 = builder.CreateGEP(
        arrayType,
        val1,
        {builder.getInt64(0), builder.getInt64(16), builder.getInt64(31)},
        "",
        true
    );
    Value* val6 = builder.CreateGEP(
        arrayType,
        val1,
        {builder.getInt64(0), builder.getInt64(16), builder.getInt64(32)},
        "",
        true
    );
    Value* val7 = builder.CreateGEP(
        arrayType,
        val1,
        {builder.getInt64(0), builder.getInt64(17), builder.getInt64(32)},
        "",
        true
    );
    builder.CreateStore(builder.getInt32(1), val3);
    builder.CreateStore(builder.getInt32(1), val4);
    builder.CreateStore(builder.getInt32(1), val5);
    builder.CreateStore(builder.getInt32(1), val6);
    builder.CreateStore(builder.getInt32(1), val7);

    builder.CreateBr(bb8);

    builder.SetInsertPoint(bb8);
    PHINode* val9 = builder.CreatePHI(builder.getInt64Ty(), 2);
    Value* val10 = builder.CreateICmpNE(val9, builder.getInt64(0));
    Value* val11 = builder.CreateAdd(val9, builder.getInt64(-1), "", false, true);
    Value* val12 = builder.CreateICmpEQ(val9, builder.getInt64(31));
    Value* val13 = builder.CreateICmpEQ(val9, builder.getInt64(31));
    Value* val14 = builder.CreateAdd(val9, builder.getInt64(1), "", true, true);
    builder.CreateBr(bb19);

    builder.SetInsertPoint(bb15);
    Value* val16 = builder.CreateICmpEQ(val14, builder.getInt64(32));
    builder.CreateCondBr(val16, bb118, bb17);

    builder.SetInsertPoint(bb17);
    PHINode* val18 = builder.CreatePHI(builder.getInt64Ty(), 2);
    builder.CreateBr(bb8);

    builder.SetInsertPoint(bb19);
    PHINode* val20 = builder.CreatePHI(builder.getInt64Ty(), 2);
    Value* val21 = builder.CreateICmpNE(val20, builder.getInt64(0));
    Value* val22 = builder.CreateAnd(val10, val21);
    builder.CreateCondBr(val22, bb23, bb29);

    builder.SetInsertPoint(bb23);
    Value* val24 = builder.CreateAdd(val20, builder.getInt64(-1), "", false, true);
    Value* val25 =
        builder.CreateGEP(arrayType, val1, {builder.getInt64(0), val11, val24}, "", true);
    Value* val26 = builder.CreateLoad(builder.getInt32Ty(), val25);
    Value* val27 = builder.CreateICmpNE(val26, builder.getInt32(0));
    Value* val28 = builder.CreateZExt(val27, builder.getInt32Ty());
    builder.CreateBr(bb38);

    builder.SetInsertPoint(bb29);
    builder.CreateCondBr(val10, bb38, bb30);

    builder.SetInsertPoint(bb30);
    Value* val31 = builder.CreateICmpEQ(val20, builder.getInt64(63));
    builder.CreateCondBr(val31, bb64, bb32);

    builder.SetInsertPoint(bb32);
    Value* val33 = builder.CreateAdd(val20, builder.getInt64(1), "", true, true);
    Value* val34 = builder.CreateGEP(arrayType, val1, {builder.getInt64(0), val9, val33}, "", true);
    Value* val35 = builder.CreateLoad(builder.getInt32Ty(), val34);
    Value* val36 = builder.CreateICmpNE(val35, builder.getInt32(0));
    Value* val37 = builder.CreateZExt(val36, builder.getInt32Ty());
    builder.CreateBr(bb71);

    builder.SetInsertPoint(bb38);
    PHINode* val39 = builder.CreatePHI(builder.getInt32Ty(), 2);
    Value* val40 =
        builder.CreateGEP(arrayType, val1, {builder.getInt64(0), val11, val20}, "", true);
    Value* val41 = builder.CreateLoad(builder.getInt32Ty(), val40);
    Value* val42 = builder.CreateICmpNE(val41, builder.getInt32(0));
    Value* val43 = builder.CreateZExt(val42, builder.getInt32Ty());
    Value* val44 = builder.CreateAdd(val39, val43, "", true, true);
    Value* val45 = builder.CreateICmpNE(val20, builder.getInt64(63));
    Value* val46 = builder.CreateAnd(val10, val45);
    builder.CreateCondBr(val46, bb47, bb54);

    builder.SetInsertPoint(bb47);
    Value* val48 = builder.CreateAdd(val20, builder.getInt64(1), "", true, true);
    Value* val49 =
        builder.CreateGEP(arrayType, val1, {builder.getInt64(0), val11, val48}, "", true);
    Value* val50 = builder.CreateLoad(builder.getInt32Ty(), val49);
    Value* val51 = builder.CreateICmpNE(val50, builder.getInt32(0));
    Value* val52 = builder.CreateZExt(val51, builder.getInt32Ty());
    Value* val53 = builder.CreateAdd(val44, val52, "", true, true);
    builder.CreateBr(bb56);

    builder.SetInsertPoint(bb54);
    builder.CreateCondBr(val45, bb56, bb55);

    builder.SetInsertPoint(bb55);
    builder.CreateCondBr(val12, bb93, bb64);

    builder.SetInsertPoint(bb56);
    PHINode* val57 = builder.CreatePHI(builder.getInt32Ty(), 2);
    Value* val58 = builder.CreateAdd(val20, builder.getInt64(1), "", true, true);
    Value* val59 = builder.CreateGEP(arrayType, val1, {builder.getInt64(0), val9, val58}, "", true);
    Value* val60 = builder.CreateLoad(builder.getInt32Ty(), val59);
    Value* val61 = builder.CreateICmpNE(val60, builder.getInt32(0));
    Value* val62 = builder.CreateZExt(val61, builder.getInt32Ty());
    Value* val63 = builder.CreateAdd(val57, val62, "", true, true);
    builder.CreateCondBr(val13, bb92, bb71);

    builder.SetInsertPoint(bb64);
    PHINode* val65 = builder.CreatePHI(builder.getInt32Ty(), 2);
    Value* val66 =
        builder.CreateGEP(arrayType, val1, {builder.getInt64(0), val14, val20}, "", true);
    Value* val67 = builder.CreateLoad(builder.getInt32Ty(), val66);
    Value* val68 = builder.CreateICmpNE(val67, builder.getInt32(0));
    Value* val69 = builder.CreateZExt(val68, builder.getInt32Ty());
    Value* val70 = builder.CreateAdd(val65, val69, "", true, true);
    builder.CreateBr(bb84);

    builder.SetInsertPoint(bb71);
    PHINode* val72 = builder.CreatePHI(builder.getInt32Ty(), 2);
    PHINode* val73 = builder.CreatePHI(builder.getInt64Ty(), 2);
    Value* val74 =
        builder.CreateGEP(arrayType, val1, {builder.getInt64(0), val14, val73}, "", true);
    Value* val75 = builder.CreateLoad(builder.getInt32Ty(), val74);
    Value* val76 = builder.CreateICmpNE(val75, builder.getInt32(0));
    Value* val77 = builder.CreateZExt(val76, builder.getInt32Ty());
    Value* val78 = builder.CreateAdd(val72, val77, "", true, true);
    Value* val79 =
        builder.CreateGEP(arrayType, val1, {builder.getInt64(0), val14, val20}, "", true);
    Value* val80 = builder.CreateLoad(builder.getInt32Ty(), val79);
    Value* val81 = builder.CreateICmpNE(val80, builder.getInt32(0));
    Value* val82 = builder.CreateZExt(val81, builder.getInt32Ty());
    Value* val83 = builder.CreateAdd(val78, val82, "", true, true);
    builder.CreateCondBr(val21, bb84, bb102);

    builder.SetInsertPoint(bb84);
    PHINode* val85 = builder.CreatePHI(builder.getInt32Ty(), 2);
    Value* val86 = builder.CreateAdd(val20, builder.getInt64(-1), "", false, true);
    Value* val87 =
        builder.CreateGEP(arrayType, val1, {builder.getInt64(0), val14, val86}, "", true);
    Value* val88 = builder.CreateLoad(builder.getInt32Ty(), val87);
    Value* val89 = builder.CreateICmpNE(val88, builder.getInt32(0));
    Value* val90 = builder.CreateZExt(val89, builder.getInt32Ty());
    Value* val91 = builder.CreateAdd(val85, val90, "", true, true);
    builder.CreateBr(bb96);

    builder.SetInsertPoint(bb92);
    builder.CreateCondBr(val21, bb93, bb102);

    builder.SetInsertPoint(bb93);
    PHINode* val94 = builder.CreatePHI(builder.getInt32Ty(), 2);
    Value* val95 = builder.CreateAdd(val20, builder.getInt64(-1), "", false, true);
    builder.CreateBr(bb96);

    builder.SetInsertPoint(bb96);
    PHINode* val97 = builder.CreatePHI(builder.getInt64Ty(), 2);
    PHINode* val98 = builder.CreatePHI(builder.getInt32Ty(), 2);
    Value* val99 = builder.CreateGEP(arrayType, val1, {builder.getInt64(0), val9, val97}, "", true);
    Value* val100 = builder.CreateLoad(builder.getInt32Ty(), val99);
    Value* val101 = builder.CreateICmpNE(val100, builder.getInt32(0));
    builder.CreateBr(bb102);

    builder.SetInsertPoint(bb102);
    PHINode* val103 = builder.CreatePHI(builder.getInt32Ty(), 3);
    PHINode* val104 = builder.CreatePHI(builder.getInt1Ty(), 3);
    Value* val105 = builder.CreateZExt(val104, builder.getInt32Ty());
    Value* val106 = builder.CreateAdd(val103, val105, "", true, true);
    Value* val107 =
        builder.CreateGEP(arrayType, val1, {builder.getInt64(0), val9, val20}, "", true);
    Value* val108 = builder.CreateLoad(builder.getInt32Ty(), val107);
    Value* val109 = builder.CreateICmpEQ(val108, builder.getInt32(0));
    Value* val110 = builder.CreateICmpEQ(val106, builder.getInt32(3));
    Value* val111 = builder.CreateAnd(val106, builder.getInt32(-2));
    Value* val112 = builder.CreateICmpEQ(val111, builder.getInt32(2));
    Value* val113 = builder.CreateSelect(val109, val110, val112);
    Value* val114 = builder.CreateZExt(val113, builder.getInt32Ty());
    Value* val115 =
        builder.CreateGEP(arrayType, val2, {builder.getInt64(0), val9, val20}, "", true);
    builder.CreateStore(val114, val115);
    Value* val116 = builder.CreateAdd(val20, builder.getInt64(1), "", true, true);
    Value* val117 = builder.CreateICmpEQ(val116, builder.getInt64(64));
    builder.CreateCondBr(val117, bb15, bb19);

    builder.SetInsertPoint(bb118);
    PHINode* val119 = builder.CreatePHI(builder.getInt32Ty(), 2);
    Value* val120 = builder.CreateLShr(val119, builder.getInt32(3));
    Value* val121 = builder.CreateZExt(val120, builder.getInt64Ty(), "", true);
    builder.CreateBr(bb126);

    builder.SetInsertPoint(bb122);
    builder.CreateCall(simFlushFunction);
    builder.CreateCall(
        memcpyFunction,
        {val1, val2, builder.getInt64(8192), builder.getInt1(false)}
    );
    builder.CreateBr(bb17);

    builder.SetInsertPoint(bb123);
    Value* val124 = builder.CreateAdd(val119, builder.getInt32(1), "", true, true);
    Value* val125 = builder.CreateICmpEQ(val124, builder.getInt32(256));
    builder.CreateCondBr(val125, bb122, bb118);

    builder.SetInsertPoint(bb126);
    PHINode* val127 = builder.CreatePHI(builder.getInt32Ty(), 2);
    Value* val128 = builder.CreateLShr(val127, builder.getInt32(3));
    Value* val129 = builder.CreateZExt(val128, builder.getInt64Ty(), "", true);
    Value* val130 =
        builder.CreateGEP(arrayType, val1, {builder.getInt64(0), val121, val129}, "", true);
    Value* val131 = builder.CreateLoad(builder.getInt32Ty(), val130);
    Value* val132 = builder.CreateICmpEQ(val131, builder.getInt32(0));
    Value* val133 = builder.CreateSelect(val132, builder.getInt32(-16777216), builder.getInt32(-1));
    builder.CreateCall(simPutPixelFunction, {val127, val119, val133});
    Value* val134 = builder.CreateAdd(val127, builder.getInt32(1), "", true, true);
    Value* val135 = builder.CreateICmpEQ(val134, builder.getInt32(512));
    builder.CreateCondBr(val135, bb123, bb126);

    val9->addIncoming(builder.getInt64(0), bb0);
    val9->addIncoming(val18, bb17);

    val18->addIncoming(val14, bb15);
    val18->addIncoming(builder.getInt64(0), bb122);

    val20->addIncoming(builder.getInt64(0), bb8);
    val20->addIncoming(val116, bb102);

    val39->addIncoming(val28, bb23);
    val39->addIncoming(builder.getInt32(0), bb29);

    val57->addIncoming(val53, bb47);
    val57->addIncoming(val44, bb54);

    val65->addIncoming(builder.getInt32(0), bb30);
    val65->addIncoming(val44, bb55);

    val72->addIncoming(val37, bb32);
    val72->addIncoming(val63, bb56);

    val73->addIncoming(val33, bb32);
    val73->addIncoming(val58, bb56);

    val85->addIncoming(val70, bb64);
    val85->addIncoming(val83, bb71);

    val94->addIncoming(val63, bb92);
    val94->addIncoming(val44, bb55);

    val97->addIncoming(val95, bb93);
    val97->addIncoming(val86, bb84);

    val98->addIncoming(val94, bb93);
    val98->addIncoming(val91, bb84);

    val103->addIncoming(val63, bb92);
    val103->addIncoming(val98, bb96);
    val103->addIncoming(val83, bb71);

    val104->addIncoming(builder.getInt1(false), bb92);
    val104->addIncoming(val101, bb96);
    val104->addIncoming(builder.getInt1(false), bb71);

    val119->addIncoming(val124, bb123);
    val119->addIncoming(builder.getInt32(0), bb15);

    val127->addIncoming(builder.getInt32(0), bb118);
    val127->addIncoming(val134, bb126);

    return {module, appFunction};
}

void dumpModuleTo(std::string const& filename, Module* module)
{
    std::ofstream output(filename);
    raw_os_ostream output_raw_ostream(output);
    module->print(output_raw_ostream, nullptr);
}

struct FunctionCreator
{
    void* operator()(std::string const& functionName)
    {
        if (functionName == "simPutPixel")
        {
            return reinterpret_cast<void*>(simPutPixel);
        }
        if (functionName == "simFlush")
        {
            return reinterpret_cast<void*>(simFlush);
        }
        return nullptr;
    }
};

void interpretApp(GeneratedIR const& generatedIR)
{
    InitializeNativeTarget();
    InitializeNativeTargetAsmPrinter();

    ExecutionEngine* engine = EngineBuilder(std::unique_ptr<Module>(generatedIR.module)).create();
    engine->InstallLazyFunctionCreator(FunctionCreator());
    engine->finalizeObject();

    simInit();
    engine->runFunction(generatedIR.appFunction, {});
    simExit();
}
} // namespace
