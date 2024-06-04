#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

// 矩阵结构体定义
struct Matrix {
    int x, y;       // 矩阵的行数和列数
    double *array;  // 指向矩阵元素的指针
};

// 创建矩阵函数
struct Matrix Create_Matrix(int x, int y) {
    struct Matrix res;
    res.x = x;
    res.y = y;
    res.array = (double*)malloc(x * y * sizeof(double));  // 动态分配内存
    memset(res.array, 0, x * y * sizeof(double));  // 将内存初始化为0
    return res;
}

// 矩阵转置函数
void Transpose(struct Matrix* a) {
    double *temp = (double*)malloc((a->x) * (a->y) * sizeof(double));  // 创建临时数组存储转置后的元素
    for (int i = 0; i < a->x; i++) {
        for (int j = 0; j < a->y; j++) {
            temp[j * a->x + i] = a->array[i * a->y + j];  // 转置操作
        }
    }
    free(a->array);  // 释放原数组内存
    a->array = temp;  // 将转置结果复制回原数组
    // 交换行数和列数
    int temp_dim = a->x;
    a->x = a->y;
    a->y = temp_dim;
}

// 矩阵加法函数
struct Matrix Add(struct Matrix a, struct Matrix b) {
    struct Matrix res = Create_Matrix(a.x, a.y);
    for (int i = 0; i < (a.x) * (a.y); i++)
        res.array[i] = a.array[i] + b.array[i];
    return res;
}

// 矩阵乘法函数
struct Matrix Multiply(struct Matrix a, struct Matrix b) {
    struct Matrix res = Create_Matrix(a.x, b.y);
    for (int i = 0; i < a.x; i++) {
        for (int j = 0; j < b.y; j++) {
            for (int k = 0; k < a.y; k++) {
                res.array[i * res.y + j] += a.array[i * a.y + k] * b.array[k * b.y + j];
            }
        }
    }
    return res;
}

// 矩阵打印函数
void Print(struct Matrix a) {
    for (int i = 0; i < a.x; i++) {
        for (int j = 0; j < a.y; j++)
            printf("%lf ", a.array[i * a.y + j]);
        printf("\n");
    }
    printf("\n");
}

// 矩阵改变形状函数（选做功能）
void Reshape(struct Matrix* a, int new_x, int new_y) {
    if (a->x * a->y != new_x * new_y) {
        printf("无法改变形状：新形状与原形状元素数量不一致。\n");
        return;
    }
    a->x = new_x;
    a->y = new_y;
}

int main() {
    struct Matrix x = Create_Matrix(3, 3);
    // 初始化赋值
    for (int i = 0; i < x.x; i++) {
        for (int j = 0; j < x.y; j++)
            x.array[i * x.y + j] = j + 1;
    }

    printf("原始矩阵:\n");
    Print(x);

    Transpose(&x);
    printf("转置矩阵:\n");
    Print(x);

    struct Matrix y = Create_Matrix(3, 3);
    // 初始化赋值
    for (int i = 0; i < y.x; i++) {
        for (int j = 0; j < y.y; j++)
            y.array[i * y.y + j] = (j + 1) * 2;
    }

    printf("第二个矩阵:\n");
    Print(y);

    struct Matrix sum = Add(x, y);
    printf("矩阵相加:\n");
    Print(sum);

    struct Matrix prod = Multiply(x, y);
    printf("矩阵相乘:\n");
    Print(prod);

    Reshape(&x, 1, 9);
    printf("改变形状后的矩阵:\n");
    Print(x);

    free(x.array);
    free(y.array);
    free(sum.array);
    free(prod.array);

    return 0;
}
