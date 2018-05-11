class Command {
  public:
    std::string action;
    std::vector<std::string> args;
    Command(std::string newAction, std::vector<std::string> newArgs) {
      action = newAction;
      args = newArgs;
    };
};

Command parseCommand(std::string command);
