#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"

#include <fstream>

using namespace llvm;

int const REG_FILE_SIZE = 9;
int const MEMORY_FILE_SIZE = 32768;

struct Register
{
    int index;

    explicit Register(int index)
        : index(index)
    {
    }
};

struct Immediate
{
    int value;

    explicit Immediate(int value)
        : value(value)
    {
    }
};

template <typename Arg>
struct InstructionArgumentType;

template <>
struct InstructionArgumentType<Register>
{
    static Type* getLLVMType(LLVMContext& context)
    {
        return Type::getInt32Ty(context);
    }

    static Value* cppToLLVM(IRBuilder<>& builder, Register cppValue)
    {
        return builder.getInt32(cppValue.index);
    }
};

template <>
struct InstructionArgumentType<Immediate>
{
    static Type* getLLVMType(LLVMContext& context)
    {
        return Type::getInt32Ty(context);
    }

    static Value* cppToLLVM(IRBuilder<>& builder, Immediate cppValue)
    {
        return builder.getInt32(cppValue.value);
    }
};

template <typename Arg>
struct InstructionArgumentName;

template <>
struct InstructionArgumentName<Register>
{
    static constexpr std::string name = "reg";
};

template <>
struct InstructionArgumentName<Immediate>
{
    static constexpr std::string name = "imm";
};

template <typename Arg>
struct InstructionArgumentParser;

template <>
struct InstructionArgumentParser<Register>
{
    static Register parse(std::string const& str)
    {
        int index;
        if (str == "rsp")
        {
            index = REG_FILE_SIZE - 1;
        }
        else
        {
            index = std::stoi(str.substr(1));
        }
        return Register(index);
    }

    static bool canParse(std::string const& str)
    {
        return str[0] == 'r';
    }
};

template <>
struct InstructionArgumentParser<Immediate>
{
    static Immediate parse(std::string const& str)
    {
        int value = std::stoi(str);
        return Immediate(value);
    }

    static bool canParse(std::string const& str)
    {
        return std::all_of(str.begin(), str.end(), [](char ch) { return ch >= '0' && ch <= '9'; });
    }
};

class IsaBuilder
{
    struct InstructionFlavor
    {
        using Action = std::function<bool(IRBuilder<>&, std::vector<std::string>&)>;
        Action action;
    };

    template <typename T>
    struct FunctionArgs;

    template <typename T>
    struct FunctionArgs : FunctionArgs<decltype(&T::operator())>
    {
    };

    template <typename C, typename R, typename... Args>
    struct FunctionArgs<R (C::*)(Args...) const>
    {
        using type = std::tuple<Args...>;
    };

    template <typename T>
    struct RemoveFirstType;

    template <typename First, typename... Others>
    struct RemoveFirstType<std::tuple<First, Others...>>
    {
        using type = std::tuple<Others...>;
    };

  public:
    IsaBuilder(Module* module)
        : module(module)
    {
    }

    template <typename Action>
    void addIRInstruction(std::string const& mnemonic, Action action)
    {
        using ArgsTuple = typename RemoveFirstType<typename FunctionArgs<Action>::type>::type;

        InstructionFlavor flavor;

        flavor.action = createInstructionFlavorAction<ArgsTuple>(
            std::make_index_sequence<std::tuple_size_v<ArgsTuple>>(),
            action
        );

        instructions[mnemonic].push_back(flavor);
    }

    template <typename... Args>
    void addEmulatedInstruction(std::string const& mnemonic, void (*action)(Args...))
    {
        std::string functionName = getFunctionName<Args...>(mnemonic);

        LLVMContext& context = module->getContext();
        FunctionType* functionType = FunctionType::get(
            Type::getVoidTy(context),
            {InstructionArgumentType<Args>::getLLVMType(context)...},
            false
        );
        FunctionCallee function = module->getOrInsertFunction(functionName, functionType);

        addIRInstruction(
            mnemonic,
            [function](IRBuilder<>& builder, Args... args) {
                builder.CreateCall(
                    function,
                    {InstructionArgumentType<Args>::cppToLLVM(builder, args)...}
                );
            }
        );

        emulatedFunctions[functionName] = reinterpret_cast<void*>(action);
    }

