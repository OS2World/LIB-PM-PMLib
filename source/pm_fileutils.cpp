/*
 * Copyright (C) 2004-2013 Dmitry A.Steklenev
 */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>

#include "pm_fileutils.h"
#include "pm_debuglog.h"

#define  isslash( c ) ( c == '/' || c == '\\' )

/* Returns TRUE if the specified location is a URL. */
BOOL
is_url( const char* location )
{
  if(!( isalpha( location[0] ) && location[1] == ':' ))
  {
    const char* pc;

    // If the parse string contain a colon after the 1st character
    // and before any characters not allowed as part of a scheme
    // name (i.e. any not alphanumeric, '+', '.' or '-'),
    // the scheme of the url is the substring of chars up to
    // but not including the first colon. These chars and the
    // colon are then removed from the parse string before
    // continuing.

    for( pc = location; isalnum( *pc ) || ( *pc == '+' ) || ( *pc == '-' ) || ( *pc == '.' ); ++pc )
    {}

    if( *pc == ':' ) {
      return TRUE;
    }
  }
  return FALSE;
}

/* Returns TRUE if the specified location is a regular file. */
BOOL
is_file( const char* location ) {
  return *location && !is_url( location );
}

/* Returns the drive letter followed by a colon (:)
   if a drive is specified in the location. */
char*
sdrive( char* result, const char* location, size_t size )
{
  if( isalpha( location[0] ) && location[1] == ':' ) {
    strlcpy( result, location, size > 3 ? 3 : size );
  } else {
    *result = 0;
  }
  return result;
}

/* Returns the scheme followed by a colon (:)
   of the specified location. */
char*
scheme( char* result, const char* location, size_t size )
{
  if( !isalpha( location[0] ) || location[1] != ':' )
  {
    const char* pc;

    // If the parse string contain a colon after the 1st character
    // and before any characters not allowed as part of a scheme
    // name (i.e. any not alphanumeric, '+', '.' or '-'),
    // the scheme of the url is the substring of chars up to
    // but not including the first colon.

    for( pc = location; isalnum( *pc ) || ( *pc == '+' ) || ( *pc == '-' ) || ( *pc == '.' ); pc++ )
    {}

    if( *pc == ':' )
    {
      size_t remain = pc - location + 2;
      strlcpy( result, location, remain < size ? remain : size );
    }
  } else if( size ) {
    *result = 0;
  }
  return result;
}

/* Returns the base file name with file extension. */
char*
sfnameext( char *result, const char* location, size_t size )
{
  const char* phead  = location;
  const char* ptail  = location + strlen( location );
  const char* pc;
  BOOL        is_url = FALSE;

  // If the parse string contain a colon after the 1st character
  // and before any characters not allowed as part of a scheme
  // name (i.e. any not alphanumeric, '+', '.' or '-'),
  // the scheme of the url is the substring of chars up to
  // but not including the first colon. These chars and the
  // colon are then removed from the parse string before
  // continuing.
  while( isalnum( *phead ) || ( *phead == '+' ) || ( *phead == '-' ) || ( *phead == '.' )) {
    ++phead;
  }
  if( *phead != ':' ) {
    phead = location;
  } else {
    phead++;
    // If the scheme consists more than of one symbol or its first
    // symbol not the alphabetic character that it is exact not a
    // name of the drive.
    if( phead - location > 2 || !isalpha( *location )) {
      is_url = TRUE;
    }
  }

  // Skip location (user:password@host:port part of the url
  // or \\server part of the regular pathname) on the front.
  if(( is_url || location == phead ) && isslash( phead[0] ) && isslash( phead[1] )) {
    phead += 2;
    while( *phead && !isslash( *phead )) {
      ++phead;
    }
  }

  // Remove fragment identifier, parameters or query information,
  // if any, from the back of the url.
  if( is_url ) {
    if(( pc = strpbrk( phead, "#?;" )) != NULL ) {
      ptail = pc;
    }
  }

  // Skip path name.
  for( pc = phead; *pc && pc < ptail; pc++ ) {
    if( isslash( *pc )) {
      phead = pc + 1;
    }
  }

  // Remainder is a file name.
  size_t remain = ptail - phead + 1;
  strlcpy( result, phead, remain < size ? remain : size );
  return result;
}

/* Returns the file name extension, if any,
   including the leading period (.). */
