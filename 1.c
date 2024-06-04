#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

// 矩陣結構體定義
struct Matrix {
 int x, y; // 矩陣的行數與列數
 double *array; // 指向矩陣元素的指針
};

// 建立矩陣函數
struct Matrix Create_Matrix(int x, int y) {
 struct Matrix res;
 res.x = x;
 res.y = y;
 res.array = (double*)malloc(x * y * sizeof(double)); // 動態分配內存
 memset(res.array, 0, x * y * sizeof(double)); // 將記憶體初始化為0
 return res;
}

// 矩陣轉置函數
void Transpose(struct Matrix* a) {
 double *temp = (double*)malloc((a->x) * (a->y) * sizeof(double)); // 建立暫存陣列儲存轉置後的元素
 for (int i = 0; i < a->x; i++) {
 for (int j = 0; j < a->y; j++) {
 temp[j * a->x + i] = a->array[i * a->y + j]; // 轉置操作
 }
 }
 free(a->array); // 釋放原數組內存
 a->array = temp; // 將轉置結果複製回原始數組
 // 交換行數和列數
 int temp_dim = a->x;
 a->x = a->y;
 a->y = temp_dim;
}

// 矩陣加法函數
struct Matrix Add(struct Matrix a, struct Matrix b) {
 struct Matrix res = Create_Matrix(a.x, a.y);
 for (int i = 0; i < (a.x) * (a.y); i++)
 res.array[i] = a.array[i] + b.array[i];
 return res;
}

// 矩陣乘法函數
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

// 矩陣列印函數
void Print(struct Matrix a) {
 for (int i = 0; i < a.x; i++) {
 for (int j = 0; j < a.y; j++)
 printf("%lf ", a.array[i * a.y + j]);
 printf("\n");
 }
 printf("\n");
}

// 矩陣改變形狀函數（選做功能）
void Reshape(struct Matrix* a, int new_x, int new_y) {
 if (a->x * a->y != new_x * new_y) {
 printf("無法改變形狀：新形狀與原形狀元素數量不一致。\n");
 return;
 }
 a->x = new_x;
 a->y = new_y;
}

int main() {
 struct Matrix x = Create_Matrix(3, 3);
 // 初始化賦值
 for (int i = 0; i < x.x; i++) {
 for (int j = 0; j < x.y; j++)
 x.array[i * x.y + j] = j + 1;
 }

 printf("原始矩陣:\n");
 Print(x);

 Transpose(&x);
 printf("轉置矩陣:\n");
 Print(x);

 struct Matrix y = Create_Matrix(3, 3);
 // 初始化賦值
 for (int i = 0; i < y.x; i++) {
 for (int j = 0; j < y.y; j++)
 y.array[i * y.y + j] = (j + 1) * 2;
 }

 printf("第二個矩陣:\n");
 Print(y);

 struct Matrix sum = Add(x, y);
 printf("矩陣相加:\n");
 Print(sum);

 struct Matrix prod = Multiply(x, y);
 printf("矩陣相乘:\n");
 Print(prod);

 Reshape(&x, 1, 9);
 printf("改變形狀後的矩陣:\n");
 Print(x);

 // 釋放分配的記憶體
 free(x.array);
 free(y.array);
 free(sum.array);
 free(prod.array);

 return 0;
}