    void asmToIr(std::string const& inputFileName, bool isEmulated)
    {
        std::ifstream input(inputFileName);

        LLVMContext& context = module->getContext();
        IRBuilder<> builder(context);

        ArrayType* regFileType = ArrayType::get(builder.getInt32Ty(), REG_FILE_SIZE);
        ArrayType* memoryFileType = ArrayType::get(builder.getInt8Ty(), MEMORY_FILE_SIZE);
        Type* flagFileType = builder.getInt1Ty();
        GlobalVariable* regFile;
        GlobalVariable* memoryFile;
        GlobalVariable* flagFile;

        if (isEmulated)
        {
            module->getOrInsertGlobal("regFile", regFileType);
            module->getOrInsertGlobal("memoryFile", memoryFileType);
            module->getOrInsertGlobal("flagFile", flagFileType);
            regFile = module->getNamedGlobal("regFile");
            memoryFile = module->getNamedGlobal("memoryFile");
            flagFile = module->getNamedGlobal("flagFile");
        }
        else
        {
            std::vector<Constant*> regFileValues(REG_FILE_SIZE, builder.getInt32(0));
            regFileValues.back() = builder.getInt32(MEMORY_FILE_SIZE);
            regFile = new GlobalVariable(
                *module,
                regFileType,
                false,
                GlobalValue::PrivateLinkage,
                ConstantArray::get(regFileType, regFileValues),
                "regFile"
            );
            memoryFile = new GlobalVariable(
                *module,
                memoryFileType,
                false,
                GlobalValue::PrivateLinkage,
                ConstantAggregateZero::get(memoryFileType),
                "memoryFile"
            );
            flagFile = new GlobalVariable(
                *module,
                flagFileType,
                false,
                GlobalValue::PrivateLinkage,
                ConstantAggregateZero::get(flagFileType),
                "flagFile"
            );
        }

        FunctionType* funcType = FunctionType::get(builder.getVoidTy(), false);
        Function* mainFunc = Function::Create(funcType, Function::ExternalLinkage, "main", module);

        std::string name;
        std::string arg;
        std::unordered_map<std::string, BasicBlock*> labels;
        std::vector<BasicBlock*> falseDestinations;

        int conditionalJumpsCount = 0;
        std::string line;
        while (std::getline(input, line))
        {
            std::vector<std::string> split = splitBySpaces(line);

            if (split.empty())
            {
                continue;
            }

            std::string name = split.front();

            if (split.size() == 1 && name.back() == ':')
            {
                name = name.substr(0, name.size() - 1);
                labels[name] = BasicBlock::Create(context, name, mainFunc);
                continue;
            }

            if (name == "cjmp")
            {
                std::string falseDestinationName =
                    "falseDestination" + std::to_string(conditionalJumpsCount);
                BasicBlock* falseDestination =
                    BasicBlock::Create(context, falseDestinationName, mainFunc);
                falseDestinations.push_back(falseDestination);
                ++conditionalJumpsCount;
                continue;
            }
        }

        input.close();
        input.open(inputFileName);

        conditionalJumpsCount = 0;
        while (getline(input, line))
        {
            std::vector<std::string> split = splitBySpaces(line);

            if (split.empty())
            {
                continue;
            }

            std::string name = split.front();

            if (split.size() == 1 && name.back() == ':')
            {
                name = name.substr(0, name.size() - 1);

                BasicBlock* insertBlock = builder.GetInsertBlock();
                if (insertBlock && !insertBlock->getTerminator())
                {
                    builder.CreateBr(labels[name]);
                }

                builder.SetInsertPoint(labels[name]);

                continue;
            }

            if (split.size() == 2 && name == "cjmp")
            {
                std::string labelName = split[1];
                Value* flag = builder.CreateLoad(builder.getInt1Ty(), flagFile);
                builder.CreateCondBr(
                    flag,
                    labels[labelName],
                    falseDestinations[conditionalJumpsCount]
                );
                builder.SetInsertPoint(falseDestinations[conditionalJumpsCount]);
                ++conditionalJumpsCount;
                continue;
            }

            if (split.size() == 2 && name == "jmp")
            {
                std::string labelName = split[1];
                builder.CreateBr(labels[labelName]);
                continue;
            }

            auto flavors = instructions.find(name);

            if (flavors == instructions.end())
            {
                outs() << "Unknown instruction " << name << ", ignoring it\n";
                continue;
            }

            split.erase(split.begin());

            bool parsedFlavor = false;
            for (auto flavor : flavors->second)
            {
                if (flavor.action(builder, split))
                {
                    parsedFlavor = true;
                    break;
                }
            }

            if (!parsedFlavor)
            {
                outs() << "Invalid args for instruction " << name << ", ignoring it\n";
            }
        }
    }

    void prepareExecutionEngine(ExecutionEngine* ee)
    {
        ee->InstallLazyFunctionCreator(
            [this](std::string const& functionName) -> void*
            {
                auto emulatedFunction = emulatedFunctions.find(functionName);

                if (emulatedFunction == emulatedFunctions.end())
                {
                    return nullptr;
                }

                return emulatedFunction->second;
            }
        );
    }

  private:
    Module* module;
    std::unordered_map<std::string, void*> emulatedFunctions;
    std::unordered_map<std::string, std::vector<InstructionFlavor>> instructions;

    template <typename... Args>
    std::string getFunctionName(std::string const& mnemonic)
    {
        return "do" + mnemonic + ((InstructionArgumentName<Args>::name) + ... + "");
    }

    template <typename ArgsTuple, std::size_t... Indices, typename Action>
    InstructionFlavor::Action
    createInstructionFlavorAction(std::index_sequence<Indices...>, Action action)
    {
        return [action](IRBuilder<>& builder, std::vector<std::string>& args)
        {
            if (args.size() != std::tuple_size_v<ArgsTuple>)
            {
                return false;
            }

            if (!(InstructionArgumentParser<std::tuple_element_t<Indices, ArgsTuple>>::canParse(
                      args[Indices]
                  ) &&
                  ...))
            {
                return false;
            }

            action(
                builder,
                InstructionArgumentParser<std::tuple_element_t<Indices, ArgsTuple>>::parse(
                    args[Indices]
                )...
            );
            return true;
        };
    }

    std::vector<std::string> splitBySpaces(std::string const& str)
    {
        std::vector<std::string> result;
        bool isCurrentlyInWord = false;

        for (char ch : str)
        {
            if (ch == ' ')
            {
                isCurrentlyInWord = false;
            }
            else
            {
                if (isCurrentlyInWord)
                {
                    result.back().push_back(ch);
                }
                else
                {
                    result.emplace_back(1, ch);
                    isCurrentlyInWord = true;
                }
            }
        }

        return result;
    }
};
