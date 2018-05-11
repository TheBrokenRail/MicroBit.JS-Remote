#include <string>
#include <vector>

class Command {
  public:
    std::string action;
    std::vector<std::string> args;
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
      args[0] = "":
      continue;
    }
    if (stage == 0) {
      action.append(command[i]);
    } else if (stage == 1) {
      if (command[i] == '"') {
        if (inString == 0) {
          inString = 1;
        } else {
          inString = 0;
        }
        continue;
      } else if (command[i] == ',' && inString == 0) {
        arg++;
        args[arg] = "";
      } else if (inString == 1) {
        args[arg].append(command[i]);
      }
    }
  }
  Command obj();
  obj.action = action;
  obj.args = args;
  return obj;
}
