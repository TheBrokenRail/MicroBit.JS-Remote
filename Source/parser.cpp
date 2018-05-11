#include <string>
#include <vector>
#include "parser.h"

Command::Command(std::string newAction, std::vector<std::string> newArgs) {
  action = newAction;
  args = newArgs;
}

Command parseCommand(std::string command) {
  std::string action = "";
  int stage = 0;
  int arg = 0;
  std::vector<std::string> args;
  int inString = 0;
  for (int i = 0; i < command.length(); i++) {
    if (command[i] == ':') {
      stage = 1;
      continue;
    }
    if (stage == 0) {
      action.append(1, command[i]);
    } else if (stage == 1) {
      if (command[i] == '"') {
        if (inString == 0) {
          inString = 1;
          args.push_back("");
        } else {
          inString = 0;
        }
        continue;
      } else if (command[i] == ',' && inString == 0) {
        arg++;
      } else if (inString == 1) {
        args[arg].append(1, command[i]);
      }
    }
  }
  return Command(action, args);
}
