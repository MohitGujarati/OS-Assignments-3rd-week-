#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child process
        struct timeval start_time;
        gettimeofday(&start_time, NULL); // Record start time

        // Write the starting time to the pipe
        if (write(pipe_fd[1], &start_time, sizeof(start_time)) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
        close(pipe_fd[1]); // Close the write end of the pipe

        // Execute the command passed as argument
        execvp(argv[1], &argv[1]);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else { // Parent process
        wait(NULL); // Wait for child process to finish

        struct timeval start_time, end_time;
        
        // Read the starting time from the pipe
        if (read(pipe_fd[0], &start_time, sizeof(start_time)) == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        close(pipe_fd[0]); // Close the read end of the pipe

        gettimeofday(&end_time, NULL); // Capture end time

        // Calculate elapsed time
        double elapsed = (end_time.tv_sec - start_time.tv_sec) +
                         (end_time.tv_usec - start_time.tv_usec) / 1e6;


        printf("Piped method Elapsed time: %.5f seconds\n", elapsed);
    }

    return 0;
}
