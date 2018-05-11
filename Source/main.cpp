#include <cstring>
#include "MicroBit.h"
#include "MicroBitUARTService.h"
#include "parser.h"

MicroBit uBit;
MicroBitUARTService *uart;

int connected = 0;
int bluetooth = 0;

void run(ManagedString msg) {
  Command command = parseCommand(msg);
}

void onConnected(MicroBitEvent) {
  bluetooth = 1;
  connected = 1;
  // mobile app will send ASCII strings terminated with the colon character
  ManagedString eom(";");
  while (connected == 1 && bluetooth == 1) {
    ManagedString msg = uart->readUntil(eom);
    run(msg);
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
