#include<stdio.h>
typedef struct node{
    void* addr;
}node;
void test(node* n);
int zx(){
    node n;
    test(&n);
    printf("%p\n    ", n.addr);
    return 0;
}
void test(node* n){
    n->addr = (void*)0x12345698;
    return;
}