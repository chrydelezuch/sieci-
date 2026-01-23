#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Użycie: %s <IP> <PORT>\n", argv[0]);
        return 1;
    }

    const char *server_ip = argv[1];
    int port = atoi(argv[2]);

    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];
    ssize_t bytes_read;

    // Utworzenie gniazda
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sock);
        return 1;
    }

    // Połączenie z serwerem
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sock);
        return 1;
    }

    printf("Połączono z %s:%d\n", server_ip, port);
    printf("Wpisuj dane do wysłania (Ctrl+D kończy):\n");

    // Wysyłanie dowolnych danych (stdin)
    while ((bytes_read = read(STDIN_FILENO, buffer, BUF_SIZE)) > 0) {
        if (send(sock, buffer, bytes_read, 0) < 0) {
            perror("send");
            break;
        }
    }

    close(sock);
    return 0;
}
