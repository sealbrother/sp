#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    if (fork() == 0) { // 第一個子進程
        if (fork() == 0) { // 第二個子進程
            while (1) {
                printf("%-5d:Mary\n", getpid());
                sleep(2);
            }
        } else { // 第一個子進程繼續
            while (1) {
                printf("%-5d:George\n", getpid());
                sleep(1);
            }
        }
    } else { // 父進程
        while (1) {
            printf("%-5d:--------------------------\n", getpid());
            sleep(1);
        }
    }
    return 0;
}