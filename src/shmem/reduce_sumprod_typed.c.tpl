/* TPL_HEADER_START */
/* -*- Mode: C{} c-basic-offset:4 {} -*- */
/*
 *  (C) 2018 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 *
 * This file is automatically generated. DO NOT EDIT.
 */

#include <shmem.h>
#include "oshmpi_impl.h"
/* TPL_HEADER_END */

void shmem_TYPENAME_sum_to_all(TYPE * dest, const TYPE * source, int nreduce, int PE_start,
                               int logPE_stride, int PE_size, TYPE * pWrk, long *pSync)
{
    OSHMPI_allreduce(dest, source, nreduce, MPI_TYPE, MPI_SUM, PE_start, logPE_stride, PE_size);
}

void shmem_TYPENAME_prod_to_all(TYPE * dest, const TYPE * source, int nreduce, int PE_start,
                                int logPE_stride, int PE_size, TYPE * pWrk, long *pSync)
{
    OSHMPI_allreduce(dest, source, nreduce, MPI_TYPE, MPI_PROD, PE_start, logPE_stride, PE_size);
}