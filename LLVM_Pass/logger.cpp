#include <cstdint>
#include <fstream>
#include <set>
#include <utility>

std::ofstream* output;
std::set<std::pair<std::int64_t, std::int64_t>>* loggedInstructionUsers;

extern "C" void initializeLogger()
{
    output = new std::ofstream("SDL/stats/usedInstructions.txt");
    loggedInstructionUsers = new std::set<std::pair<std::int64_t, std::int64_t>>();
}

extern "C" void terminateLogger()
{
    delete output;
    output = nullptr;
    delete loggedInstructionUsers;
    loggedInstructionUsers = nullptr;
}

extern "C" void logInstructionWithUser(
    std::int64_t instructionId,
    char* instructionName,
    std::int64_t userId,
    char* userName
)
{
    auto instructionWithUserId = std::make_pair(instructionId, userId);
    if (loggedInstructionUsers->find(instructionWithUserId) == loggedInstructionUsers->end())
    {
        loggedInstructionUsers->insert(instructionWithUserId);
        *output << instructionName << " <- " << userName << std::endl;
    }
}
