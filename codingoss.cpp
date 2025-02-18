#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    // Create a child process using fork
    pid_t pid = fork();

    // Check if fork failed
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }
    // Child process
    else if (pid == 0) {
        // Simulating child process exiting immediately (becoming a zombie)
        printf("Child process (PID: %d) exiting...\n", getpid());
        exit(0);
    }
    // Parent process
    else {
        // Print parent's PID and the child's PID
        printf("Parent process (PID: %d), Child PID: %d\n", getpid(), pid);
        printf("The child process will become a zombie for a while.\n");

        // Parent sleeps for 10 seconds, leaving the child process as a zombie
        sleep(10);
        
        // Parent reaps the zombie process
        wait(NULL);  // This cleans up the zombie process
        printf("Zombie process reaped. Parent process exiting.\n");
    }

    return 0;
}
