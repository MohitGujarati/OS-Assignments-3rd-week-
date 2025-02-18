#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN_PID 300
#define MAX_PID 5000
#define PID_RANGE (MAX_PID - MIN_PID + 1)

// Bitmap size based on the number of PIDs
#define BITMAP_SIZE ((PID_RANGE + 31) / 32) // Round up to the nearest 32-bit word

// Bitmap to track PID availability (0 = available, 1 = used)
static unsigned int pid_bitmap[BITMAP_SIZE];

// Function to initialize the PID manager
int allocate_map(void) {
    // Initialize the bitmap to 0 (all PIDs available)
    memset(pid_bitmap, 0, sizeof(pid_bitmap));
    return 1;  // Success
}

// Function to allocate and return a PID
int allocate_pid(void) {
    for (int i = 0; i < BITMAP_SIZE; ++i) {
        // Check each 32-bit word in the bitmap
        if (pid_bitmap[i] != 0xFFFFFFFF) {  // If there are available PIDs
            for (int j = 0; j < 32; ++j) {
                int bit_position = i * 32 + j;
                if (bit_position >= PID_RANGE) {
                    break;
                }
                if ((pid_bitmap[i] & (1 << j)) == 0) {  // Check if this PID is available
                    // Mark PID as used
                    pid_bitmap[i] |= (1 << j);
                    return MIN_PID + bit_position;  // Return the allocated PID
                }
            }
        }
    }
    return -1;  // No available PID
}

// Function to release a PID
void release_pid(int pid) {
    if (pid < MIN_PID || pid > MAX_PID) {
        printf("Invalid PID %d\n", pid);
        return;
    }

    int bit_position = pid - MIN_PID;
    int word_index = bit_position / 32;
    int bit_index = bit_position % 32;

    // Mark the PID as available
    pid_bitmap[word_index] &= ~(1 << bit_index);
}

int main(void) {
    // Initialize the PID manager
    if (allocate_map() == -1) {
        printf("Error initializing PID manager\n");
        return -1;
    }

    // Test the allocation and release of PIDs
    int pid1 = allocate_pid();
    if (pid1 != -1) {
        printf("Allocated PID: %d\n", pid1);
    } else {
        printf("Failed to allocate PID\n");
    }

    int pid2 = allocate_pid();
    if (pid2 != -1) {
        printf("Allocated PID: %d\n", pid2);
    } else {
        printf("Failed to allocate PID\n");
    }

    // Release a PID
    release_pid(pid1);
    printf("Released PID: %d\n", pid1);

    // Allocate another PID after release
    int pid3 = allocate_pid();
    if (pid3 != -1) {
        printf("Allocated PID after release: %d\n", pid3);
    } else {
        printf("Failed to allocate PID after release\n");
    }

    return 0;
}
