#include "ec.h"
#include "ptab.h"
#include "stdio.h"
#include "string.h"
  
typedef enum
{
    sys_print = 1,
    sys_sum = 2,
    sys_break = 3,
    sys_thr_create = 4,
    sys_thr_yield = 5,
} Syscall_numbers;
  
void Ec::syscall_handler(uint8 a)
{
    // Get access to registers stored during entering the system - see
    // entry_sysenter in entry.S
    Sys_regs *r = current->sys_regs();
    Syscall_numbers number = static_cast<Syscall_numbers>(a);
  
    switch (number)
    {
    case sys_print:
    {
        char *data = reinterpret_cast<char *>(r->esi);
        unsigned len = r->edi;
        for (unsigned i = 0; i < len; i++)
            printf("%c", data[i]);
        break;
    }
    case sys_sum:
    {
        // Naprosto nepotřebné systémové volání na sečtení dvou čísel
        int first_number = r->esi;
        int second_number = r->edi;
        r->eax = first_number + second_number;
        break;
    }
    case sys_break:
    {
        // ========================== Potential errors ==========================
        if (r->esi == 0)
        {
            //printf("[Break] -> Actual address is: %lx \n", Ec::break_current);
            r->eax = Ec::break_current;
            break;
        }
  
        if (r->esi >= 0xC0000000 || r->esi < Ec::break_min)
        {
            //printf("[Break] -> Address isn't in allowed interval: %lu =>x <%lu, 0xC0000000>\n", r->esi, Ec::break_min);
            r->eax = 0x0;
            break;
        }
  
        // ========================== Start vars ==========================
  
        mword address = r->esi;
        mword start_break = Ec::break_current;   // doesn't change
        mword break_on_move = (start_break % PAGE_SIZE == 0) ? start_break : start_break - (start_break % PAGE_SIZE) + PAGE_SIZE; // changes
  
        // ========================== Forward break ==========================
  
        mword allocated = break_on_move;
        mword need_to_allocate = (address % PAGE_SIZE == 0) ? address : address - (address % PAGE_SIZE) + PAGE_SIZE;
  
        if (address > start_break)
        {
            //printf("[Break] -> Forward break\n");
  
            if (allocated == need_to_allocate)
            {
                //printf("[Break] -> nothing to allocate\n");
                memset(reinterpret_cast<void *>(start_break), 0x00, address - start_break);
            }
  
            else
            {
                memset(reinterpret_cast<void *>(start_break), 0x00, allocated - start_break);
  
                int page_count = (need_to_allocate - allocated) / PAGE_SIZE;
                //printf("[Break] -> Need to allocate: %d\n", page_count);
  
                for (int i = 0; i < page_count; i++)
                {
                    void *newPage = Kalloc::allocator.alloc_page(1, Kalloc::FILL_0);
  
                    if (!newPage)
                    {
                        //printf("Page alloc error on %lx\n", break_on_move);
                        r->eax = 0x0;
                        break;
                    }
  
                    bool mapping = Ptab::insert_mapping(
                        break_on_move,
                        Kalloc::virt2phys(newPage),
                        Ptab::PRESENT | Ptab::RW | Ptab::USER);
  
                    if (!mapping)
                    {
                        //printf("Mapping error\n");
                        r->eax = 0x0;
                        break;
                    }
  
                    break_on_move += PAGE_SIZE;
                }
  
                if (r->eax == 0x0) // alloc or mapping fault
                {
                    int del = (break_on_move - start_break) / PAGE_SIZE;
                    for (int i = 0; i < del; i++)
                    {
                        mword page = Ptab::get_mapping(break_on_move - PAGE_SIZE) & ~(Ptab::RW | Ptab::USER | Ptab::PRESENT);
  
                        Ptab::insert_mapping(
                            break_on_move - PAGE_SIZE,
                            page,
                            Ptab::RW | Ptab::USER);
  
                        Kalloc::allocator.free_page(Kalloc::phys2virt(page & 0xFFFFFF00));
  
                        break_on_move -= PAGE_SIZE;
                    }
                }
            }
  
            if (r->eax == 0x0)
            {
                break;
            }
        }
  
        // ========================== Backward break ==========================
  
        else if (address < start_break) // backward break
        {
            //printf("[Break] -> Backward break\n");
  
            if (allocated == need_to_allocate)
            {
                //printf("[Break] -> nothing to deallocate\n");
                memset(reinterpret_cast<void *>(address), 0x00, start_break - address);
            }
  
            else
            {
                memset(reinterpret_cast<void *>(address), 0x00, start_break - need_to_allocate);
  
                int page_count = (allocated - need_to_allocate) / PAGE_SIZE;
                //printf("[Break] -> Need to delete: %d\n", page_count);
  
                for (int i = 0; i < page_count; i++)
                {
                    mword page = Ptab::get_mapping(break_on_move - PAGE_SIZE) & ~(Ptab::RW | Ptab::USER | Ptab::PRESENT);
  
                    Ptab::insert_mapping(
                        break_on_move - PAGE_SIZE,
                        page,
                        Ptab::RW | Ptab::USER);
  
                    Kalloc::allocator.free_page(Kalloc::phys2virt(page & 0xFFFFFF00));
  
                    break_on_move -= PAGE_SIZE;
                }
            }
        }
  
        Ec::break_current = address;
        r->eax = start_break;
        break;
    }
  
    default:
        printf("unknown syscall %d\n", number);
        break;
    };
  
    ret_user_sysexit();
}