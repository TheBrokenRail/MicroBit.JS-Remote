#include <string>
#include <vector>
#include "MicroBit.h"
#include "MicroBitUARTService.h"

MicroBit uBit;
MicroBitUARTService *uart;

class Command {
  public:
    std::string action;
    std::vector<std::string> args;
    Command(std::string newAction, std::vector<std::string> newArgs);
};

Command::Command(std::string newAction, std::vector<std::string> newArgs) {
  action = newAction;
  args = newArgs;
}

Command parseCommand(ManagedString commandTemp) {
  std::string command = commandTemp.toCharArray();
  std::string action = "";
  int stage = 0;
  std::vector<std::string> args;
  std::string temp = "";
  int inString = 0;
  for (unsigned int i = 0; i < command.length(); i++) {
    if (stage == 0 && command[i] == ':') {
      stage = 1;
    } else if (stage == 0) {
      action.push_back(command[i]);
    } else if (stage == 1) {
      if (command[i] == '"' && command[i - 1] != '\\') {
        if (inString == 0) {
          inString = 1;
        } else {
          inString = 0;
        }
      } else if ((command[i] == ',' || command[i] == ';') && inString == 0) {
        args.push_back(temp);
        temp = "";
      } else if (inString == 1) {
        temp.push_back(command[i]);
      }
    }
  }
  return Command(action, args);
}

void runCommand(Command command, void (*send)(std::string)) {
  send(".MSG:" + command.action + ";");
  send(".MSG:" + command.args[0] + ";");
  send(".MSG:" + command.args[1] + ";");
  if (command.action == "DISPLAY_PLOT") {
    uBit.display.image.setPixelValue(std::stoi(command.args[1]), std::stoi(command.args[2]), std::stoi(command.args[3]));
    send(".DONE:" + command.args[0] + ";");
  } else if (command.action == "DISPLAY_SCROLL") {
    uBit.display.scroll(command.args[1].c_str());
    send(".DONE:" + command.args[0] + ";");
  } else {
    send(".INVALID_COMMAND_ERROR:" + command.args[0] + ";");
  }
}

int connected = 0;

void bluetoothSend(std::string msg) {
  uart->send(msg.c_str());
}

void serialSend(std::string msg) {
  uBit.serial.printf(msg.c_str());
}

void onConnected(MicroBitEvent) {
  if (connected == 0) {
    connected = 1;
    while (connected == 1) {
      ManagedString msg = uart->readUntil(";");
      runCommand(parseCommand(msg), bluetoothSend);
    }
  }
}

void onDisconnected(MicroBitEvent) {
  connected = 0;
}

int main() {
  // Initialise the micro:bit runtime.
  uBit.init();
  uBit.display.setDisplayMode(DISPLAY_MODE_GREYSCALE);

  uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_CONNECTED, onConnected);
  uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_DISCONNECTED, onDisconnected);
  uart = new MicroBitUARTService(*uBit.ble, 32, 32);
  
  ManagedString buffer("");
  while (true) {
    ManagedString msg = uBit.serial.readUntil(";");
    serialSend(".MSG:" + std::string(msg.toCharArray()) + ";");
    if (std::string(msg.toCharArray()).back() == ';') {
      runCommand(parseCommand(buffer + msg), serialSend);
      buffer = "";
    } else {
      buffer = buffer + msg;
    }
  }

  // If main exits, there may still be other fibers running or registered event handlers etc.
  // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
  // sit in the idle task forever, in a power efficient sleep.
  release_fiber();
}
