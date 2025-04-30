#include "types.h"
#include "user.h"

int main() {
  printf(1, "Rebooting...\n"); // 1 = stdout
  sleep(2);
  reboot();
  printf(1, "If you see this, reboot failed.\n");
  exit(); // no arguments
}
