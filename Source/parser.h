#include <string>
#include <vector>

class Command {
  public:
    std::string action;
    std::vector<std::string> args;
    Command(std::string newAction, std::vector<std::string> newArgs);
};

Command parseCommand(std::string command);
