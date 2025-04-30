#include "types.h"
#include "stat.h"
#include "user.h"

int main(void) {
  printf(1, "Uptime in seconds: %d\n", uptime_seconds());
  exit();
}
