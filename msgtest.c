#include "types.h"
#include "user.h"

// Custom function to copy strings safely
void safestrcpy(char *dst, const char *src, int n) {
    int i;
    for(i = 0; i < n - 1 && src[i] != '\0'; i++) {
        dst[i] = src[i];
    }
    dst[i] = '\0';  // Null-terminate the destination string
}

// Custom function to concatenate strings safely
void safestrcat(char *dst, const char *src, int n) {
    int i = 0;
    // Find the end of the destination string
    while(dst[i] != '\0') {
        i++;
    }
    // Copy characters from source to destination
    int j = 0;
    while(i < n - 1 && src[j] != '\0') {
        dst[i++] = src[j++];
    }
    dst[i] = '\0';  // Null-terminate the destination string
}

int main(int argc, char *argv[])
{
  int pid = fork();

  if (pid == 0) {
    // Child process: send message to parent
    sleep(10);  // Allow parent to be ready
    
    // Define the message and PID string buffer
    char msg[128];
    char pid_str[16];
    
    // Convert PID to string manually (very simple approach for single digit)
    pid_str[0] = '0' + getpid();  // Only works for single-digit PID for simplicity

    // Construct the message by appending the PID
    printf(1, "Child PID: %d\n", getpid());
    safestrcpy(msg, "Hi There, this is child with PID ", sizeof(msg));
    safestrcat(msg, pid_str, sizeof(msg));  // Append the PID to the message

    // Send the message
    if (send(msg) == 0) {
      printf(1, "Child: Message sent to parent\n");
    } else {
      printf(1, "Child: Failed to send message\n");
    }

    exit();
  } else {
    // Parent process: receive message from child
    printf(1, "Parent PID: %d\n", getpid());
    char buf[128];
    sleep(50);  // Allow child to send message
    if (recv(buf) == 0)
      printf(1, "Parent received: %s\n", buf);
    else
      printf(1, "Parent: No message received\n");

    wait();
  }

  exit();
}
