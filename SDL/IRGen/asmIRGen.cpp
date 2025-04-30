#include "../sim.h"
#include "isaBuilder.cpp"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"

using namespace llvm;

class InstructionCreator
{
  public:
    InstructionCreator(Module* module)
        : module(module)
    {
    }

    auto createAssignImm()
    {
        return [&](IRBuilder<>& builder, Register target, Immediate value)
        {
            setRegisterValue(builder, target, builder.getInt32(value.value));
        };
    }

    auto createAssignReg()
    {
        return [&](IRBuilder<>& builder, Register target, Register value)
        {
            setRegisterValue(builder, target, getRegisterValue(builder, value));
        };
    }

    auto createAddImm()
    {
        return [&](IRBuilder<>& builder, Register target, Immediate value)
        {
            Value* targetValue = getRegisterValue(builder, target);
            Value* result = builder.CreateAdd(targetValue, builder.getInt32(value.value));
            setRegisterValue(builder, target, result);
        };
    }

    auto createAddReg()
    {
        return [&](IRBuilder<>& builder, Register target, Register value)
        {
            Value* targetValue = getRegisterValue(builder, target);
            Value* valueValue = getRegisterValue(builder, value);
            Value* result = builder.CreateAdd(targetValue, valueValue);
            setRegisterValue(builder, target, result);
        };
    }

    auto createSubImm()
    {
        return [&](IRBuilder<>& builder, Register target, Immediate value)
        {
            Value* targetValue = getRegisterValue(builder, target);
            Value* result = builder.CreateSub(targetValue, builder.getInt32(value.value));
            setRegisterValue(builder, target, result);
        };
    }

    auto createSubReg()
    {
        return [&](IRBuilder<>& builder, Register target, Register value)
        {
            Value* targetValue = getRegisterValue(builder, target);
            Value* valueValue = getRegisterValue(builder, value);
            Value* result = builder.CreateSub(targetValue, valueValue);
            setRegisterValue(builder, target, result);
        };
    }

    auto createMulImm()
    {
        return [&](IRBuilder<>& builder, Register target, Immediate value)
        {
            Value* targetValue = getRegisterValue(builder, target);
            Value* result = builder.CreateMul(targetValue, builder.getInt32(value.value));
            setRegisterValue(builder, target, result);
        };
    }

    auto createMulReg()
    {
        return [&](IRBuilder<>& builder, Register target, Register value)
        {
            Value* targetValue = getRegisterValue(builder, target);
            Value* valueValue = getRegisterValue(builder, value);
            Value* result = builder.CreateMul(targetValue, valueValue);
            setRegisterValue(builder, target, result);
        };
    }

    auto createDivImm()
    {
        return [&](IRBuilder<>& builder, Register target, Immediate value)
        {
            Value* targetValue = getRegisterValue(builder, target);
            Value* result = builder.CreateSDiv(targetValue, builder.getInt32(value.value));
            setRegisterValue(builder, target, result);
        };
    }

    auto createDivReg()
    {
        return [&](IRBuilder<>& builder, Register target, Register value)
        {
            Value* targetValue = getRegisterValue(builder, target);
            Value* valueValue = getRegisterValue(builder, value);
            Value* result = builder.CreateSDiv(targetValue, valueValue);
            setRegisterValue(builder, target, result);
        };
    }

    auto createXor()
    {
        return [&](IRBuilder<>& builder, Register target, Register value)
        {
            Value* targetValue = getRegisterValue(builder, target);
            Value* valueValue = getRegisterValue(builder, value);
            Value* result = builder.CreateXor(targetValue, valueValue);
            setRegisterValue(builder, target, result);
        };
    }

    auto createCmpEImm()
    {
        return [&](IRBuilder<>& builder, Register target, Immediate value)
        {
            Value* targetValue = getRegisterValue(builder, target);
            Value* comparison = builder.CreateICmpEQ(targetValue, builder.getInt32(value.value));
            setFlagValue(builder, comparison);
        };
    }

