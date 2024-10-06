#include <llvm/IR/IRBuilder.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/PassPlugin.h>

using namespace llvm;

namespace
{
bool isLoggerFunction(StringRef name)
{
    return name == "logInstructionWithUser" || name == "initializeLogger" ||
           name == "terminateLogger";
}

bool isMainFunction(Function& function)
{
    return function.getName() == "main";
}

struct MyModPass : public PassInfoMixin<MyModPass>
{
    PreservedAnalyses run(Module& module, ModuleAnalysisManager& AM)
    {
        for (auto& function : module)
        {
            if (isLoggerFunction(function.getName()) || function.isDeclaration())
            {
                continue;
            }

            // Prepare builder for IR modification
            LLVMContext& context = function.getContext();
            IRBuilder<> builder(context);
            Type* voidType = Type::getVoidTy(context);

            // Prepare initializeLogger function
            ArrayRef<Type*> initializeLoggerParameterTypes = {};
            FunctionType* initializeLoggerFunctionType =
                FunctionType::get(voidType, initializeLoggerParameterTypes, false);
            FunctionCallee initializeLoggerFunction =
                module.getOrInsertFunction("initializeLogger", initializeLoggerFunctionType);

            // Prepare terminateLogger function
            ArrayRef<Type*> terminateLoggerParameterTypes = {};
            FunctionType* terminateLoggerFunctionType =
                FunctionType::get(voidType, terminateLoggerParameterTypes, false);
            FunctionCallee terminateLoggerFunction =
                module.getOrInsertFunction("terminateLogger", terminateLoggerFunctionType);

            // Prepare logInstructionWithUser function
            ArrayRef<Type*> logInstructionWithUserParameterTypes = {
                builder.getInt64Ty(),
                builder.getInt8Ty()->getPointerTo(),
                builder.getInt64Ty(),
                builder.getInt8Ty()->getPointerTo()
            };
            FunctionType* logInstructionWithUserFunctionType =
                FunctionType::get(voidType, logInstructionWithUserParameterTypes, false);
            FunctionCallee logInstructionWithUserFunction = module.getOrInsertFunction(
                "logInstructionWithUser",
                logInstructionWithUserFunctionType
            );

            // Initialize the logger at the beginning of main
            if (isMainFunction(function))
            {
                BasicBlock& entryBB = function.getEntryBlock();
                builder.SetInsertPoint(&entryBB.front());
                builder.CreateCall(initializeLoggerFunction, {});
            }

            for (auto& block : function)
            {
                for (auto& instruction : block)
                {
                    if (auto* ret = dyn_cast<ReturnInst>(&instruction))
                    {
                        // Terminate the logger at the end of main
                        if (isMainFunction(function))
                        {
                            builder.SetInsertPoint(ret);
                            builder.CreateCall(terminateLoggerFunction, {});
                        }
                    }
                    else if (strcmp(instruction.getOpcodeName(), "phi") != 0)
                    {
                        // Log the instruction along with all of its instruction users
                        builder.SetInsertPoint(&instruction);
                        builder.SetInsertPoint(&block, ++builder.GetInsertPoint());

                        Value* instructionAddress =
                            ConstantInt::get(builder.getInt64Ty(), (int64_t) (&instruction));
                        Value* instructionName =
                            builder.CreateGlobalStringPtr(instruction.getOpcodeName());

                        for (auto& use : instruction.uses())
                        {
                            if (auto* user = dyn_cast<Instruction>(use.getUser()))
                            {
                                Value* userAddress =
                                    ConstantInt::get(builder.getInt64Ty(), (int64_t) (&user));
                                Value* userName =
                                    builder.CreateGlobalStringPtr(user->getOpcodeName());
                                Value* args[] =
                                    {instructionAddress, instructionName, userAddress, userName};
                                builder.CreateCall(logInstructionWithUserFunction, args);
                            }
                        }
                    }
                }
            }
        }
        return PreservedAnalyses::none();
    };
};

PassPluginLibraryInfo getPassPluginInfo()
{
    const auto callback = [](PassBuilder& PB)
    {
        PB.registerOptimizerLastEPCallback(
            [&](ModulePassManager& MPM, auto)
            {
                MPM.addPass(MyModPass{});
                return true;
            }
        );
    };

    return {LLVM_PLUGIN_API_VERSION, "InstructionWindowAnalyzerPlugin", "0.0.1", callback};
};
} // namespace

extern "C" LLVM_ATTRIBUTE_WEAK PassPluginLibraryInfo llvmGetPassPluginInfo()
{
    return getPassPluginInfo();
}
