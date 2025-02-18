#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

// Define the size of the shared memory buffer
#define SHM_SIZE 1024

// Function to generate Collatz sequence
void collatz_sequence(int num, int *shm_ptr) {
    int i = 0;  // Index to store the sequence in shared memory
    while (num != 1) {
        shm_ptr[i++] = num;  // Store the current number in shared memory
        if (num % 2 == 0) {
            num /= 2;
        } else {
            num = 3 * num + 1;
        }
    }
    shm_ptr[i] = 1;  // Store the final value (1)
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <positive integer>\n", argv[0]);
        return 1;
    }

    int num = atoi(argv[1]);
    if (num <= 0) {
        fprintf(stderr, "Error: Please provide a positive integer.\n");
        return 1;
    }

    // Create shared memory object
    int shm_fd = shm_open("/collatz_shm", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (shm_fd == -1) {
        perror("shm_open");
        return 1;
    }

    // Configure the size of the shared memory object
    if (ftruncate(shm_fd, SHM_SIZE) == -1) {
        perror("ftruncate");
        return 1;
    }

    // Map the shared memory object into the address space
    int *shm_ptr = (int*)mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {  // Child process
        collatz_sequence(num, shm_ptr);
        exit(0);  // Child process exits after generating the sequence
    } else {  // Parent process
        wait(NULL);  // Wait for child process to finish
        printf("Collatz sequence for %d: ", num);

        // Output the sequence from shared memory
        for (int i = 0; shm_ptr[i] != 0; i++) {
            printf("%d ", shm_ptr[i]);
        }
        printf("\n");

        // Clean up shared memory
        if (shm_unlink("/collatz_shm") == -1) {
            perror("shm_unlink");
        }
    }

    return 0;
}