char*
sfext( char* result, const char* location, size_t size )
{
  const char* phead  = location;
  const char* ptail  = location + strlen( location );
  const char* pc;
  BOOL        is_url = FALSE;

  // If the parse string contain a colon after the 1st character
  // and before any characters not allowed as part of a scheme
  // name (i.e. any not alphanumeric, '+', '.' or '-'),
  // the scheme of the url is the substring of chars up to
  // but not including the first colon. These chars and the
  // colon are then removed from the parse string before
  // continuing.
  while( isalnum( *phead ) || ( *phead == '+' ) || ( *phead == '-' ) || ( *phead == '.' )) {
    ++phead;
  }
  if( *phead != ':' ) {
    phead = location;
  } else {
    phead++;
    // If the scheme consists more than of one symbol or its first
    // symbol not the alphabetic character that it is exact not a
    // name of the drive.
    if( phead - location > 2 || !isalpha( *location )) {
      is_url = TRUE;
    }
  }

  // Skip location (user:password@host:port part of the url
  // or \\server part of the regular pathname) on the front.
  if(( is_url || location == phead ) && isslash( phead[0] ) && isslash( phead[1] )) {
    phead += 2;
    while( *phead && !isslash( *phead )) {
      ++phead;
    }
  }

  // Remove fragment identifier, parameters or query information,
  // if any, from the back of the url.
  if( is_url ) {
    if(( pc = strpbrk( phead, "#?;" )) != NULL ) {
      ptail = pc;
    }
  }

  // Skip path name.
  for( pc = phead; *pc && pc < ptail; pc++ ) {
    if( isslash( *pc )) {
      phead = pc + 1;
    }
  }

  // Remainder is a file name, search file extension.
  for( pc = ptail - 1; pc > phead && *pc != '.'; pc-- )
  {}

  if( *pc == '.' && pc != phead )
  {
    size_t remain = ptail - pc + 1;
    strlcpy( result, pc, remain < size ? remain : size );
  } else if( size ) {
    *result = 0;
  }

  return result;
}

/* Replaces an extension of the specified file.
 * The specified extension may include the leading period (.).
 */

char*
sfextrep( char* filename, const char* ext, int size )
{
  char old_ext[_MAX_EXT];
  int  dot = ( ext[0] == '.' );

  if( *filename ) {
    sfext( old_ext, filename, sizeof( old_ext ));

    if( stricmp( old_ext + 1 - dot, ext ) != 0 ) {
      filename[ strlen( filename ) - strlen( old_ext )] = 0;
      strlcat( filename, ".", size );
      strlcat( filename, ext + dot, size );
    }
  }

  return filename;
}

/* Returns the base file name without any extensions. */
char*
sfname( char* result, const char* location, size_t size )
{
  const char* phead  = location;
  const char* ptail  = location + strlen( location );
  const char* pc;
  BOOL        is_url = FALSE;

  // If the parse string contain a colon after the 1st character
  // and before any characters not allowed as part of a scheme
  // name (i.e. any not alphanumeric, '+', '.' or '-'),
  // the scheme of the url is the substring of chars up to
  // but not including the first colon. These chars and the
  // colon are then removed from the parse string before
  // continuing.
  while( isalnum( *phead ) || ( *phead == '+' ) || ( *phead == '-' ) || ( *phead == '.' )) {
    ++phead;
  }
  if( *phead != ':' ) {
    phead = location;
  } else {
    phead++;
    // If the scheme consists more than of one symbol or its first
    // symbol not the alphabetic character that it is exact not a
    // name of the drive.
    if( phead - location > 2 || !isalpha( *location )) {
      is_url = TRUE;
    }
  }

  // Skip location (user:password@host:port part of the url
  // or \\server part of the regular pathname) on the front.
  if(( is_url || location == phead ) && isslash( phead[0] ) && isslash( phead[1] )) {
    phead += 2;
    while( *phead && !isslash( *phead )) {
      ++phead;
    }
  }

  // Remove fragment identifier, parameters or query information,
  // if any, from the back of the url.
  if( is_url ) {
    if(( pc = strpbrk( phead, "#?;" )) != NULL ) {
      ptail = pc;
    }
  }

  // Skip path name.
  for( pc = phead; *pc && pc < ptail; pc++ ) {
    if( isslash( *pc )) {
      phead = pc + 1;
    }
  }

  // Remainder is a file name, skip file extension.
  for( pc = ptail - 1; pc > phead && *pc != '.'; pc-- )
  {}

  if( *pc == '.' && pc != phead ) {
    ptail = pc;
  }

  size_t remain = ptail - phead + 1;
  strlcpy( result, phead, remain < size ? remain : size );
  return result;
}

