#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct {
    int productCode;
    char* description;
    float price;
} Product;

int main(void) {
    int fd, data_size = sizeof(Product);
    Product *p;
    
    //Accesses shared memory area
    if ((fd = shm_open("/ex02", O_RDWR, S_IRUSR|S_IWUSR)) < 0)
        perror("Failed to create shared memory");
    //Defines memory size
    if (ftruncate (fd, data_size) < 0)
        perror("Failed to adjust memory size");
    //Maps the area in an address
    p = (Product*) mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);
    if (p == NULL) {
        perror("Failed to map the area");
        exit(0);
    }

    //Reads data from memory and prints
    printf("Product code: %d\n", p->productCode);
    printf("Description: %s\n", p->description);
    printf("Price: %.2f\n", p->price);

    if(munmap(a, data_size)<0){
        perror("No munmap()");
        exit(0);
    }

    if(close(fd)<0){
        perror("No close()");
        exit(0);
    }

    if(shm_unlink("/ex04")<0){
        perror("No unlink()");
        exit(1);
    }
}