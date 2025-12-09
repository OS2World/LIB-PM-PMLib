/*
 * Copyright (C) 2010-2015 Dmitry A.Steklenev
 */

#include <stdlib.h>
#include <direct.h>
#include "pm_fileview.h"
#include "pm_debuglog.h"
#include "pm_memory.h"

/* Wraps the file view control window object around
 * an existing presentation window handle.
 */

PMFileView::PMFileView( HWND handle )
: PMWindow( handle )
{}

/* Wraps the file view control window object around
 * an existing child window with the specified identifier.
 */

PMFileView::PMFileView( HWND hparent, SHORT id )
: PMWindow( hparent, id )
{}

/* Constructs the file view control window.
 */

PMFileView::PMFileView( SHORT id, HWND hparent,
                        LONG x, LONG y, LONG cx, LONG cy, ULONG style )

: PMWindow( WC_FILEVIEW, id, "", hparent, hparent,
            x, y, cx, cy, style, NULL )
{}

/* Assigns co-operating directory tree container.
 */

HWND PMFileView::attach_dirtree( HWND hdirtree ) {
  return (HWND)send( FVM_SETDIRTREE, MPFROMHWND( hdirtree ), 0 );
}

/* Returns the name of the current selected directory.
 */

char* PMFileView::curdir( char* result, int size ) const
{
  WinSendMsg( handle(), FVM_QUERYCURDIR, MPFROMLONG( size ),  MPFROMP( result ));
  return result;
}

/* Selects the specified directory.
 */

void PMFileView::chdir( const char* directory )
{
  char pathname[CCHMAXPATH];
  char curdir[CCHMAXPATH];

  if( strchr( directory, '\\' )) {
    strlcpy( pathname, directory, sizeof( pathname ));
  } else {
    getcwd( curdir, sizeof( curdir ));
    FOCPathRelToAbs( curdir, directory, pathname, sizeof( pathname ));
  }

  DEBUGLOG2(( "PMFileView: chdir to %s\n", pathname ));

  if( !WinSendMsg( handle(), FVM_CHDIR, MPFROMP( pathname ), 0 )) {
    PM_THROW_GUIERROR();
  }
}

/* Returns position of the split bar relative to the container
 * used in the details view.
 */

LONG PMFileView::splitbar() const {
  return (LONG)WinSendMsg( handle(), FVM_QUERYSPLITBAR, 0, 0 );
}

/* Sets the position of the split bar relative to the container
 * used in the details view.
 */

void PMFileView::splitbar( LONG x )
{
  if( !send( FVM_SETSPLITBAR, MPFROMLONG( x ), 0 )) {
    PM_THROW_GUIERROR();
  }
}

/* Sorts the file view items.
 */

void PMFileView::sort( ULONG order )
{
  if( !send( FVM_SORT, MPFROMLONG( order ), 0 )) {
    PM_THROW_GUIERROR();
  }
}

/* Refreshes a current selected directory.
 */

void PMFileView::refresh()
{
  if( !send( FVM_REFRESH, 0, 0 )) {
    PM_THROW_GUIERROR();
  }
}

/* Returns a pointer to the iterator object.
 *
 * This object can be used to iterate through selected files or
 * directories and must be deleted after usage.
 */

PMFileView::iterator* PMFileView::selected()
{
  iterator* i;
  FILELIST  filelist = { sizeof( FILELIST )};

  WinSendMsg( handle(), FVM_QUERYSELECTED, MPFROMP( &filelist ), 0 );
  i = new iterator( &filelist );
  WinSendMsg( handle(), FVM_FREEFILELIST,  MPFROMP( &filelist ), 0 );
  return i;
}

/* Returns a pointer to the iterator object.
 *
 * This object can be used to iterate through selected files or
 * directories and must be deleted after usage.
 */

PMFileView::iterator* PMFileView::checked()
{
  iterator* i;
  FILELIST  filelist = { sizeof( FILELIST )};

  WinSendMsg( handle(), FVM_QUERYCHECKED, MPFROMP( &filelist ), 0 );
  i = new iterator( &filelist );
  WinSendMsg( handle(), FVM_FREEFILELIST, MPFROMP( &filelist ), 0 );
  return i;
}

/* Constructs the iterator object.
 */

PMFileView::iterator::iterator( FILELIST* pfilelist )

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

PMFileView::iterator::~iterator()
{
  int  i;
  for( i = 0; i < m_count; i++ ) {
    xfree( m_array[i] );
  }

  xfree( m_array );
}

/* Returns the next selected file or directory.
 */

BOOL PMFileView::iterator::next( char* pathname, int size )
{
  if( m_next < m_count ) {
    strlcpy( pathname, m_array[ m_next++ ], size );
    return TRUE;
  }

  return FALSE;
}


