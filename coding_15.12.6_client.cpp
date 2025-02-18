#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 6017
#define BUF_SIZE 512

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];
    
    // Create the socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(1);
    }
    
    // Set up the server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    
    // Convert the IP address from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        exit(1);
    }

    // Connect to the server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    // Receive the quote from the server
    ssize_t n = recv(sockfd, buffer, BUF_SIZE - 1, 0);
    if (n < 0) {
        perror("Receive failed");
        exit(1);
    }
    buffer[n] = '\0';  // Null-terminate the string

    // Print the received quote
    printf("Quote of the Day: %s\n", buffer);

    // Close the socket
    close(sockfd);
    return 0;
}
