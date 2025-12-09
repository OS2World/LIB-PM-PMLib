/*
 * Copyright (C) 2010-2015 Dmitry A.Steklenev
 */

#include "pm_selectdir.h"
#include "pm_gui.h"
#include "pm_error.h"
#include "pm_frame.h"
#include "pm_debuglog.h"
#include "pm_memory.h"

#pragma library( "foc.lib" )

/* Constructs the directory selection dialog object.
 *
 * This constructor create file dialog object. You must use show_modally
 * to activate it.
 */

PMSelectDir::PMSelectDir( const char* caption,  HWND howner,
                          const char* pathname, ULONG style )
: m_owner ( howner     ),
  m_handle( NULLHANDLE )
{
  memset( &m_seldir, 0, sizeof( FOCSELDIR ));

  m_seldir.cbSize     = sizeof( FOCSELDIR );
  m_seldir.fl         = style;
  m_seldir.pszTitle   = xstrdup( caption );
  m_seldir.ulUser     = (ULONG)this;
  m_seldir.pfnDlgProc = dlg_proc;

  strlcpy( m_seldir.szFullDir, pathname, sizeof( m_seldir.szFullDir ));
}

/* Constructs the custom directory selection dialog object.
 *
 * This constructor create file dialog object. You must use show_modally
 * to activate it.
 */

PMSelectDir::PMSelectDir( const char* caption,  HWND howner,
                          const char* pathname, HMODULE hmodule, SHORT res_id, ULONG style )
: m_owner ( howner     ),
  m_handle( NULLHANDLE )
{
  memset( &m_seldir, 0, sizeof( FOCSELDIR ));

  m_seldir.cbSize     = sizeof( FOCSELDIR );
  m_seldir.fl         = style | FDS_CUSTOM;
  m_seldir.pszTitle   = xstrdup( caption );
  m_seldir.ulUser     = (ULONG)this;
  m_seldir.pfnDlgProc = dlg_proc;
  m_seldir.hMod       = hmodule;
  m_seldir.usDlgId    = res_id;

  strlcpy( m_seldir.szFullDir, pathname, sizeof( m_seldir.szFullDir ));
}

/* Destructs the directory selection dialog object.
 *
 * Cleans up as this dialog object and destroying the
 * associated presentation window for this object.
 */

PMSelectDir::~PMSelectDir() {
  xfree( m_seldir.pszTitle );
}

/**
 * Displays the file dialog.
 *
 * While the dialog window is being shown modally, its parent or owner or
 * both windows are disabled. Other top level windows belonging to the
 * application are not disabled.
 */

ULONG PMSelectDir::show_modally()
{
  m_handle = NULLHANDLE;

  if( FOCSelectDir( HWND_DESKTOP, m_owner, &m_seldir ) == NULLHANDLE ) {
    PM_THROW_GUIERROR();
  }

  return m_seldir.lReturn;
}

/* Returns the chosen directory.
 */

BOOL PMSelectDir::selected( char* pathname, int size )
{
  strlcpy( pathname, m_seldir.szFullDir, size );
  return TRUE;
}

/* Dispatch system native events.
 *
 * PMSelectDir calls this function when a system native window
 * event occurs.
 */

MRESULT PMSelectDir::dispatch( ULONG msg, MPARAM mp1, MPARAM mp2 ) {
  return dispatch_everything( msg, mp1, mp2 );
}

/* Dispatch all not catched events.
 *
 * A virtual method that provides default implementation for
 * all events that occur on directory selection dialog window.
 */

MRESULT PMSelectDir::dispatch_everything( ULONG msg, MPARAM mp1, MPARAM mp2 ) {
  return FOCDefSelectDirProc( m_handle, msg, mp1, mp2 );
}

/* Events processing.
 */

MRESULT _System PMSelectDir::dlg_proc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
  PFOCSELDIR focseldir = (PFOCSELDIR)WinQueryWindowULong( hwnd, QWL_USER );

  if( focseldir && focseldir->ulUser )
  {
    PMSelectDir* pmselectdir = (PMSelectDir*)focseldir->ulUser;

    pmselectdir->m_handle = hwnd;
    return pmselectdir->dispatch( msg, mp1, mp2 );
  } else {
    return 0;
  }
}

