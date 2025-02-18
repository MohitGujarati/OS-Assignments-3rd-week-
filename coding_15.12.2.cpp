#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command>\n", argv[0]);
        exit(EXIT_FAILURE);
    

    int shm_fd = shm_open("/shm_time", O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(struct timeval));
    
    // Cast mmap result to struct timeval*
    struct timeval *shared_time = (struct timeval*) mmap(NULL, sizeof(struct timeval), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child Process
        gettimeofday(shared_time, NULL); // Store start time in shared memory
        munmap(shared_time, sizeof(struct timeval));
        close(shm_fd);
        
        execvp(argv[1], &argv[1]);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else { // Parent Process
        wait(NULL); // Wait for child to complete

        struct timeval end_time;
        gettimeofday(&end_time, NULL); // Capture end time

        double elapsed = (end_time.tv_sec - shared_time->tv_sec) +
                         (end_time.tv_usec - shared_time->tv_usec) / 1e6;
        printf("Timed:-");   
        printf("Elapsed time: %.5f seconds\n", elapsed);

        shm_unlink("/shm_time"); // Cleanup shared memory
    }

    return 0;
}
