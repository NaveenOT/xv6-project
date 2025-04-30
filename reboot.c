#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"

// Inline assembly to write to I/O port
void
reboot_hardware() {
  outb(0x64, 0xFE); // Send reboot command
}

int
sys_reboot(void) {
  reboot_hardware();
  return 0; // Never returns if reboot succeeds
}
