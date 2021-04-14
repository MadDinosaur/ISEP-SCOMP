#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

#define BUFFER_SIZE 100
#define DIV 5
#define N_PRODUCTS 4

typedef struct {
    char name[BUFFER_SIZE];
    float price;
    int barcode;
} Product;

typedef struct {
    int readerID;
    int barcode;
} BarcodeReaderRequest;

Product product;
Product products[N_PRODUCTS];
BarcodeReaderRequest request;

int pipe_status;

Product* initialize_database() {
    Product apple;
    strcpy(apple.name, "apple");
    apple.price = 0.25f;
    apple.barcode = 1111;

    Product banana;
    strcpy(banana.name, "banana");
    banana.price = 0.5f;
    banana.barcode = 2222;

    Product chocolate;
    strcpy(chocolate.name, "chocolate");
    chocolate.price = 1.5f;
    chocolate.barcode = 3333;

    Product butter;
    strcpy(butter.name, "butter");
    butter.price = 1.0f;
    butter.barcode = 4444;

    products[0] = apple;
    products[1] = banana;
    products[2] = chocolate;
    products[3] = butter;
}

int generate_random_barcode(int id) {
    int barcodes[] = {1111,2222,3333,4444};

    srand(time(0)*id);
    int rand_index = rand() % (sizeof(barcodes)/sizeof(int));
    return barcodes[rand_index];
}

Product search_product(int search_barcode) {
    int i;
    for (i = 0; i < N_PRODUCTS; i++)
        if (products[i].barcode == search_barcode)
            return products[i];
}

int generate_barcode_reader(int* fd_main, int* fd_child, int id) {
    int p = fork();

    if (p < 0) exit(-1); //ERRO
    if (p == 0) {
        //closes read descriptor on main pipe (shared by all children)
        close(fd_main[0]);
        //closes write descriptor on child-specific pipe
        close(fd_child[1]);

        request.readerID = id;

        //generates two random barcodes at a random interval (1-5 seconds) - simulates customer use of a barcode reader
        int num_readings = 2;
        int s;
        for (s = 0; s < num_readings; s++) {
            sleep(rand() % 4 + 1);

            request.barcode = generate_random_barcode(id);
            //writes to pipe
            write(fd_main[1], &request, BUFFER_SIZE);
            printf("Reader no. %d requested information on barcode no. %d.\n", id, request.barcode);
            //waits for reply and prints it
            do {
                pipe_status = read(fd_child[0], &product, BUFFER_SIZE + 4);
            } while(pipe_status <= 0);
            printf("Reader no. %d displayed product: %s %.2f€\n", id, product.name, product.price);
        }

        //closes read descriptor on child-specific pipe
        close(fd_child[0]);
        exit(0);
    }
}


int main (void) {
    int fd_main[2], fd_child[DIV][2];

    initialize_database();
    
    //parent creates main communication pipe
    if(pipe(fd_main) == -1) {
        perror("Pipe failed");
        return 1;
    }

    int i;
    for (i = 0; i < DIV; i++) {
        //parent creates a pipe for each child
        if(pipe(fd_child[i]) == -1) {
            perror("Pipe failed");
            return 1;
        }
        //parent creates a child with a sequencial id
        generate_barcode_reader(fd_main, fd_child[i], i);

        //closes read descriptor on child pipe - unused by parent
        close(fd_child[i][0]);
    }

    //closes write descriptor on main pipe - unused by parent
    close(fd_main[1]);

    //reads product information requests and replies to each child
    while (read(fd_main[0], &request, BUFFER_SIZE) != 0) {
        //DEBUGGING
        //printf("Database received request from reader %d on barcode %d.\n", request.readerID, request.barcode);
        
        Product requested = search_product(request.barcode);
        //DEBUGGING
        //printf("Barcode: %d, Name: %s, Price: %.2f\n", request.barcode, requested.name, requested.price);
        write(fd_child[request.readerID][1], &requested, BUFFER_SIZE + 4);
    }
    
    //closes read descriptor on main pipe
    close(fd_main[0]);
    //closes write descriptor on each of the childrens' pipes
    int c;
    for (c = 0; c < DIV; c++) {
        close(fd_child[c][1]);
    }
    return 0;
}