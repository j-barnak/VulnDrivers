#ifndef __UAF__
#define __UAF__

struct UafObject {
    char buffer_one[56];
    long arg;
    void (*function_ptr)(void);
    char buffer_two[20];
};

extern struct UafObject* global_uaf_object;

struct KernelObject {
    char buffer[96];
};

enum IoctlUaf {
    ALLOC_UAF_OBJ = 0,
    USE_UAF_OBJ = 1,
    ALLOC_K_OBJ = 2,
    FREE_UAF_OBJ = 3
};

int alloc_uaf_obj(long __user arg);
void use_uaf_obj(void);
void alloc_k_obj(struct KernelObject* object);
void free_uaf_obj(void);

void callback(void);

#endif
