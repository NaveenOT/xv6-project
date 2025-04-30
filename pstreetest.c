#include "user.h"
#include "types.h"
#include "stat.h"
#include "fcntl.h"

// Declare getppid and get_state functions
int getppid(void); // To get the parent process ID
int getstate(void); // To get the process state

void print_process_info(char *label) {
    // Print the process information in a table format
    printf(1, "%s", label);
    printf(1, "+------------------------+-------------------+--------------------+-------------------+\n");
    printf(1, "|  PID: %-15d | Parent PID: %-12d | State: %-10d |\n",getpid(), getppid(), getstate());
    printf(1, "+------------------------+-------------------+--------------------+-------------------+\n");
}

int main(void) {
    // Create multiple processes and print process info
    printf(1, "Creating multiple processes...\n");

    // First child process
    int pid = fork();
    if (pid == 0) {
        // Inside child process
        print_process_info("Child Process");

        // Grandchild process
        pid = fork();
        if (pid == 0) {
            // Inside grandchild process
            print_process_info("Grandchild Process");
            exit();
        }
        wait(); // Wait for the grandchild to finish
        exit();
    }

    // Another child process
    pid = fork();
    if (pid == 0) {
        print_process_info("Another Child Process");
        exit();
    }

    // Parent process waits for all children to terminate
    for (int i = 0; i < 3; i++) {
        wait();  // Wait for each child process to finish
    }

    printf(1, "All child processes have completed.\n");

    exit();
}
