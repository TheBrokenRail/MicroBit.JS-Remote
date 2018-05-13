#include <string>
#include "MicroBit.h"
#include "MicroBitUARTService.h"
#include <vector>

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

MicroBit uBit;
MicroBitUARTService *uart;

int connected = 0;
int bluetooth = 0;

void bluetoothSend(std::string msg) {
  uart->send(msg);
}

void onConnected(MicroBitEvent) {
  if (connected == 0) {
    bluetooth = 1;
    connected = 1;
    ManagedString eom(";");
    while (connected == 1 && bluetooth == 1) {
      std::string msg = uart->readUntil(eom);
      runCommand(parseCommand(msg), uBit, bluetoothSend);
    }
  }
}

void onDisconnected(MicroBitEvent) {
  bluetooth = 0;
  connected = 0;
}

int main() {
  // Initialise the micro:bit runtime.
  uBit.init();
  uBit.display.setDisplayMode(DISPLAY_MODE_GREYSCALE);

  uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_CONNECTED, onConnected);
  uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_DISCONNECTED, onDisconnected);
  uart = new MicroBitUARTService(*uBit.ble, 32, 32);

  // If main exits, there may still be other fibers running or registered event handlers etc.
  // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
  // sit in the idle task forever, in a power efficient sleep.
  release_fiber();
}
