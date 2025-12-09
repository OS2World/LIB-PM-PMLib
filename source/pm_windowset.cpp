/*
 * Copyright (C) 2010-2016 Dmitry A.Steklenev
 */

#include "pm_windowset.h"
#include "pm_debuglog.h"
#include "pm_window.h"

HMTX             PMWindowSet::m_mutex;
PMWindowSet::map PMWindowSet::m_map[PM_MAX_WINDOWS];

/* Sets a window user pointer value to a new window object.
 */

PMWindow* PMWindowSet::setptr( HWND hwnd, class PMWindow* new_window )
{
  PMWindow* old_window = NULL;
  BOOL rc;

  DosRequestMutexSem( m_mutex, SEM_INDEFINITE_WAIT );
  old_window = (PMWindow*)WinQueryWindowPtr( hwnd, QWP_USER );
  rc = WinSetWindowPtr( hwnd, QWP_USER, new_window );
  DosReleaseMutexSem( m_mutex );

  if( !rc ) {
    PM_THROW_GUIERROR();
  }

  return old_window;
}

/* Inserts a new window object to the windows set.
 */

PMWindow* PMWindowSet::insert( HWND hwnd, PMWindow* new_window )
{
  PMWindow* old_window = NULL;
  int i, pos;

  DosRequestMutexSem( m_mutex, SEM_INDEFINITE_WAIT );

  for( i = 0, pos = -1; i < sizeof( m_map ) / sizeof( m_map[0] ); i++ ) {
    if( m_map[i].handle == hwnd ) {
      pos = i;
      break;
    }
    if( m_map[i].handle == NULLHANDLE && pos < 0 ) {
      pos = i;
    }
  }

  if( pos >= 0 ) {
    DEBUGLOG2(( "PMWindowSet: insert pointer %08X handle %08X\n", new_window, hwnd ));
    old_window = m_map[pos].window;
    m_map[pos].handle = hwnd;
    m_map[pos].window = new_window;
  }

  DosReleaseMutexSem( m_mutex );

  if( pos < 0 ) {
    PM_THROW_ERROR( PM_ERR_TOO_MANY_WINDOWS, "PMLIB", "Too many strangers." );
  }

  return old_window;
}

/* Removes a window object from the windows set.
 */

void PMWindowSet::remove( HWND hwnd )
{
  int i;

  DosRequestMutexSem( m_mutex, SEM_INDEFINITE_WAIT );

  for( i = 0; i < sizeof( m_map ) / sizeof( m_map[0] ); i++ ) {
    if( m_map[i].handle == hwnd ) {
        DEBUGLOG2(( "PMWindowSet: remove pointer %08X handle %08X\n", m_map[i].window, m_map[i].handle ));
        m_map[i].handle =  NULLHANDLE;
        m_map[i].window =  NULL;
        break;
    }
  }

  DosReleaseMutexSem( m_mutex );
}

/* Queries a window object.
 */

PMWindow* PMWindowSet::query( HWND hwnd )
{
  PMWindow* window = NULL;
  int i;

  DosRequestMutexSem( m_mutex, SEM_INDEFINITE_WAIT );

  for( i = 0; i < sizeof( m_map ) / sizeof( m_map[0] ); i++ ) {
    if( m_map[i].handle == hwnd ) {
      window = m_map[i].window;
      break;
    }
  }

  DosReleaseMutexSem( m_mutex );
  return window;
}

