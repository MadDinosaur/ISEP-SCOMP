#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int productCode;
    char description[100];
    float price;
} Product;

int main (void) {
    int fd, data_size = sizeof(Product);
    char str[100];
    Product* p;
    
    //Creates shared memory area
    if ((fd = shm_open("/ex02", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR)) < 0)
        perror("Failed to create shared memory");
    //Defines memory size
    if (ftruncate (fd, data_size) < 0)
        perror("Failed to adjust memory size");
    //Maps the area in an address
    p = (Product *) mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (p == NULL) {
        perror("Failed to map the area");
        exit(0);
    }

    //Reads data from keyboard and writes them to memory
    scanf("%d", &(p->productCode));
    scanf("%s", &str);
    strcpy(p->description, str);
    scanf("%f", &(p->price));
}