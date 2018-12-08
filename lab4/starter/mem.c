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

#define PRINTBLOCK
#define POOLDUMP

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

void best_fit_memory_nuke();

void print_block(mem_ctrl* block)
{
#ifdef PRINTBLOCK
    printf("<%p>  next[%p] prev[%p] size(%lu) ptr[%p] addr[%p] state(%d)\n",
               block, block->next, block->prev, block->size, block->ptr, block->addr, (int)block->state);
#endif
}

void pool_dump(void* pool)
{
#ifdef POOLDUMP
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
#endif
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


void best_fit_memory_nuke()
{
    free(POOL_BEST); 
}

/* memory initializer */
int best_fit_memory_init(size_t size)
{

    if (size <= sizeof(mem_ctrl))
    {
        return -1;
    } 


    POOL_BEST = malloc(sizeof(size));
    //printf("malloc %p\n", POOL_BEST);
    //printf("size %lu\n", sizeof(mem_ctrl));
    // setup first node
    mem_ctrl* first_block = POOL_BEST;

    first_block->prev = NULL;
    first_block->next = NULL;
    first_block->size = size - sizeof(mem_ctrl);
    first_block->ptr = POOL_BEST + sizeof(mem_ctrl);
    first_block->addr = NULL;
    first_block->state = FREE;

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
    size_t smallest = 10000000; // TODO pick bigger number here
    
    // iterate throught the linked list to find the smallest block that fits the requested size
    mem_ctrl* curr = POOL_BEST;
    while(curr != NULL)
    {
        //printf("searching: ");
        //print_block(curr);

        // check if valid solution
        if (curr->size >= size + calc_byte_offset(curr->ptr))
        {
            //printf("\talloc trying\n");

            // check if smaller and available
            if (curr->size < smallest && curr->state == FREE)
            {
                //printf("\t\talloc updating: ");
                //print_block(curr);

                soln = curr;
                smallest = soln->size;
            }
        } 
        curr = curr->next;
    }

    printf("####alloc soln\n");
    print_block(soln);

    // make sure solution works
    if (soln->size > size + sizeof(mem_ctrl) + calc_byte_offset(soln->ptr))
    {
        //printf("splitting block\n");
        // make new node for remainder
        mem_ctrl* remainder = soln->ptr + size + calc_byte_offset(soln->ptr);
        //printf("allocate remainder @%p\n", remainder);
        // compute new nodes size
        remainder->size = soln->size - size - calc_byte_offset(soln->ptr) - sizeof(mem_ctrl);
        // update pointer for remainder block
        remainder->ptr = (void*)((size_t)remainder + sizeof(mem_ctrl));
        //printf("remainder block @%p\n", remainder->ptr);
        remainder->addr = NULL;
        remainder->state = FREE;

        //update solution
        soln->size = calc_byte_offset(soln->ptr) + size;
        soln->addr = calc_byte_offset(soln->ptr) + soln->ptr;
        soln->state = ALLOCATED;


        // update linked list
        remainder->next = soln->next;
        if (remainder->next != NULL)
        {
            remainder->next->prev = remainder;
        }
        else
        {
            remainder->next = NULL;
        }
        remainder->prev = soln;
        soln->next = remainder;

        return soln->addr; 
    }
    else if (soln->size >= size + calc_byte_offset(soln->ptr))
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
    // traverse the list and find the requested memory block, mark as free
    mem_ctrl* curr = POOL_BEST;
    mem_ctrl* dealloc = NULL;
    while(curr != NULL)
    {
        if (curr->addr == ptr)
        {
            curr->state = FREE;
            dealloc = curr;
            break;
        }
        curr = curr->next;
    }

    // do nothing if not valid
    if (dealloc == NULL)
    {
        return;
    }
   
    // mark free
    dealloc->state = FREE; 


    // check the next and prev block's to see if they are free, if so coalless
    mem_ctrl* before = NULL;


    // coaless before if empty
    if (dealloc->prev != NULL)
    {
        before = dealloc->prev;
        if (before->state == FREE)
        {
            // coalless before
            //printf("coalless before\n");
            //print_block(before);

            // compute new space for before block
            before->size = before->size + dealloc->size + sizeof(mem_ctrl); 
            // remove dealloc from linked list
            before->next = dealloc->next;
            if (dealloc->next != NULL)
            {
                dealloc->next->prev = before;
            }

            // update dealloc to point to before
            dealloc = before;
        }
    }


    // coaless after if empty
    mem_ctrl* after = NULL;
    if (dealloc->next != NULL)
    {
        after = dealloc->next;
        if (after->state == FREE)
        {
            // coalless after
            //printf("coalless after\n");
            //print_block(after);

            // compute new space for dealloc block
            dealloc->size = dealloc->size + after->size + sizeof(mem_ctrl);
            // remove after from linked list
            dealloc->next = after->next;
            if (after->next != NULL)
            {
                after->next->prev = dealloc;
            }
        }
    }

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
    int frags = 0;
    mem_ctrl* curr = POOL_BEST;
    while(curr != NULL)
    {
        if (curr->size < size && curr->state == FREE)
        {
            frags++;
        }
        curr = curr->next;
    } 

	return frags;
}

int worst_fit_count_extfrag(size_t size)
{
	// To be completed by students
	return 0;
}
