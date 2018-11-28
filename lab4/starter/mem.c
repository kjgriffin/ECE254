/**
 * @file memory.c
 * @brief: ECE254 Lab: memory allocation algorithm comparison template file
 * @author: 
 * @date: 2015-11-20
 */

/* includes */
#include <stdio.h> 
#include <stdlib.h> 
#include "mem.h"

/* defines */
enum State
{
    FREE=0, ALLOCATED=1
};

typedef struct mem_ctrl
{
    struct mem_ctrl* next;
    struct mem_ctrl* prev;
    size_t size;
    void* ptr; // actual start of memory block
    void* addr; // 4-byte aligned start address
    enum State state; 
} mem_ctrl;

/* global variables */
void* POOL_BEST;
void* POOL_WORST;

/* Functions */

void pool_dump(void* pool)
{
    pool = POOL_BEST;
    printf("*******************\n");
    printf("POOL DUMP %p\n", pool);
    mem_ctrl* curr = pool;
   
    while (curr != NULL)
    {
        printf("<%p>  next[%p] prev[%p] size(%lu) ptr[%p] addr[%p] state(%d)\n",
                curr, curr->next, curr->prev, curr->size, curr->ptr, curr->addr, (int)curr->state);
        curr = curr->next;
    }

    printf("********************\n");

}

int calc_byte_offset(void* addr)
{
    switch ((long)addr % 4)
    {
        case 0:
            return 0;
        case 1:
            return 3;
        case 2:
            return 2;
        case 3:
            return 1;
        default:
            return 0;
     }
}

/* memory initializer */
int best_fit_memory_init(size_t size)
{

    if (size <= sizeof(mem_ctrl))
    {
        return -1;
    } 


    POOL_BEST = malloc(sizeof(size));
    printf("malloc %p\n", POOL_BEST);
    printf("size %lu\n", sizeof(mem_ctrl));
    // setup first node
    mem_ctrl* first_block = POOL_BEST;

    first_block->prev = NULL;
    first_block->next = NULL;
    first_block->size = size - sizeof(mem_ctrl);
    first_block->ptr = POOL_BEST + sizeof(mem_ctrl);
    first_block->addr = NULL;
    first_block->state = FREE;
    //POOL_BEST = first_block;

	return 0;

}

int worst_fit_memory_init(size_t size)
{

	// To be completed by students
	// You call malloc once here to obtain the memory to be managed.
	return 0;

}

/* memory allocators */
void *best_fit_alloc(size_t size)
{
    // pointer to solution
    mem_ctrl* soln = POOL_BEST;
    
    // iterate throught the linked list to find the smallest block that fits the requested size
    mem_ctrl* curr;
    for(curr = POOL_BEST; curr != NULL; curr = curr->next)
    {
        // check if valid solution
        if (curr->size >= size + calc_byte_offset(curr->ptr))
        {
            // check if smaller and available
            if (curr->size < soln->size && curr->state == FREE)
            {
                soln = curr;
            }
        } 
    }



    // make sure solution works
    if (soln->size > size + sizeof(mem_ctrl) + calc_byte_offset(soln->ptr))
    {
        printf("soln %p", soln);
        // make a new node ( points to leftover)
        mem_ctrl* new_node = soln->ptr;

        new_node->size = soln->size - sizeof(mem_ctrl) - calc_byte_offset(soln->ptr) - size;
        new_node->ptr = soln->ptr + sizeof(mem_ctrl) + calc_byte_offset(soln->ptr) + size;
        new_node->addr = NULL;
        new_node->state = FREE;

        // update soln
        soln->ptr = soln->ptr + sizeof(mem_ctrl);
        soln->size = calc_byte_offset(soln->ptr) + size;
        soln->addr = soln->ptr + calc_byte_offset(soln->ptr);
        soln->state = ALLOCATED;

        // update linked list
        new_node->next = soln->next;
        if (new_node->next != NULL)
        {
            new_node->next->prev = new_node;
        }
        else
        {
            new_node->next = NULL;
        }
        new_node->prev = soln;
        soln->next = new_node;

        return soln->addr; 
    }
    else if (soln->size > size + calc_byte_offset(soln->ptr))
    {
        // just use this node
        // re-compute byte aligned pointer
        soln->addr = soln->ptr + calc_byte_offset(soln->ptr);
        // mark used
        soln->state = ALLOCATED;
        // return the address
        return soln->addr;
    }
    else
    {
        // no room
        return NULL;
    }


	return NULL;


}


void *worst_fit_alloc(size_t size)
{
	// To be completed by students
	return NULL;
}

/* memory de-allocator */
void best_fit_dealloc(void *ptr) 
{

	// To be completed by students
	return;
}

void worst_fit_dealloc(void *ptr) 
{

	// To be completed by students
	return;
}

/* memory algorithm metric utility function(s) */

/* count how many free blocks are less than the input size */ 
int best_fit_count_extfrag(size_t size)
{
	// To be completed by students
	return 0;
}

int worst_fit_count_extfrag(size_t size)
{
	// To be completed by students
	return 0;
}
