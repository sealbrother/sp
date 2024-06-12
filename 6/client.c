#include "net.h"
#include <pthread.h>

#define SMAX 1024

void *receive_messages(void *sock) {
    int connfd = *((int *)sock);
    char buffer[SMAX];
    int bytes_read;

    while ((bytes_read = read(connfd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0';
        printf("%s", buffer);
        fflush(stdout);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    int port = (argc >= 2) ? atoi(argv[1]) : PORT;
    net_t net;
    net_init(&net, SOCK_STREAM, 0, port, "127.0.0.1");

    net_connect(&net);

    printf("Connected to server 127.0.0.1\n");

    pthread_t tid;
    pthread_create(&tid, NULL, &receive_messages, &net.sockfd);

    char buffer[SMAX];
    while (1) {
        fgets(buffer, sizeof(buffer), stdin);
        send(net.sockfd, buffer, strlen(buffer), 0);
    }

    close(net.sockfd);
    return 0;
}