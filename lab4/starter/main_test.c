/**
  * ECE254 Linux Dynamic Memory Management Lab
  * @file: main_test.c
  * @brief: The main file to write tests of memory allocation algorithms
  */ 

/* includes */
/* system provided header files. You may add more */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* non-system provided header files. 
   Do not include more user-defined header files here
 */
#include "mem.h"


/* general test functions */


/* writes data to a memory block*/
int test_helper_write_data(void* block, size_t size)
{
    int i = 0;
    //printf("ptr: %p\n", block);
    for (i = 0; i < size; i++)
    {
       *(char*)(block + i) = 40;
    }
}


/* makes sure data is still in a memory block */
int test_helper_validate_data(void* block, size_t size)
{
    int valid = 1;
    int i = 0;
    //printf("ptr: %p\n", block);
    for (i = 0; i < size; i++)
    {
        //printf("%hhx,", *(int*)(block + i));
        if (*(char*)(block + i) != 40)
        {
            valid = 0;
        }
    }
    return valid;
}

/* test functions for worst alloc */
int test_worst_mem_too_big()
{
    void* p;
    worst_fit_memory_init(1024);
    // request something of size 1024
    p = worst_fit_alloc(1024);

    // cleanup memory
    worst_fit_memory_nuke();    

    // expect NULL ptr for impossible request 
    return (p == NULL);
}

int test_worst_dealloc()
{
    int res = 0;
    void*p;
    worst_fit_memory_init(1024);
    p = worst_fit_alloc(100);
    test_helper_write_data(p, 100);
    res = test_helper_validate_data(p, 100);
    worst_fit_dealloc(NULL);
    res = test_helper_validate_data(p, 100);
    worst_fit_memory_nuke();
    return res;
}

/* allocates, deallocates, allocates, check memory integrity */
int test_worst_mem_large_cycle()
{
    void* p[10];
    int i = 0;
    int valid = 1;
    
    worst_fit_memory_init(2048);

    // request a bunch of larger blocks
    for(i = 0; i < 10; i++)
    {
        p[i] = worst_fit_alloc(100);
        // use the memory
        test_helper_write_data(p[i], 100);
    }    

    //printf("allocated 10 blocks of 100 bytes\n");
    //pool_dump();

    // make sure data is intact

    for (i = 0; i < 10; i++)
    {
        if (test_helper_validate_data(p[i], 100) != 1)
        {
            valid = 0;
        } 
    }

    //printf("data integrity check: %d\n", valid);

    // deallocate some data
    worst_fit_dealloc(p[0]); 
    worst_fit_dealloc(p[9]);
    worst_fit_dealloc(p[4]);
    worst_fit_dealloc(p[5]);
    worst_fit_dealloc(p[3]);

    //printf("deallocated 5 blocks\n");
    //pool_dump();


    //printf("check integrity after dealloc\n");
    // check integrity
    for (i = 1; i < 9; i++)
    {
        if (i == 0, i == 3 || i == 4 || i == 5 || i == 9)
            break;
        //printf("****check block %d\n", i);
        if (test_helper_validate_data(p[i], 100) != 1)
        {
            valid = 0;
        } 
        //printf("\n****\n");
    }


    //printf("0000xdata integrity check: %d\n", valid);


    // allocate data
    p[0] = worst_fit_alloc(200);
    //printf("0:%p\n", p[0]);
    test_helper_write_data(p[0], 200);
    //pool_dump();
    p[9] = worst_fit_alloc(100);
    //printf("9:%p\n", p[9]);
    test_helper_write_data(p[9], 100);
    //pool_dump();
    p[4] = worst_fit_alloc(300);
    //printf("4:%p\n", p[4]);
    test_helper_write_data(p[4], 300);
    p[5] = worst_fit_alloc(50);
    //printf("5:%p\n", p[5]);
    test_helper_write_data(p[5], 50);
    p[3] = worst_fit_alloc(4);
    //printf("3:%p\n", p[3]);
    test_helper_write_data(p[3], 4);

    //printf("re-allocate 5 blocks\n");
    //pool_dump();

    // check integrity
    //printf("****\ncheck block: %d\n", 0);
    valid = (test_helper_validate_data(p[0], 200) == 1) ? valid : 0;
    //printf("\nvalid %d\n", test_helper_validate_data(p[0], 200)); 

    //printf("****\ncheck block: %d\n", 1);
    valid = (test_helper_validate_data(p[1], 100) == 1) ? valid : 0;
    //printf("\nvalid %d\n", test_helper_validate_data(p[1], 100));

    //printf("****\ncheck block: %d\n", 2);
    valid = (test_helper_validate_data(p[2], 100) == 1) ? valid : 0;
    //printf("\nvalid %d\n", test_helper_validate_data(p[2], 100));

    //printf("****\ncheck block: %d\n", 3);
    valid = (test_helper_validate_data(p[3], 4) == 1) ? valid : 0;
    //printf("\nvalid %d\n", test_helper_validate_data(p[3], 4));

    //printf("****\ncheck block: %d\n", 4);
    valid = (test_helper_validate_data(p[4], 300) == 1) ? valid : 0;
    //printf("\nvalid %d\n", test_helper_validate_data(p[4], 300));

    //printf("****\ncheck block: %d\n", 5);
    valid = (test_helper_validate_data(p[5], 50) == 1) ? valid : 0;
    //printf("\nvalid %d\n", test_helper_validate_data(p[5], 50));

    //printf("****\ncheck block: %d\n", 6);
    valid = (test_helper_validate_data(p[6], 100) == 1) ? valid : 0;
    //printf("\nvalid %d\n", test_helper_validate_data(p[6], 100));

    //printf("****\ncheck block: %d\n", 7);
    valid = (test_helper_validate_data(p[7], 100) == 1) ? valid : 0;
    //printf("\nvalid %d\n", test_helper_validate_data(p[7], 100));

    //printf("****\ncheck block: %d\n", 8);
    valid = (test_helper_validate_data(p[8], 100) == 1) ? valid : 0;
    //printf("\nvalid %d\n", test_helper_validate_data(p[8], 100));

    //printf("****\ncheck block: %d\n", 9);
    valid = (test_helper_validate_data(p[9], 100) == 1) ? valid : 0;
    //printf("\nvalid %d\n", test_helper_validate_data(p[9], 100));


    //printf("data integrity check: %d\n", valid);

    worst_fit_memory_nuke();
    
    return valid;
}


