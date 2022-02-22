#include "mem_alloc.h"
#include <stdio.h>
 
static inline void *nbrk(void *address);
 
#ifdef NOVA
 
/**********************************/
/* nbrk() implementation for NOVA */
/**********************************/
 
static inline unsigned syscall2(unsigned w0, unsigned w1)
{
    asm volatile("   mov %%esp, %%ecx    ;"
                 "   mov $1f, %%edx      ;"
                 "   sysenter            ;"
                 "1:                     ;"
                 : "+a"(w0)
                 : "S"(w1)
                 : "ecx", "edx", "memory");
    return w0;
}
 
static void *nbrk(void *address)
{
    return (void *)syscall2(3, (unsigned)address);
}
#else
 
static void *nbrk(void *address)
{
    void *current_brk = sbrk(0);
    if (address != NULL)
    {
        int ret = brk(address);
        if (ret == -1)
            return NULL;
    }
    return current_brk;
}
 
#endif
 
#define NULL ((void *)0) // i lost 5 uploads, thx
 
typedef struct block
{
    unsigned long size;
    unsigned is_free;
    struct block *next;
} block_t;
 
block_t *head = NULL, *tail = NULL;
 
block_t *find_next_block(unsigned long size);
void move_tail();
void print_mem_list();
 
void *my_malloc(unsigned long size)
{
 
    block_t *block = find_next_block(size);
    //printf("[Allocator] -> Block found: %p\n", block);
 
    if (block != NULL)
    {
        //printf("[Allocator] -> No need to allocate more space\n");
        block->is_free = 0;
        return (void *)(block + 1);
    }
 
    //printf("[Allocator] -> Gotta create new block...\n");
    void *curr = nbrk(NULL);
    void *new_block = nbrk(sizeof(block_t) + size + curr);
 
    if (new_block == NULL)
    {
        //printf("[Allocator] -> Error while creating block!\n");
        return NULL;
    }
 
    block = new_block;
    block->size = size;
    block->is_free = 0;
    block->next = NULL;
 
    if (head == NULL)
    {
        head = block;
    }
 
    if (tail != NULL)
    {
        tail->next = block;
    }
 
    tail = block;
 
    //printf("[Allocator] -> New block created: %p\n", (void *)(block + 1));
    return (void *)(block + 1);
}
 
int my_free(void *address)
{
    void *curr_brk = nbrk(NULL);
    block_t *header = (block_t *)address - 1;
 
    //printf("[Allocator] -> Header found %p\n", header);
 
    if ((head == NULL && tail == NULL))
    {
        //printf("[Allocator] -> Free error detected\n");
        return 1;
    }
 
    if (header != NULL)
    {
        if (header->is_free == 1)
        {
            //printf("[Allocator] -> Free free block detected\n");
            return 1;
        }
    }
 
    if (address + header->size == curr_brk) // at the end of the heap? -> release to OS
    {
        //printf("[Allocator] -> Releasing to OS\n");
        if (head == tail)
        {
            head = NULL;
            tail = NULL;
        }
        else
        {
            move_tail();
        }
 
        return nbrk(curr_brk - header->size - sizeof(block_t)) == NULL;
    }
    // or just mark as free to use
    //printf("[Allocator] -> Block is now free\n");
    header->is_free = 1;
    return 0;
}
 
block_t *find_next_block(unsigned long size)
{
    //printf("[Allocator] -> Finding free block...\n");
    block_t *curr = head;
    while (curr != NULL)
    {
        if (curr->is_free && curr->size >= size)
            return curr;
        curr = curr->next;
    }
    return NULL;
}
 
void move_tail()
{
    //printf("[Allocator] -> Moving tail...\n");
    block_t *curr = head;
    while (curr != NULL)
    {
        if (tail == curr->next)
        {
            curr->next = NULL;
            tail = curr;
        }
        curr = curr->next;
    }
}
 
void print_mem_list()
{
    block_t *curr = head;
    printf("head = %p, tail = %p \n", (void *)head, (void *)tail);
    while (curr)
    {
        printf("addr = %p, size = %lu, is_free=%u, next=%p\n",
               (void *)curr, curr->size, curr->is_free, (void *)curr->next);
        curr = curr->next;
    }
}
