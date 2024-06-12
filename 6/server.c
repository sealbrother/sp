#include "net.h"
#include <pthread.h>

#define MAX_CLIENTS 5 // 最大客戶端數量

int client_fds[MAX_CLIENTS]; // 用於存儲每個客戶端的連接符
int client_count = 0; // 目前連接的客戶端數量

struct ThreadArgs {
  int connfd;
  int output_len;
  char cmd[SMAX];
  char output[SMAX]; 
};

void *client_print(void *arg) {
  struct ThreadArgs *args = (struct ThreadArgs *)arg;
  int connfd = args->connfd;
  int output_len = args->output_len;
  char *cmd = args->cmd;
  char *output = args->output;
  write(connfd, cmd, strlen(cmd)+1);
  sleep(1); // 避免競爭
  write(connfd, output, output_len+1); // 將output傳給 client
  free(args);
  return NULL;
}

int serv(int client_fds[], int client_count) {
  char cmd[SMAX];
  printf("$ ");           // 印出提示訊息
  if (fgets(cmd, sizeof(cmd), stdin) == NULL) return 0;
  if (strncmp(cmd, "exit", 4) == 0) return 0; // 若是 exit 則離開！
  strtok(cmd, "\n");                     // 把 \n 去除
  fprintf(stderr, "cmd=%s\n", cmd);      // 印出命令方便觀察
  system(cmd);

  FILE *fp = popen(cmd, "r");
  if (fp == NULL){
    perror("Error : ");
    return 0;
  }

  size_t output_len = 0; // 初設 output 的長度為0
  char buf[SMAX], output[SMAX];
  while (fgets(buf, SMAX, fp) != NULL) { // 將 fp 一行內容讀取到 buf ，並在換行時(讀完整個輸出)停止讀取。
    size_t buf_len = strlen(buf); // buf 的長度
    if (output_len + buf_len < SMAX) {
      strcpy(output + output_len, buf); // 從 output 的第 output_len 個位置開始進行複製 buf
      output_len += buf_len; // 更新 output 長度
    } 
    else {
      break;
    }
  }
  pclose(fp);
  
  pthread_t threads[MAX_CLIENTS];
  for (int i = 0; i < client_count; i++) {
    int connfd = client_fds[i];
    if (connfd > 0) {
      struct ThreadArgs *args = malloc(sizeof(struct ThreadArgs));
      args->connfd = connfd;
      args->output_len = output_len;
      strcpy(args->cmd, cmd); 
      strcpy(args->output, output); 
      pthread_create(&threads[i], NULL, &client_print, args);
    }
  }
  return 1;
}

void *accept_client(void *arg) {
  int *port_ptr = (int *)arg;
  int port = *port_ptr;
  net_t net;
  net_init(&net, SOCK_STREAM, 1, port, NULL);
  net_bind(&net);
  net_listen(&net);
  while (1) {
    int connfd = net_accept(&net); // 等待連線進來
    if (connfd > 0) {
      // 將客戶端的連接符保存到數組中
      client_fds[client_count++] = connfd;
    }
  }
  return NULL;
}

int main(int argc, char *argv[]) {
  int port = (argc >= 2) ? atoi(argv[1]) : PORT;
  pthread_t thread_net;
  pthread_create(&thread_net, NULL, &accept_client, &port);  
  while (serv(client_fds, client_count)) {}
  return 0;
}
