/*
 * Copyright (C) 2015 Dmitry A.Steklenev
 */

#ifndef PM_MEMORY_H
#define PM_MEMORY_H

#include "pm_os2.h"
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @pkg Memory_Functions */

/**
 * Memory allocation functions.
 *
 * These functions are utility functions for memory  allocation.
 * <i>xmalloc</i>, <i>xrealloc</i>, and  <i>xfree</i> are error
 * checking versions of the standard library routines <i>malloc</i>,
 * <i>realloc</i>, and <i>free</i>, respectively. They are guaranteed
 * to never return unless there was no problem. Hence, the caller does
 * not need to check for  a  NULL return value,  and  the code that
 * calls these functions is simpler due to the lack of error checks.
 *
 * @pkgdoc Memory_Functions
 */

/**
 * Reserves a block of storage of size bytes.
 *
 * Returns a pointer to the reserved space. The storage space to which
 * the return value points is suitably aligned for storage of any type of object.
 * The return value is NULL if size was specified as zero.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

void* xmalloc( size_t size );

/**
 * Changes the size of a previously reserved storage block.
 *
 * Returns a pointer to the reallocated storage block. The storage location of
 * the block may be moved by the <i>realloc</i> function. Thus, the <i>p</i> argument
 * to <i>xrealloc</i> is not necessarily the same as the return value.
 *
 * If <i>size</i> is 0, <i>xrealloc</i> returns NULL.
 *
 * The storage to which the return value points is aligned for storage of any type of object.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

void* xrealloc( void* p, size_t size );

/**
 * Reserve and initialize storage.
 *
 * Reserves storage space for an array of <i>num</i> elements, each of length <i>size</i>
 * bytes. Then gives all the bits of each element an initial value of 0.
 *
 * The storage space to which the return value points is suitably aligned for storage
 * of any type of object. The return value is NULL if there is not enough storage, or
 * if <i>num</i> or <i>size</i> is 0.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

void* xcalloc( size_t num, size_t size );

/**
 * Frees a block of storage.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

void xfree( void *p );

/**
 * Reserves storage space for a copy of string.
 *
 * Returns a pointer to the storage space containing the copied string.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

char* xstrdup( const char* string );

#ifdef __cplusplus
}
#endif

#endif /* PM_MEMORY_H */
