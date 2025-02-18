#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_LINE 80  // Maximum length of command

void execute_command(char *input);

int main() {
    char input[MAX_LINE];  // Store user input
    int should_run = 1;

    while (should_run) {
        // Print shell prompt
        printf("HyperShell > ");
        fflush(stdout);

        // Get user input
        if (fgets(input, MAX_LINE, stdin) == NULL) {
            continue; // Handle EOF or error
        }

        // Remove newline character at the end of the input
        input[strcspn(input, "\n")] = 0;

        // Handle exit command
        if (strcmp(input, "exit") == 0) {
            should_run = 0;
            continue;
        }

        // Call the function to execute the command
        execute_command(input);
    }

    return 0;
}

// Function to execute a command
void execute_command(char *input) {
    char *args[MAX_LINE / 2 + 1];  // Array to store the arguments
    int i = 0;
    char *token;

    // Tokenize the input and fill the args[] array
    token = strtok(input, " ");
    while (token != NULL) {
        args[i] = token;
        i++;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;  // NULL terminate the arguments array

    // Fork a child process to execute the command
    pid_t pid = fork();

    if (pid < 0) {  // Error forking
        perror("Fork failed");
        exit(1);
    }

    if (pid == 0) {  // Child process
        // Check if the user input is a built-in command (like echo or ls)
        if (execvp(args[0], args) == -1) {
            // If execvp fails, print an error
            perror("Exec failed");
            exit(1);
        }
    } else {  // Parent process
        // Wait for the child process to finish unless it's a background process
        if (strcmp(args[i - 1], "&") != 0) {
            wait(NULL);  // Parent waits for child process to finish
        }
    }
}
