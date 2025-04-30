#include "types.h"
#include "user.h"


int main(int argc, char *argv[]) {
    // Correct usage of printf, passing 1 for the file descriptor
    printf(1, "Shutting down...\n");

    // Call the shutdown system call
    shutdown();

    // Exit without arguments
    exit();
}
