/*
 * Copyright (C) 2010-2022 Dmitry A.Steklenev
 */

#include "pm_dirtree.h"
#include "pm_gui.h"
#include "pm_fileutils.h"
#include "pm_debuglog.h"
#include "pm_filelist.h"
#include "pm_memory.h"
#include <direct.h>

/* Wraps the directory tree control window object around
 * an existing presentation window handle.
 */

PMDirTree::PMDirTree( HWND handle )
: PMWindow( handle )
{}

/* Wraps the directory tree control window object around
 * an existing child window with the specified identifier.
 */

PMDirTree::PMDirTree( HWND hparent, SHORT id )
: PMWindow( hparent, id )
{}

/* Constructs the directory tree control window.
 */

PMDirTree::PMDirTree( SHORT id, HWND hparent,
                      LONG x, LONG y, LONG cx, LONG cy, ULONG style )

: PMWindow( WC_DIRTREE, id, "", hparent, hparent,
            x, y, cx, cy, style, NULL )
{}

/* Assigns co-operating file view container.
 */

HWND PMDirTree::attach_fileview( HWND hfileview ) {
  return (HWND)send( DTM_SETFILEVIEW, MPFROMHWND( hfileview ), 0 );
}

/* Returns the name of the selected directory.
 */

char* PMDirTree::curdir( char* result, int size ) const
{
  WinSendMsg( handle(), DTM_QUERYCURDIR, MPFROMLONG( size ), MPFROMP( result ));
  return result;
}

/* Selects the specified directory.
 */

void PMDirTree::chdir( const char* directory )
{
  char pathname[CCHMAXPATH];
  char curdir[CCHMAXPATH];

  if( strchr( directory, '\\' )) {
    strlcpy( pathname, directory, sizeof( pathname ));
  } else {
    getcwd( curdir, sizeof( curdir ));
    FOCPathRelToAbs( curdir, directory, pathname, sizeof( pathname ));
  }

  DEBUGLOG2(( "PMDirTree: chdir to %s\n", pathname ));

  if( !WinSendMsg( handle(), DTM_CHDIR, MPFROMP( pathname ), 0 )) {
    PM_THROW_GUIERROR();
  }
}

/* Refreshes a specified part of the directory tree.
 */

void PMDirTree::refresh( const char* directory )
{
  if( !WinSendMsg( handle(), DTM_REFRESH, MPFROMP( directory ), 0 )) {
    PM_THROW_GUIERROR();
  }
}

/* Returns a pointer to the iterator object.
 *
 * This object can be used to iterate through selected files or
 * directories and must be deleted after usage.
 */

PMDirTree::iterator* PMDirTree::checked() const
{
  iterator* i;
  FILELIST  filelist = { sizeof( FILELIST )};

  WinSendMsg( handle(), DTM_QUERYCHECKED, MPFROMP( &filelist ), 0 );
  i = new iterator( &filelist );
  WinSendMsg( handle(), DTM_FREEFILELIST, MPFROMP( &filelist ), 0 );
  return i;
}

/* Checks specified files or directories.
 */

void PMDirTree::check( const char* filespec, ULONG flags )
{
  char pathname[CCHMAXPATH];
  char curdir[CCHMAXPATH];
  PMFileList flist;

  if( !strchr( filespec, '*' ) && !strchr( filespec, '?' )) {
    if( strchr( filespec, '\\' )) {
      DEBUGLOG2(( "PMDirTree: add to check list: %s\n", filespec ));
      flist.add( filespec, "+" );
    } else {
      getcwd( curdir, sizeof( curdir ));
      FOCPathRelToAbs( curdir, filespec, pathname, sizeof( pathname ));
      DEBUGLOG2(( "PMDirTree: add to check list: %s\n", pathname ));
      flist.add( pathname, "+" );
    }
  }
  else
  {
    HDIR          hdir = HDIR_CREATE;
    ULONG         findCount = 1;
    FILEFINDBUF3L buf;
    APIRET        rc;

    sdrivedir( curdir, filespec, sizeof(curdir));

    if( !*curdir ) {
      getcwd( curdir, sizeof( curdir ));
    }

    rc = DosFindFirst( filespec, &hdir, FILE_DIRECTORY | FILE_SYSTEM | FILE_HIDDEN |
                       FILE_ARCHIVED | FILE_READONLY, &buf, sizeof(buf), &findCount,
                       FIL_STANDARDL );

    while( rc == NO_ERROR ) {
      if( strcmp( buf.achName, "."  ) != 0 && strcmp( buf.achName, ".." ) != 0 ) {
        smakepath( pathname, curdir, buf.achName, sizeof( pathname ));
        flist.add( pathname, "+" );
        DEBUGLOG2(( "PMDirTree: add to check list: %s\n", pathname ));
      }
      findCount = 1;
      rc = DosFindNext( hdir, &buf, sizeof(buf), &findCount );
    }
  }
  if( flist.ulFQFCount ) {
    WinSendMsg( handle(), DTM_PLACECHECKS, MPFROMP( &flist ), MPFROMLONG( flags ));
  }
}

/* Constructs the iterator object.
 */

PMDirTree::iterator::iterator( FILELIST* pfilelist )

: m_count( 0    ),
  m_array( NULL ),
  m_next ( 0    )
{
  int i;

  if( pfilelist->ulFQFCount ) {
    m_count = pfilelist->ulFQFCount;
    m_array = (char**)xmalloc( m_count * sizeof(char*));

    for( i = 0; i < m_count; i++ ) {
      m_array[i] = xstrdup((*pfilelist->papszFQFilename)[i] );
    }
  }
}

/* Destructs the iterator object.
 */

PMDirTree::iterator::~iterator()
{
  int  i;
  for( i = 0; i < m_count; i++ ) {
    xfree( m_array[i] );
  }

  xfree( m_array );
}

/* Returns the next selected file or directory.
 */

BOOL PMDirTree::iterator::next( char* pathname, int size )
{
  if( m_next < m_count ) {
    strlcpy( pathname, m_array[ m_next++ ], size );
    return TRUE;
  }

  return FALSE;
}