/* test functions for best alloc */

/* test basic functionality */
int test_best_mem_too_big()
{
    void* p;
    best_fit_memory_init(1024);
    // request something of size 1024
    p = best_fit_alloc(1024);

    // cleanup memory
    best_fit_memory_nuke();    

    // expect NULL ptr for impossible request 
    return (p == NULL);
}

int test_best_dealloc()
{
    int res = 0;
    void* p;
    best_fit_memory_init(1024);
    p = best_fit_alloc(100);
    test_helper_write_data(p, 100);
    res = test_helper_validate_data(p, 100);
    best_fit_dealloc(NULL);
    res = test_helper_validate_data(p, 100);
    best_fit_memory_nuke();
    return res;
}

/* allocates, deallocates, allocates, check memory integrity */
int test_best_mem_large_cycle()
{
    void* p[10];
    int i = 0;
    int valid = 1;
    
    best_fit_memory_init(2048);

    // request a bunch of larger blocks
    for(i = 0; i < 10; i++)
    {
        p[i] = best_fit_alloc(100);
        // use the memory
        test_helper_write_data(p[i], 100);
    }    

    //printf("allocated 10 blocks of 100 bytes\n");
    //pool_dump();

    // make sure data is intact

    for (i = 0; i < 10; i++)
    {
        if (test_helper_validate_data(p[i], 100) != 1)
        {
            valid = 0;
        } 
    }

    //printf("data integrity check: %d\n", valid);

    // deallocate some data
    best_fit_dealloc(p[0]); 
    best_fit_dealloc(p[9]);
    best_fit_dealloc(p[4]);
    best_fit_dealloc(p[5]);
    best_fit_dealloc(p[3]);

    //printf("deallocated 5 blocks\n");
    //pool_dump();


    //printf("check integrity after dealloc\n");
    // check integrity
    for (i = 1; i < 9; i++)
    {
        if (i == 0, i == 3 || i == 4 || i == 5 || i == 9)
            break;
        //printf("****check block %d\n", i);
        if (test_helper_validate_data(p[i], 100) != 1)
        {
            valid = 0;
        } 
        //printf("\n****\n");
    }


    //printf("0000xdata integrity check: %d\n", valid);


    // allocate data
    p[0] = best_fit_alloc(200);
    //printf("0:%p\n", p[0]);
    test_helper_write_data(p[0], 200);
    //pool_dump();
    p[9] = best_fit_alloc(100);
    //printf("9:%p\n", p[9]);
    test_helper_write_data(p[9], 100);
    //pool_dump();
    p[4] = best_fit_alloc(300);
    //printf("4:%p\n", p[4]);
    test_helper_write_data(p[4], 300);
    p[5] = best_fit_alloc(50);
    //printf("5:%p\n", p[5]);
    test_helper_write_data(p[5], 50);
    p[3] = best_fit_alloc(4);
    //printf("3:%p\n", p[3]);
    test_helper_write_data(p[3], 4);

    //printf("re-allocate 5 blocks\n");
    //pool_dump();

    // check integrity
    //printf("****\ncheck block: %d\n", 0);
    valid = (test_helper_validate_data(p[0], 200) == 1) ? valid : 0;
    //printf("\nvalid %d\n", test_helper_validate_data(p[0], 200)); 

    //printf("****\ncheck block: %d\n", 1);
    valid = (test_helper_validate_data(p[1], 100) == 1) ? valid : 0;
    //printf("\nvalid %d\n", test_helper_validate_data(p[1], 100));

    //printf("****\ncheck block: %d\n", 2);
    valid = (test_helper_validate_data(p[2], 100) == 1) ? valid : 0;
    //printf("\nvalid %d\n", test_helper_validate_data(p[2], 100));

    //printf("****\ncheck block: %d\n", 3);
    valid = (test_helper_validate_data(p[3], 4) == 1) ? valid : 0;
    //printf("\nvalid %d\n", test_helper_validate_data(p[3], 4));

    //printf("****\ncheck block: %d\n", 4);
    valid = (test_helper_validate_data(p[4], 300) == 1) ? valid : 0;
    //printf("\nvalid %d\n", test_helper_validate_data(p[4], 300));

    //printf("****\ncheck block: %d\n", 5);
    valid = (test_helper_validate_data(p[5], 50) == 1) ? valid : 0;
    //printf("\nvalid %d\n", test_helper_validate_data(p[5], 50));

    //printf("****\ncheck block: %d\n", 6);
    valid = (test_helper_validate_data(p[6], 100) == 1) ? valid : 0;
    //printf("\nvalid %d\n", test_helper_validate_data(p[6], 100));

    //printf("****\ncheck block: %d\n", 7);
    valid = (test_helper_validate_data(p[7], 100) == 1) ? valid : 0;
    //printf("\nvalid %d\n", test_helper_validate_data(p[7], 100));

    //printf("****\ncheck block: %d\n", 8);
    valid = (test_helper_validate_data(p[8], 100) == 1) ? valid : 0;
    //printf("\nvalid %d\n", test_helper_validate_data(p[8], 100));

    //printf("****\ncheck block: %d\n", 9);
    valid = (test_helper_validate_data(p[9], 100) == 1) ? valid : 0;
    //printf("\nvalid %d\n", test_helper_validate_data(p[9], 100));


    //printf("data integrity check: %d\n", valid);

    best_fit_memory_nuke();
    
    return valid;
}