/* Returns the drive letter or scheme and the path of
   subdirectories, if any, including the trailing slash.
   Slashes (/), backslashes (\), or both may be present
   in location. */
char*
sdrivedir( char *result, const char* location, size_t size )
{
  const char* phead  = location;
  const char* ptail  = location + strlen( location );
  const char* pc;
  BOOL        is_url = FALSE;

  // If the parse string contain a colon after the 1st character
  // and before any characters not allowed as part of a scheme
  // name (i.e. any not alphanumeric, '+', '.' or '-'),
  // the scheme of the url is the substring of chars up to
  // but not including the first colon. These chars and the
  // colon are then removed from the parse string before
  // continuing.
  while( isalnum( *phead ) || ( *phead == '+' ) || ( *phead == '-' ) || ( *phead == '.' )) {
    ++phead;
  }
  if( *phead != ':' ) {
    phead = location;
  } else {
    phead++;
    // If the scheme consists more than of one symbol or its first
    // symbol not the alphabetic character that it is exact not a
    // name of the drive.
    if( phead - location > 2 || !isalpha( *location )) {
      is_url = TRUE;
    }
  }

  // Skip location (user:password@host:port part of the url
  // or \\server part of the regular pathname) on the front.
  if(( is_url || location == phead ) && isslash( phead[0] ) && isslash( phead[1] )) {
    phead += 2;
    while( *phead && !isslash( *phead )) {
      ++phead;
    }
  }

  // Remove fragment identifier, parameters or query information,
  // if any, from the back of the url.
  if( is_url ) {
    if(( pc = strpbrk( phead, "#?;" )) != NULL ) {
      ptail = pc;
    }
  }

  // Search end of the path name.
  for( pc = ptail - 1; pc >= phead && !isslash( *pc ); pc-- )
  {}

  ptail = pc + 1;

  size_t remain = ptail - location + 1;
  DEBUGLOG(( "size_t remain = %d\n", remain ));
  strlcpy( result, location, remain < size ? remain : size );
  return result;
}

/* Passed any string value, decode from URL transmission. */
char*
sdecode( char* result, const char* location, size_t size )
{
  const char* digits = "0123456789ABCDEF";
  const char* p;
  const char* ps = location;
  char* pr = result;
  int   i;

  if( size-- ) {
    while( *ps && size )
    {
      if( *ps == '%' ) {
        if( *ps && ( p = strchr( digits, toupper( *++ps ))) != NULL ) {
          i = p - digits;
          if( *ps && ( p = strchr( digits, toupper( *++ps ))) != NULL ) {
            i = i * 16 + p - digits;
            if( size ) {
              *pr = (char)i;
            }
          }
        }
      } else {
        if( size ) {
          *pr = *ps;
        }
      }
      ++ps;
      ++pr;
      --size;
    }
    *pr = 0;
  }

  return result;
}

/* Returns TRUE if the specified location is a root directory. */
BOOL
is_root( const char* location )
{
  return strlen( location ) == 3
         && location[1] == ':'
         && isslash( location[2] );
}

/* Returns TRUE if the specified location is a directory. */
BOOL
is_dir( const char* location )
{
  struct stat fi;
  return ( stat( location, &fi ) == 0 ) && ( fi.st_mode & S_IFDIR );
}

/* Creates a single path name, composed of a base path name and file
 * or directory name.
 */

char*
smakepath( char* result, const char* pathname, const char* name, int size )
{
  int path_size;

  strlcpy( result, pathname, size );
  path_size = strlen( result );

  if( path_size && !isslash( result[ path_size - 1 ] )) {
    strlcat( result, "\\", size );
  }

  strlcat( result, name, size );
  return result;
}

/**
 * Change current default drive and working directory.
 *
 * Causes the current default drive and working directory to change to the
 * directory specified by pathname. The pathname must refer to
 * an existing directory.
 *
 * Returns a value of 0 if the default drive and working directory was
 * successfully changed. A return value of -1 indicates an error.
 */

int
chdrivedir( const char* pathname )
{
  char   drive[3] = "";
  APIRET rc = NO_ERROR;

  if( *sdrive( drive, pathname, sizeof(drive))) {
    rc = DosSetDefaultDisk( toupper( drive[0] ) - ( 'A' - 1 ));
  }
  if( rc == NO_ERROR && *pathname ) {
    rc = DosSetCurrentDir( pathname );
  }
  if( rc == NO_ERROR ) {
    return 0;
  } else {
    return ENOENT;
  }
}

