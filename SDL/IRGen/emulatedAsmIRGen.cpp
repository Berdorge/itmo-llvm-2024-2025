#include "../sim.h"
#include "isaBuilder.cpp"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"

using namespace llvm;

uint32_t REG_FILE[REG_FILE_SIZE];
uint8_t MEMORY_FILE[MEMORY_FILE_SIZE];
bool FLAG_FILE = false;

void doXor(Register r1, Register r2)
{
    REG_FILE[r1.index] ^= REG_FILE[r2.index];
}

void doAssignImm(Register target, Immediate value)
{
    REG_FILE[target.index] = value.value;
}

void doAssignReg(Register r1, Register r2)
{
    int regValue = REG_FILE[r2.index];
    doAssignImm(r1, Immediate(regValue));
}

void doMulImm(Register target, Immediate value)
{
    REG_FILE[target.index] *= value.value;
}

void doMulReg(Register r1, Register r2)
{
    int regValue = REG_FILE[r2.index];
    doMulImm(r1, Immediate(regValue));
}

void doAddImm(Register target, Immediate value)
{
    REG_FILE[target.index] += value.value;
}

void doAddReg(Register r1, Register r2)
{
    int regValue = REG_FILE[r2.index];
    doAddImm(r1, Immediate(regValue));
}

void doSubImm(Register target, Immediate value)
{
    REG_FILE[target.index] -= value.value;
}

void doSubReg(Register r1, Register r2)
{
    int regValue = REG_FILE[r2.index];
    doSubImm(r1, Immediate(regValue));
}

void doDivImm(Register target, Immediate value)
{
    REG_FILE[target.index] /= value.value;
}

void doDivReg(Register r1, Register r2)
{
    int regValue = REG_FILE[r2.index];
    doDivImm(r1, Immediate(regValue));
}

void doCmpEImm(Register target, Immediate value)
{
    FLAG_FILE = REG_FILE[target.index] == value.value;
}

void doCmpNeImm(Register target, Immediate value)
{
    FLAG_FILE = REG_FILE[target.index] != value.value;
}

void doCmpLtImm(Register target, Immediate value)
{
    FLAG_FILE = REG_FILE[target.index] < value.value;
}

void doCmpGtImm(Register target, Immediate value)
{
    FLAG_FILE = REG_FILE[target.index] > value.value;
}

void doPutPxImm(Register x, Register y, Immediate argb)
{
    simPutPixel(REG_FILE[x.index], REG_FILE[y.index], argb.value);
}

void doPutPxReg(Register x, Register y, Register argb)
{
    int argbValue = REG_FILE[argb.index];
    doPutPxImm(x, y, Immediate(argbValue));
}

void doFlush()
{
    simFlush();
}

void doStoreImm(Register pointer, Immediate value)
{
    *reinterpret_cast<uint32_t*>(MEMORY_FILE + REG_FILE[pointer.index]) = value.value;
}

void doStoreReg(Register pointer, Register value)
{
    int regValue = REG_FILE[value.index];
    doStoreImm(pointer, Immediate(regValue));
}

void doLoad(Register target, Register pointer)
{
    REG_FILE[target.index] = *reinterpret_cast<uint32_t*>(MEMORY_FILE + REG_FILE[pointer.index]);
}

class InstructionCreator
{
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

    isaBuilder.addIRInstruction("exit", [](IRBuilder<>& builder) { builder.CreateRetVoid(); });

    isaBuilder.addEmulatedInstruction("asgn", doAssignImm);
    isaBuilder.addEmulatedInstruction("asgn", doAssignReg);

    isaBuilder.addEmulatedInstruction("add", doAddImm);
    isaBuilder.addEmulatedInstruction("add", doAddReg);

    isaBuilder.addEmulatedInstruction("sub", doSubImm);
    isaBuilder.addEmulatedInstruction("sub", doSubReg);

    isaBuilder.addEmulatedInstruction("mul", doMulImm);
    isaBuilder.addEmulatedInstruction("mul", doMulReg);

    isaBuilder.addEmulatedInstruction("div", doDivImm);
    isaBuilder.addEmulatedInstruction("div", doDivReg);

    isaBuilder.addEmulatedInstruction("xor", doXor);

    isaBuilder.addEmulatedInstruction("cmpe", doCmpEImm);
    isaBuilder.addEmulatedInstruction("cmpne", doCmpNeImm);
    isaBuilder.addEmulatedInstruction("cmplt", doCmpLtImm);
    isaBuilder.addEmulatedInstruction("cmpgt", doCmpGtImm);

    isaBuilder.addEmulatedInstruction("store", doStoreImm);
    isaBuilder.addEmulatedInstruction("store", doStoreReg);
    isaBuilder.addEmulatedInstruction("load", doLoad);

    isaBuilder.addEmulatedInstruction("putpx", doPutPxImm);
    isaBuilder.addEmulatedInstruction("putpx", doPutPxReg);
    isaBuilder.addEmulatedInstruction("flush", doFlush);

    isaBuilder.asmToIr(argv[1], true);

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
    isaBuilder.prepareExecutionEngine(ee);
    ee->addGlobalMapping(module->getNamedGlobal("regFile"), (void*) REG_FILE);
    ee->addGlobalMapping(module->getNamedGlobal("memoryFile"), (void*) MEMORY_FILE);
    ee->addGlobalMapping(module->getNamedGlobal("flagFile"), (void*) &FLAG_FILE);
    ee->finalizeObject();

    simInit();

    REG_FILE[REG_FILE_SIZE - 1] = MEMORY_FILE_SIZE;
    ee->runFunction(mainFunc, {});
    outs() << "#[Code was run]\n";

    simExit();
    return EXIT_SUCCESS;
}
