#define DEFAULT_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/time.h>

void* my_malloc(int size)
{
    // TODO verify space in Mapped Region

    // verify space in Unmapped Region
    return sbrk(size);
}

void my_free(int size);

int main()
{
    void* brkAdress = sbrk(0);
    printf("break is currently at %p\n", sbrk(0));

    //Essayer de stocker un int sur le HEAP en deplacant le break
    int* value = brkAdress;
    
    brkAdress = my_malloc(sizeof(int));
    printf("break is currently at %p (+ 4 bytes)\n", sbrk(0));

    *value = 42;
    printf("value = %d\n", *value);

    brkAdress = sbrk(-sizeof(int));
    printf("break is currently at %p (- 4 bytes)\n", sbrk(0));

    return 0;
}

//gcc test_brk.c && ./a.out