void test_all_frag_data()
{

}



int main(int argc, char *argv[])
{
	int algo = 0; // default algorithm to test is best fit  
	void *p, *q;
    void* ptr[5];

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <0/1>. 0 for best fit and 1 for worst fit \n", argv[0]);
		exit (1);
	} else if (!strcmp(argv[1], "1") || !strcmp(argv[1], "0")) {
		algo = atoi(argv[1]);
	} else {
		fprintf(stderr, "Invalid argument, please specify 0 or 1\n");
		exit(1);
	}
	
	if ( algo == 0 ) {

        printf("this print statment makes it all work\n");


        // run test cases
        printf("mem too big: %d\n", test_best_mem_too_big());
        printf("dealloc invalid: %d\n", test_best_dealloc());
        printf("testing large cycle:\n");
        printf("large cycle: %d\n", test_best_mem_large_cycle());


	} else if ( algo == 1 ) {

        printf("this print statment makes it all work\n");
    

        // run test cases
        printf("mem too big: %d\n", test_worst_mem_too_big());
        printf("dealloc invalid: %d\n", test_worst_dealloc());
        printf("testing large cycle:\n");
        printf("large cycle: %d\n", test_worst_mem_large_cycle());


	} else {
        
        // run frag test

	}


	return 0;
}
