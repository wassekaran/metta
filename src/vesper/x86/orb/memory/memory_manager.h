//
// Copyright 2007 - 2009, Stanislav Karchebnyy <berkus+metta@madfire.net>
//
// Distributed under the Boost Software License, Version 1.0.
// (See file LICENSE_1_0.txt or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "types.h"
#include "heap.h"
#include "multiboot.h"

//TODO: move these to the linker script
#define K_HEAP_START              0xF0000000
#define K_HEAP_INITIAL_SIZE       0x100000
#define K_HEAP_END                0xFFFFF000
// #define U_HEAP_START         0xD0000000
// #define U_HEAP_INITIAL_SIZE  0x100000
// #define U_HEAP_END           0xDFFFFFFF
#define STACK_START             (0xF8000000-0x4)
#define STACK_INITIAL_SIZE      0x10000
#define STACK_END               0xF0000000
// #define KERNEL_START            0xC0000000 use K_SPACE_START defined by linker

#define STACK_ADDRESS(x)  ((address_t)x <= STACK_START && (address_t)x > STACK_END)
#define K_HEAP_ADDRESS(x) ((address_t)x >= K_HEAP_START && (address_t)x <= K_HEAP_END)
// #define U_HEAP_ADDRESS(x) ((address_t)x >= U_HEAP_START && (address_t)x <= U_HEAP_END)

class page_t;
class page_directory_t;

#define kmemmgr memory_manager::self()

/*!
* Kernel memory manager controls physical and virtual memory allocation.
*/
class memory_manager_t
{
public:
    /*!
    * Heap can call our private alloc_frame() functions.
    */
    friend class heap_t;

    /*!
    * Singleton accessor.
    */
    static memory_manager_t& self();

    /*!
    * Normal constructor - passes the address of end of memory.
    * Initialises paging and sets up a standard kernel page directory. //FIXME
    * Enables paging, then maps some pages for the heap.
    */
    void init(address_t mem_end, multiboot_t::header_t::memmap_t* mmap);

    /*!
    * Allocate @c size bytes, returning the physical address of the
    * segment allocated in physical_location if physical_location != NULL.
    * @param page_align align returned address on a page boundary.
    * @param[out] physical_location return physical address of the allocated space.
    */
    void* allocate(uint32_t size, bool page_align = false, address_t* physical_location = NULL);

    /*!
    * Deallocate the memory allocated to p.
    */
    void free(void* p);

    /*!
    * Changes the size of the memory block pointed to by @c ptr to @c size bytes.
    * The contents will be unchanged up to the minimum of the old and new sizes;
    * newly allocated memory will be uninitialized. If @c ptr is NULL, then the call is
    * equivalent to allocate(size), for all values of size; if @c size is equal to zero,
    * and @c ptr is not NULL, then the call is equivalent to free(ptr).
    * Unless ptr is NULL, it must have been returned by an earlier call to allocate()
    * or reallocate(). If the area pointed to was moved, a free(ptr) is done.
    */
    void* reallocate(void* ptr, size_t size);

    /*!
    * Accessor functions for heapInitialised and placementAddress.
    */
    bool is_heap_initialized() { return heap_initialized; }
    address_t get_placement_address() { return placement_address; }
    void set_placement_address(address_t a) { placement_address = a; }

    /*!
    * Forces the placementAddress variable to be PAGE_SIZE aligned.
    */
    void align_placement_address();

    /*!
    * Accessor functions for kernel_directory, current_directory
    */
    inline page_directory* get_kernel_directory()  { return kernel_directory; }
    inline page_directory* get_current_directory() { return current_directory; }
    void set_current_directory(page_directory* p) { current_directory = p; }

    /*!
    * Causes the given range of virtual memory to get allocated physical
    * memory.
    */
    void allocate_range(address_t start_address, uint32_t size);

    /*!
    * Returns the size of the heap. For analysis purposes.
    */
    uint32_t get_heap_size();

    /*!
    * Checks the heap for integrity.
    */
    void check_integrity();

private:
    /*!
    * Default constructor, does nothing.
    */
    memory_manager_t();

private:
    /*!
    * Total number of physical memory frames. TODO should be in frame_allocator
    */
//     uint32_t n_frames;

    /*!
    * Has the kernel heap been initialised yet?
    */
    bool heap_initialised;

    /*!
    * The kernel heap
    */
    heap_t heap;

    /*!
    * The page frame allocator.
    */
    page_frame_allocator_t frame_allocator;

    /*!
    * Before the heap is initialised, this holds the next available location
    * for 'placement new' to be called on.
    */
    address_t placement_address;

    /*!
    * The currently active page directory
    */
    page_directory *current_directory;

    /*!
    * Pointer to the "master" page directory. This holds page table pointers for kernel
    * space. All other page directories must match the entries in here to maintain easy
    * consistency of kernel-space over memory spaces.
    */
    page_directory *kernel_directory;
};

// kate: indent-width 4; replace-tabs on;
// vim: set et sw=4 ts=4 sts=4 cino=(4 :