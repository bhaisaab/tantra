#include <descriptor_table.h>
#include <gdt.h>

#define SEGMENT_BASE    0             // Use Flat addressing
#define SEGMENT_LIMIT   0xFFFFFFFF    // Allow it to access all of 4GB memory

#define GDT_ENTRIES 5

gdt_entry_t gdt_entries[GDT_ENTRIES];
gdt_ptr_t gdt_ptr;

static void gdt_set_gate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    // Setup the descriptor base address
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;

    // Setup the descriptor limits
    gdt_entries[num].limit_low = (limit & 0xFFFF);
    gdt_entries[num].granularity = ((limit >> 16) & 0x0F);

    // Finally, set up the granularity and access flags
    gdt_entries[num].granularity |= (gran & 0xF0);
    gdt_entries[num].access = access;
}

static void init_gdt()
{
    gdt_ptr.limit   = (sizeof(gdt_entry_t) * GDT_ENTRIES) - 1;
    gdt_ptr.base    = (uint32_t)&gdt_entries;

    gdt_set_gate(0, 0, 0, 0, 0);                              // Null segment
    gdt_set_gate(1, SEGMENT_BASE, SEGMENT_LIMIT, 0x9A, 0xCF); // Kernel Code segment
    gdt_set_gate(2, SEGMENT_BASE, SEGMENT_LIMIT, 0x92, 0xCF); // Kernel Data segment
    gdt_set_gate(3, SEGMENT_BASE, SEGMENT_LIMIT, 0xFA, 0xCF); // User mode code segment
    gdt_set_gate(4, SEGMENT_BASE, SEGMENT_LIMIT, 0xF2, 0xCF); // User mode data segment

    gdt_flush((uint32_t)&gdt_ptr);
}

void init_descriptor_tables()
{
   // Initialise the global descriptor table
   init_gdt();
}

