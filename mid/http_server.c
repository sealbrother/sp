#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>//定義了一些用於 IP 地址轉換的函數，例如 inet_addr、inet_ntoa 等。它還定義了一些用於將數字地址和字符串地址之間進行轉換的函數，這在網絡編程中非常有用。
#include <sys/socket.h>//定義了許多用於網絡通信的函數和數據結構，包括創建套接字、綁定地址、設置選項等函數。這個標頭文件是進行網絡編程時不可或缺的一部分。
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define THREAD_POOL_SIZE 10

void *handle_client(void *client_socket);

void send_response(int client_socket, const char *header, const char *content_type, const char *body);
void send_404(int client_socket);

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 10) < 0) {
        perror("Listen failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", PORT);

    pthread_t thread_pool[THREAD_POOL_SIZE];

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        int *pclient = malloc(sizeof(int));
        *pclient = client_socket;

        for (int i = 0; i < THREAD_POOL_SIZE; i++) {
            if (pthread_create(&thread_pool[i], NULL, handle_client, pclient) != 0) {
                perror("Thread creation failed");
                close(client_socket);
                free(pclient);
            }
        }
    }

    close(server_socket);
    return 0;
}

void *handle_client(void *client_socket) {
    int sock = *((int *)client_socket);
    free(client_socket);
    char buffer[BUFFER_SIZE];
    int read_size = recv(sock, buffer, sizeof(buffer) - 1, 0);

    if (read_size > 0) {
        buffer[read_size] = '\0';
        printf("Received request:\n%s\n", buffer);

        char method[16], path[256], version[16];
        sscanf(buffer, "%s %s %s", method, path, version);

        if (strcmp(method, "GET") == 0) {
            if (strcmp(path, "/") == 0) {
                path = "/index.html";
            }

            char full_path[512];
            sprintf(full_path, "public%s", path);
            int file = open(full_path, O_RDONLY);

            if (file < 0) {
                send_404(sock);
            } else {
                struct stat file_stat;
                fstat(file, &file_stat);
                char *file_content = malloc(file_stat.st_size);
                read(file, file_content, file_stat.st_size);
                close(file);

                send_response(sock, "HTTP/1.1 200 OK", "text/html", file_content);
                free(file_content);
            }
        }
    }

    close(sock);
    return NULL;
}

void send_response(int client_socket, const char *header, const char *content_type, const char *body) {
    char response[BUFFER_SIZE];
    sprintf(response, "%s\nContent-Type: %s\nContent-Length: %ld\n\n%s", header, content_type, strlen(body), body);
    send(client_socket, response, strlen(response), 0);
}

void send_404(int client_socket) {
    const char *body = "<html><body><h1>404 Not Found</h1></body></html>";
    send_response(client_socket, "HTTP/1.1 404 Not Found", "text/html", body);
}
