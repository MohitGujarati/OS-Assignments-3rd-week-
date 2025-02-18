#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 6017
#define BUF_SIZE 512

// Define the quote of the day
const char *QUOTE = "The only way to do great work is to love what you do. - Steve Jobs";

int main() {
    int sockfd, clientfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    char buffer[BUF_SIZE];

    // Create the socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Set up the server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket to the specified port
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(1);
    }

    // Listen for incoming connections
    if (listen(sockfd, 5) < 0) {
        perror("Listen failed");
        close(sockfd);
        exit(1);
    }

    printf("Quote of the Day server running on port %d...\n", PORT);

    // Accept incoming client connections
    client_len = sizeof(client_addr);
    while (1) {
        clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);
        if (clientfd < 0) {
            perror("Accept failed");
            continue;
        }

        printf("Connection established with client.\n");

        // Send the quote to the client
        ssize_t n = send(clientfd, QUOTE, strlen(QUOTE), 0);
        if (n < 0) {
            perror("Send failed");
        }

        // Close the client connection
        close(clientfd);
    }

    // Close the server socket
    close(sockfd);
    return 0;
}