    auto createCmpNeImm()
    {
        return [&](IRBuilder<>& builder, Register target, Immediate value)
        {
            Value* targetValue = getRegisterValue(builder, target);
            Value* comparison = builder.CreateICmpNE(targetValue, builder.getInt32(value.value));
            setFlagValue(builder, comparison);
        };
    }

    auto createCmpLtImm()
    {
        return [&](IRBuilder<>& builder, Register target, Immediate value)
        {
            Value* targetValue = getRegisterValue(builder, target);
            Value* comparison = builder.CreateICmpSLT(targetValue, builder.getInt32(value.value));
            setFlagValue(builder, comparison);
        };
    }

    auto createCmpGtImm()
    {
        return [&](IRBuilder<>& builder, Register target, Immediate value)
        {
            Value* targetValue = getRegisterValue(builder, target);
            Value* comparison = builder.CreateICmpSGT(targetValue, builder.getInt32(value.value));
            setFlagValue(builder, comparison);
        };
    }

    auto createStoreImm()
    {
        return [&](IRBuilder<>& builder, Register pointer, Immediate value)
        {
            builder.CreateStore(
                builder.getInt32(value.value),
                getMemoryPointerAt(builder, pointer)
            );
        };
    }

    auto createStoreReg()
    {
        return [&](IRBuilder<>& builder, Register pointer, Register value)
        {
            builder.CreateStore(
                getRegisterValue(builder, value),
                getMemoryPointerAt(builder, pointer)
            );
        };
    }

    auto createLoad()
    {
        return [&](IRBuilder<>& builder, Register target, Register pointer)
        {
            Value* memoryPointer = getMemoryPointerAt(builder, pointer);
            Value* memoryElementValue = builder.CreateLoad(builder.getInt32Ty(), memoryPointer);
            setRegisterValue(builder, target, memoryElementValue);
        };
    }

    auto createPutPxImm()
    {
        return [&](IRBuilder<>& builder, Register x, Register y, Immediate argb)
        {
            FunctionType* simPutPixelFunctionType = FunctionType::get(
                builder.getVoidTy(),
                {builder.getInt32Ty(), builder.getInt32Ty(), builder.getInt32Ty()},
                false
            );
            FunctionCallee simPutPixelFunction =
                module->getOrInsertFunction("simPutPixel", simPutPixelFunctionType);
            builder.CreateCall(
                simPutPixelFunction,
                {getRegisterValue(builder, x),
                 getRegisterValue(builder, y),
                 builder.getInt32(argb.value)}
            );
        };
    }

    auto createPutPxReg()
    {
        return [&](IRBuilder<>& builder, Register x, Register y, Register argb)
        {
            FunctionType* simPutPixelFunctionType = FunctionType::get(
                builder.getVoidTy(),
                {builder.getInt32Ty(), builder.getInt32Ty(), builder.getInt32Ty()},
                false
            );
            FunctionCallee simPutPixelFunction =
                module->getOrInsertFunction("simPutPixel", simPutPixelFunctionType);
            builder.CreateCall(
                simPutPixelFunction,
                {getRegisterValue(builder, x),
                 getRegisterValue(builder, y),
                 getRegisterValue(builder, argb)}
            );
        };
    }

    auto createFlush()
    {
        return [&](IRBuilder<>& builder)
        {
            FunctionType* simFlushFunctionType = FunctionType::get(builder.getVoidTy(), {}, false);
            FunctionCallee simFlushFunction =
                module->getOrInsertFunction("simFlush", simFlushFunctionType);
            builder.CreateCall(simFlushFunction);
        };
    }

  private:
    Module* module;

    Value* getRegisterPointer(IRBuilder<>& builder, Register reg)
    {
        GlobalVariable* regFile = module->getNamedGlobal("regFile");
        return builder.CreateStructGEP(regFile->getValueType(), regFile, reg.index);
    }

    Value* getRegisterValue(IRBuilder<>& builder, Register reg)
    {
        return builder.CreateLoad(builder.getInt32Ty(), getRegisterPointer(builder, reg));
    }

