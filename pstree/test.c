#include <stdio.h>
#include <stdlib.h>

int main() {
    int *oldArr, *newArr;
    int oldSize, newSize;
    int i;

    // 获取原数组长度
    printf("Enter the size of the original array: ");
    scanf("%d", &oldSize);

    // 分配原数组内存
    oldArr = (int *)malloc(oldSize * sizeof(int));

    // 初始化原数组
    for (i = 0; i < oldSize; i++) {
        oldArr[i] = i;
    }

    // 获取新数组长度
    printf("Enter the size of the new array: ");
    scanf("%d", &newSize);

    // 扩容原数组
    newArr = (int *)realloc(oldArr, newSize * sizeof(int));

    // 初始化新数组
    for (i = oldSize; i < newSize; i++) {
        newArr[i] = i;
    }

    // 输出新数组
    printf("oldArr: ");
    for (i = 0; i < newSize; i++) {
        printf("%d ", oldArr[i]);
    }
    printf("\n");

    // 释放内存
    free(newArr);

    return 0;
}