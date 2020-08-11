/* -*- Mode: C; c-basic-offset:4 ; -*- */
/*
 * (C) 2018 by Argonne National Laboratory.
 *     See COPYRIGHT in top-level directory.
 */

#include <shmem.h>
#include "oshmpi_impl.h"

void *OSHMPI_malloc(size_t size)
{
    void *ptr = NULL;

    OSHMPI_THREAD_ENTER_CS(&OSHMPI_global.symm_heap_mspace_cs);
    ptr = mspace_malloc(OSHMPI_global.symm_heap_mspace, size);
    OSHMPI_THREAD_EXIT_CS(&OSHMPI_global.symm_heap_mspace_cs);

    OSHMPI_DBGMSG("size %ld, ptr %p, disp 0x%lx\n", size, ptr,
                  (MPI_Aint) ptr - (MPI_Aint) OSHMPI_global.symm_heap_base);
    OSHMPI_barrier_all();
    return ptr;
}

#define CHECK_MEM_REGION(ptr, heap_base, heap_sz)   \
    ((MPI_Aint) (ptr) >= (MPI_Aint) (heap_base) && (MPI_Aint) (ptr) < (MPI_Aint) (heap_base) + (heap_sz))

void OSHMPI_free(void *ptr)
{
    OSHMPI_DBGMSG("ptr %p\n", ptr);
    OSHMPI_barrier_all();

    /* Check default symm heap */
    if (CHECK_MEM_REGION(ptr, OSHMPI_global.symm_heap_base, OSHMPI_global.symm_heap_size)) {
        OSHMPI_THREAD_ENTER_CS(&OSHMPI_global.symm_heap_mspace_cs);
        mspace_free(OSHMPI_global.symm_heap_mspace, ptr);
        OSHMPI_THREAD_EXIT_CS(&OSHMPI_global.symm_heap_mspace_cs);
    } else {
        /* Check space symm heaps */
        OSHMPI_space_t *space, *tmp;
        OSHMPI_THREAD_ENTER_CS(&OSHMPI_global.space_list.cs);
        LL_FOREACH_SAFE(OSHMPI_global.space_list.head, space, tmp) {
            if (CHECK_MEM_REGION(ptr, space->sobj_attr.base, space->sobj_attr.size))
                OSHMPI_space_free(space, ptr);
        }
        OSHMPI_THREAD_EXIT_CS(&OSHMPI_global.space_list.cs);
    }
}

void *OSHMPI_realloc(void *ptr, size_t size)
{
    void *rptr = NULL;

    OSHMPI_THREAD_ENTER_CS(&OSHMPI_global.symm_heap_mspace_cs);
    rptr = mspace_realloc(OSHMPI_global.symm_heap_mspace, ptr, size);
    OSHMPI_THREAD_EXIT_CS(&OSHMPI_global.symm_heap_mspace_cs);

    OSHMPI_DBGMSG("ptr %p size %ld -> %p\n", ptr, size, rptr);
    OSHMPI_barrier_all();
    return rptr;
}

void *OSHMPI_align(size_t alignment, size_t size)
{
    void *ptr = NULL;

    OSHMPI_THREAD_ENTER_CS(&OSHMPI_global.symm_heap_mspace_cs);
    ptr = mspace_memalign(OSHMPI_global.symm_heap_mspace, alignment, size);
    OSHMPI_THREAD_EXIT_CS(&OSHMPI_global.symm_heap_mspace_cs);

    OSHMPI_DBGMSG("alignment %ld size %ld -> %p\n", alignment, size, ptr);
    OSHMPI_barrier_all();
    return ptr;
}