    void setRegisterValue(IRBuilder<>& builder, Register target, Value* value)
    {
        builder.CreateStore(value, getRegisterPointer(builder, target));
    }

    void setFlagValue(IRBuilder<>& builder, Value* value)
    {
        GlobalVariable* flagFile = module->getNamedGlobal("flagFile");
        Value* flagPointer = builder.CreateStructGEP(builder.getInt1Ty(), flagFile, 0);
        builder.CreateStore(value, flagPointer);
    }

    Value* getMemoryPointerAt(IRBuilder<>& builder, Register pointer)
    {
        GlobalVariable* memoryFile = module->getNamedGlobal("memoryFile");
        Value* pointerValue = getRegisterValue(builder, pointer);
        Value* memoryPointer = builder.CreateGEP(
            builder.getInt8Ty(),
            builder.CreateStructGEP(builder.getInt8Ty(), memoryFile, 0),
            pointerValue
        );
        return memoryPointer;
    }
};

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        outs() << "Usage: emulatedAsmIRGen <assembly input>\n";
        return 1;
    }

    LLVMContext context;
    Module* module = new Module("top", context);
    IsaBuilder isaBuilder(module);
    InstructionCreator creator(module);

    isaBuilder.addIRInstruction("exit", [](IRBuilder<>& builder) { builder.CreateRetVoid(); });

    isaBuilder.addIRInstruction("asgn", creator.createAssignImm());
    isaBuilder.addIRInstruction("asgn", creator.createAssignReg());

    isaBuilder.addIRInstruction("add", creator.createAddImm());
    isaBuilder.addIRInstruction("add", creator.createAddReg());

    isaBuilder.addIRInstruction("sub", creator.createSubImm());
    isaBuilder.addIRInstruction("sub", creator.createSubReg());

    isaBuilder.addIRInstruction("mul", creator.createMulImm());
    isaBuilder.addIRInstruction("mul", creator.createMulReg());

    isaBuilder.addIRInstruction("div", creator.createDivImm());
    isaBuilder.addIRInstruction("div", creator.createDivReg());

    isaBuilder.addIRInstruction("xor", creator.createXor());

    isaBuilder.addIRInstruction("cmpe", creator.createCmpEImm());
    isaBuilder.addIRInstruction("cmpne", creator.createCmpNeImm());
    isaBuilder.addIRInstruction("cmplt", creator.createCmpLtImm());
    isaBuilder.addIRInstruction("cmpgt", creator.createCmpGtImm());

    isaBuilder.addIRInstruction("store", creator.createStoreImm());
    isaBuilder.addIRInstruction("store", creator.createStoreReg());
    isaBuilder.addIRInstruction("load", creator.createLoad());

    isaBuilder.addIRInstruction("putpx", creator.createPutPxImm());
    isaBuilder.addIRInstruction("putpx", creator.createPutPxReg());
    isaBuilder.addIRInstruction("flush", creator.createFlush());

    isaBuilder.asmToIr(argv[1], false);

    Function* mainFunc = module->getFunction("main");

    outs() << "\n#[LLVM IR]:\n";
    module->print(outs(), nullptr);
    outs() << "\n";
    bool verif = verifyFunction(*mainFunc, &outs());
    outs() << "[VERIFICATION] " << (!verif ? "OK\n\n" : "FAIL\n\n");

    outs() << "\n#[Running code]\n";
    InitializeNativeTarget();
    InitializeNativeTargetAsmPrinter();

    ExecutionEngine* ee = EngineBuilder(std::unique_ptr<Module>(module)).create();
    ee->InstallLazyFunctionCreator(
        [](std::string const& functionName) -> void*
        {
            if (functionName == "simFlush")
            {
                return reinterpret_cast<void*>(simFlush);
            }
            if (functionName == "simPutPixel")
            {
                return reinterpret_cast<void*>(simPutPixel);
            }
            return nullptr;
        }
    );
    ee->finalizeObject();

    simInit();

    ee->runFunction(mainFunc, {});
    outs() << "#[Code was run]\n";

    simExit();
    return EXIT_SUCCESS;
}
