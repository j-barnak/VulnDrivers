#ifndef __UAF__
#define __UAF__

typedef struct uaf_object {
	char uaf_buffer_one[56];
	long args;
	void (*cb)(void);
	char uaf_buffer_two[20];
} uaf_object_t;


typedef struct k_object {
	char k_buffer[96];
} k_object_t;

void uaf_callback(void);
int uaf_alloc_object(unsigned long __user args);
int uaf_use_object(void);
void uaf_free_object(void);

uaf_object_t * uaf_object_context; 

#endif
