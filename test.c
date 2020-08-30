#include "mem_manager.h"
#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0


static void Test(const char *_desc, int _status) {
    printf("%s: %s \n", _desc, (_status)? "Pass": "FAIL");
}

static int TestInit() {
    const size_t size = 1000;
    void *block =malloc(size);
     if(block == NULL){
        return FALSE;
    }
    Init(block, size);
    free(block);
    return TRUE;
}

static int TestMyMalloc() {
    int status = TRUE;
    const size_t size = 65*2;
    void *block = malloc(size);
    if(block == NULL){
        return FALSE;
    }
    Init(block, size);
    if((char *)block+66 != MyMalloc(64)){
        status = FALSE;
    }
    if(NULL != MyMalloc(64)){
        status = FALSE;
    }
    free(block);
    return status;
}

static int TestMyMalloc16() {
    int status = TRUE;
    const size_t size = 65*2;
    void *block = malloc(size);
    if(block == NULL){
        return FALSE;
    }
    Init(block, size);
    if((NULL == MyMalloc(16))) {
        status = FALSE;
    }
    if(NULL == MyMalloc(16)) {
        status = FALSE;
    }
    if(NULL != MyMalloc(64)) {
        status = FALSE;
    }
    free(block);
    return status;
}


static int TestMyFree() {
    int status = TRUE;
    const size_t size = 65*2;
    void *block, *block64;
    block = malloc(size);
    if(block == NULL){
        return FALSE;
    }
    Init(block, size);
    block64 = MyMalloc(64);
    MyFree(block64);
    if((char *)block+66 != MyMalloc(64)){
        status = FALSE;
    }
    return status;
}

int main() {
    Test("TestInit", TestInit());
    Test("TestMyMalloc", TestMyMalloc());
    Test("TestMyFree", TestMyFree());
    Test("TestMyMalloc16", TestMyMalloc16());
    return 0;
}