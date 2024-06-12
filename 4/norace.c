#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define LOOPS 100000
int account_balance = 1000;

void *deposit(void *arg) {
    int amount = *((int *)arg);
    for (int i = 0; i < LOOPS; i++) {
        account_balance += amount;
    }
    return NULL;
}

void *withdraw(void *arg) {
    int amount = *((int *)arg);
    for (int i = 0; i < LOOPS; i++) {
        account_balance -= amount;
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    int deposit_amount = 1;
    int withdraw_amount = 1;

    pthread_create(&thread1, NULL, deposit, &deposit_amount);
    pthread_create(&thread2, NULL, withdraw, &withdraw_amount);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Final account balance: %d\n", account_balance);
    return 0;
}