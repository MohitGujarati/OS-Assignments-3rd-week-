#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void generate_collatz_sequence(int num) {
    // Generate the Collatz sequence for the given number
    while (num != 1) {
        printf("%d ", num);
        if (num % 2 == 0) {
            num /= 2;  // If even, divide by 2
        } else {
            num = 3 * num + 1;  // If odd, multiply by 3 and add 1
        }
    }
    printf("1\n");  // Print the last 1 in the sequence
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <positive integer>\n", argv[0]);
        return 1;
    }

    // Convert the argument to an integer
    int num = atoi(argv[1]);

    // Ensure the number is positive
    if (num <= 0) {
        fprintf(stderr, "Error: Please provide a positive integer.\n");
        return 1;
    }

    // Fork a new process
    pid_t pid = fork();

    if (pid < 0) {
        // Error in fork
        perror("Fork failed");
        return 1;
    }

    if (pid == 0) {
        // Child process
        generate_collatz_sequence(num);
        exit(0);  // Exit the child process after completing the sequence
    } else {
        // Parent process
        wait(NULL);  // Wait for the child process to finish
        printf("Child process completed.\n");
    }

    return 0;
}
