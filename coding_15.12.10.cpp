#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstdlib>
#include <cstring>

#define BUFFER_SIZE 1024

void copy_file(const char* source, const char* destination) {
    int pipefd[2];
    pid_t pid;
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead, bytesWritten;

    // Create a pipe
    if (pipe(pipefd) == -1) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    // Fork a new process
    pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {  // Child process
        // Close write-end of the pipe (since the child will read from it)
        close(pipefd[1]);

        // Open the destination file for writing
        int destFd = open(destination, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (destFd == -1) {
            perror("Destination file opening failed");
            exit(EXIT_FAILURE);
        }

        // Read from pipe and write to destination file
        while ((bytesRead = read(pipefd[0], buffer, BUFFER_SIZE)) > 0) {
            bytesWritten = write(destFd, buffer, bytesRead);
            if (bytesWritten != bytesRead) {
                perror("Write failed");
                exit(EXIT_FAILURE);
            }
        }

        // Close the destination file and pipe
        close(destFd);
        close(pipefd[0]);
        exit(EXIT_SUCCESS);

    } else {  // Parent process
        // Close read-end of the pipe (since the parent will write to it)
        close(pipefd[0]);

        // Open the source file for reading
        std::ifstream srcFile(source, std::ios::binary);
        if (!srcFile.is_open()) {
            perror("Source file opening failed");
            exit(EXIT_FAILURE);
        }

        // Read from the source file and write to pipe
        while (srcFile.read(buffer, BUFFER_SIZE) || srcFile.gcount() > 0) {
            ssize_t bytesWritten = write(pipefd[1], buffer, srcFile.gcount());
            if (bytesWritten != srcFile.gcount()) {
                perror("Pipe write failed");
                exit(EXIT_FAILURE);
            }
        }

        // Close the source file and pipe
        srcFile.close();
        close(pipefd[1]);

        // Wait for child process to finish
        wait(NULL);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: ./filecopy <source_file> <destination_file>\n";
        return 1;
    }

    const char* sourceFile = argv[1];
    const char* destinationFile = argv[2];

    copy_file(sourceFile, destinationFile);

    std::cout << "File copied successfully from " << sourceFile << " to " << destinationFile << ".\n";
    return 0;
}
