let uBit = {
  sleep: ffi('void sleep(int)'),
  display: {
    scroll: ffi('void displayScroll(char*)'),
    print: ffi('void displayPrint(char*)'),
    clear: ffi('void displayClear()'),
    image: {
      setPixelValue: ffi('void displayImagePixelValue(int, int, int)')
    }
  },
  serial: {
    printf: ffi('void serialPrintf(char*)'),
    read: ffi('char* serialRead(int)')
  },
  messageBus: {
    listen: ffi('void messageBusListen(int, int, void (*)(userdata), userdata)')
  }
};
load = undefined;
print = undefined;
ffi = undefined;
ffi_cb_free = undefined;
getMJS = undefined;
uBit.display.scroll('Ready');
