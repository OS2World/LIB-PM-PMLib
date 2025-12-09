/*
 * Copyright (C) 2001-2016 Dmitry A.Steklenev
 */

#include <stdlib.h>

// Methods of the PMGUI class are divided between two modules so that
// non-PM applications can safe use of objects of this class.

#include "pm_gui.h"
#include "pm_error.h"

/* Constructs the GUI object and initializes the
 * GUI facilities for use by an application.
 */

PMGUI::PMGUI()
{
  TID cur_tid = tid();

  if( cur_tid > PM_MAX_THREADS ) {
    PM_THROW_ERROR( PM_ERR_TOO_MANY_THREADS, "PMLIB", "Too many threads" );
  }

  m_hab[cur_tid] = NULLHANDLE;
  m_hmq[cur_tid] = NULLHANDLE;

  if(( m_hab[cur_tid] = WinInitialize(0)) == NULLHANDLE ) {
    PM_THROW_ERROR( PM_ERR_INITIALIZE, "PMLIB", "Unexpected error in WinInitialize()" );
  }
  if(( m_hmq[cur_tid] = WinCreateMsgQueue( m_hab[cur_tid], 0 )) == NULLHANDLE ) {
    PM_THROW_GUIERROR();
  }

  m_wrapped = FALSE;
}

/* Constructs the GUI object from an existing GUI environment.
 */

PMGUI::PMGUI( HAB hab )
{
  TID cur_tid = tid();

  if( cur_tid > PM_MAX_THREADS ) {
    PM_THROW_ERROR( PM_ERR_TOO_MANY_THREADS, "PMLIB", "Too many threads" );
  }

  m_hab[cur_tid] = hab;
  m_hmq[cur_tid] = HMQ_CURRENT;
  m_wrapped      = TRUE;
}

/* Destroys the GUI object.
 *
 * Releases all of associated GUI resources if the object
 * was not created from an existing environment.
 */

PMGUI::~PMGUI()
{
  TID cur_tid = tid();

  if( !m_wrapped ) {
    if( m_hmq[cur_tid] ) {
      WinDestroyMsgQueue( m_hmq[cur_tid] );
    }
    if( m_hab[cur_tid] ) {
      WinTerminate( m_hab[cur_tid] );
    }

    m_hab[cur_tid] = NULLHANDLE;
    m_hmq[cur_tid] = NULLHANDLE;
  }
}

/* Returns a system metric.
 *
 * Allows the application to ask for details about
 * the user interface element sizes.
 */

LONG PMGUI::metric( LONG id ) {
  return WinQuerySysValue( HWND_DESKTOP, id );
}

/* Returns a current screen resolution.
 */

static LONG PMGUI::dpi()
{
  HPS  hps;
  HDC  hdc;
  LONG dpi = 96;

  hps = WinGetPS( HWND_DESKTOP );
  hdc = GpiQueryDevice( hps );
  DevQueryCaps( hdc, CAPS_HORIZONTAL_FONT_RES, 1L, &dpi );
  WinReleasePS( hps );

  return dpi;
}

/* Dispatches messages.
 */

void PMGUI::run()
{
  QMSG qms;
  HAB  cur_hab = hab();

  while( WinGetMsg( cur_hab, &qms, 0, 0, 0 )) {
    WinDispatchMsg( cur_hab, &qms );
  }
}

