/*
 * Copyright (C) 2013-2015 Dmitry A.Steklenev
 */

#include <stdlib.h>
#include <malloc.h>
#include "pm_error.h"
#include "pm_debuglog.h"
#include "pm_memory.h"

#if defined(DEBUG) && DEBUG >= 2

  static int blocks   = 0;
  static int used     = 0;
  static int used_max = 0;

  #define DEBUG_MALLOC( p )                                                               \
                                                                                          \
  if( p ) {                                                                               \
    if(( used += _msize(p)) > used_max ) {                                                \
      used_max = used;                                                                    \
    }                                                                                     \
    blocks++;                                                                             \
                                                                                          \
    DEBUGLOG2(( "%s allocate %d bytes at %08X, total %u blocks, %u bytes, %u max used\n", \
                 __FUNCTION__, _msize(p), p, blocks, used, used_max ));                   \
  }

  #define DEBUG_FREE( p )                                                                 \
                                                                                          \
  if( p ) {                                                                               \
    blocks--;                                                                             \
    used -= _msize(p);                                                                    \
                                                                                          \
    DEBUGLOG(( "%s free %d bytes at %08X, remain %u blocks, %u bytes, %u max used\n",     \
               __FUNCTION__, _msize(p), p, blocks, used, used_max ));                     \
  }
#else
  #define DEBUG_MALLOC( p )
  #define DEBUG_FREE( p )
#endif

/* Reserves a block of storage of size bytes.
 *
 * Returns a pointer to the reserved space. The storage space to which
 * the return value points is suitably aligned for storage of any type of object.
 * The return value is NULL if size was specified as zero.
 */

void* xmalloc( size_t size )
{
  void* p = malloc( size );

  if( !p && size ) {
    PM_THROW_ERROR( ENOMEM, "CLIB", strerror( ENOMEM ));
  }

  DEBUG_MALLOC( p );
  return p;
}

/* Changes the size of a previously reserved storage block.
 *
 * Returns a pointer to the reallocated storage block. If size is 0,
 * returns NULL. The storage to which the return value points is aligned for
 * storage of any type of object.
 */

void* xrealloc( void* p, size_t size )
{
  DEBUG_FREE( p );
  p = realloc( p, size );

  if( !p && size ) {
    PM_THROW_ERROR( ENOMEM, "CLIB", strerror( ENOMEM ));
  }

  DEBUG_MALLOC( p );
  return p;
}

/**
 * Reserve and initialize storage
 *
 * Reserves storage space for an array of num elements, each of length size
 * bytes. Then gives all the bits of each element an initial value of 0.
 *
 * The storage space to which the return value points is suitably aligned for storage
 * of any type of object. The return value is NULL if there is not enough storage, or
 * if num or size is 0.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

void* xcalloc( size_t num, size_t size )
{
  void* p = calloc( num, size );

  if( !p && size && num ) {
    PM_THROW_ERROR( ENOMEM, "CLIB", strerror( ENOMEM ));
  }

  DEBUG_MALLOC( p );
  return p;
}

/**
 * Frees a block of storage.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

void xfree( void *p )
{
  if( p ) {
    DEBUG_FREE( p );
    free( p );
  }
}

/**
 * Reserves storage space for a copy of string.
 *
 * Returns a pointer to the storage space containing the copied string.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

char* xstrdup( const char* string )
{
  if( string ) {
    size_t size = strlen( string ) + 1;
    char*  p = (char*)malloc( size );

    if( !p ) {
      PM_THROW_ERROR( ENOMEM, "CLIB", strerror( ENOMEM ));
    }

    DEBUG_MALLOC( p );
    return (char*)memcpy( p, string, size );
  } else {
    return NULL;
  }
}

/* Allocates size bytes of storage, suitably aligned to represent any object
 * of that size, and returns a non-null pointer to the first byte of
 * this block.
 */

void* operator new( size_t size )
{
  void* p = malloc( size );

  if( !p ) {
    PM_THROW_ERROR( ENOMEM, "CLIB", strerror( ENOMEM ));
  }

  DEBUG_MALLOC( p );
  return p;
}

void* operator new[]( size_t size ) {
  return operator new( size );
}

/* Deallocates the memory block pointed by p (if not null), releasing
 * the storage space previously allocated to it by a call to operator
 * new and rendering that pointer location invalid.
 */

void operator delete( void *p )
{
  if( p ) {
    DEBUG_FREE( p );
    free( p );
  }
}

void operator delete[]( void *p ) {
  return operator delete( p );
}

