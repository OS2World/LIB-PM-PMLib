/*
 * Copyright (C) 2001-2015 Dmitry A.Steklenev
 */

#include <memory.h>
#include "pm_filedialog.h"
#include "pm_gui.h"
#include "pm_error.h"
#include "pm_fileutils.h"
#include "pm_frame.h"
#include "pm_debuglog.h"
#include "pm_memory.h"

#pragma library( "foc.lib" )

/* Constructs the file dialog object.
 *
 * This constructor create file dialog object. You must use show_modally
 * to activate it.
 */

PMFileDialog::PMFileDialog( const char* caption,  HWND howner,
                            const char* filename, ULONG style )

: m_owner      (  howner     ),
  m_types      (  NULL       ),
  m_types_count(  0          ),
  m_handle     (  NULLHANDLE ),
  m_next       (  0          ),
  m_initial    (  0          )
{
  memset( &m_filedlg, 0, sizeof( FOCFILEDLG ));

  m_filedlg.cbSize     = sizeof( FOCFILEDLG );
  m_filedlg.fl         = style;
  m_filedlg.pszTitle   = xstrdup( caption );
  m_filedlg.ulUser     = (ULONG)this;
  m_filedlg.pfnDlgProc = dlg_proc;

  strlcpy( m_filedlg.szFullFile, filename, sizeof( m_filedlg.szFullFile ));
}

/* Constructs the custom file dialog object.
 *
 * This constructor create file dialog object. You must use show_modally
 * to activate it.
 */

PMFileDialog::PMFileDialog( const char* caption, HWND howner, const char* filename,
                            HMODULE hmodule, SHORT res_id, ULONG style )

: m_owner      (  howner     ),
  m_types      (  NULL       ),
  m_types_count(  0          ),
  m_handle     (  NULLHANDLE ),
  m_next       (  0          ),
  m_initial    (  0          )
{
  memset( &m_filedlg, 0, sizeof( FOCFILEDLG ));

  m_filedlg.cbSize     = sizeof( FOCFILEDLG );
  m_filedlg.fl         = style | FDS_CUSTOM;
  m_filedlg.pszTitle   = xstrdup( caption );
  m_filedlg.ulUser     = (ULONG)this;
  m_filedlg.pfnDlgProc = dlg_proc;
  m_filedlg.hMod       = hmodule;
  m_filedlg.usDlgId    = res_id;

  strlcpy( m_filedlg.szFullFile, filename, sizeof( m_filedlg.szFullFile ));
}

/* Destructs the file dialog object.
 *
 * Cleans up as this dialog object and destroying the
 * associated presentation window for this object.
 */

PMFileDialog::~PMFileDialog()
{
  int  i;
  for( i = 0; i < m_types_count; i++ ) {
    xfree( m_types[i][0] );
  }

  xfree( m_types );
  xfree( m_filedlg.pszTitle );

  if( m_filedlg.papszFQFilename ) {
    FOCFreeFileDlgList( m_filedlg.papszFQFilename );
  }
}

/**
 * Displays the file dialog.
 *
 * While the dialog window is being shown modally, its parent or owner or
 * both windows are disabled. Other top level windows belonging to the
 * application are not disabled.
 */

ULONG PMFileDialog::show_modally()
{
  char type[254];
  m_handle = NULLHANDLE;

  if( m_types != NULL )
  {
    // WinFileDlg returns error if a length of the pszIType string is above
    // 255 characters. Therefore the small part from the full filter is used
    // as initial extended-attribute type filter. This part has enough to
    // find the full filter in the papszITypeList.
    strlcpy( type, m_types[m_initial][0], sizeof( type ));

    m_filedlg.papszITypeList = m_types;
    m_filedlg.pszIType = type;
  }

  if( m_filedlg.papszFQFilename ) {
    FOCFreeFileDlgList( m_filedlg.papszFQFilename );
    m_filedlg.papszFQFilename = NULL;
    m_filedlg.ulFQFCount = 0;
  }

  if( FOCFileDlg( HWND_DESKTOP, m_owner, &m_filedlg ) == NULLHANDLE ) {
    PM_THROW_GUIERROR();
  }

  return m_filedlg.lReturn;
}

/* Adds the specified type to the drop-down list box.
 */

void PMFileDialog::add_filetype( const char* filetype, const char* filter, BOOL initial )
{
  char* combined =
    (char*)xmalloc( strlen( filetype ) + strlen( filter ) * 2 + 5 );

  sprintf( combined, "%s (%s)\t%s", filetype, filter, filter );
  m_types = (APSZ*)xrealloc( m_types, sizeof( APSZ ) * ( m_types_count + 2 ));

  if( initial ) {
    m_initial = m_types_count;
  }

  m_types[m_types_count    ][0] = combined;
  m_types[m_types_count + 1][0] = NULL;
  m_types_count++;
}

/* Returns a selected file type to assign to the file name returned.
 */

int PMFileDialog::filetype() const {
  return m_filedlg.sEAType;
}

/* Returns a pointer to the iterator object.
 *
 * This object can be used to iterate through chosen files
 * and must be deleted after usage.
 */

PMFileDialog::iterator* PMFileDialog::selected() {
  return new iterator( &m_filedlg );
}

/* Constructs the iterator object.
 */

PMFileDialog::iterator::iterator( PFOCFILEDLG pfiledlg )

: m_count( 0    ),
  m_array( NULL ),
  m_next ( 0    )
{
  int i;

  if( pfiledlg->ulFQFCount ) {
    m_count = pfiledlg->ulFQFCount;
    m_array = (char**)xmalloc( m_count * sizeof(char*));

    if( m_count > 1 ) {
      for( i = 0; i < m_count; i++ ) {
        m_array[i] = xstrdup( (*pfiledlg->papszFQFilename)[i] );
      }
    } else {
      m_array[0] = xstrdup( pfiledlg->szFullFile );
    }
  }
}

/* Destructs the iterator object.
 */

PMFileDialog::iterator::~iterator()
{
  int i;

  for( i = 0; i < m_count; i++ ) {
    xfree( m_array[i] );
  }

  xfree( m_array );
}

/* Returns the next selected file or directory.
 */

BOOL PMFileDialog::iterator::next( char* pathname, int size )
{
  if( m_next < m_count ) {
    strlcpy( pathname, m_array[m_next], size );
    ++m_next;
    return TRUE;
  }

  return FALSE;
}

/* Dispatch system native events.
 *
 * PMFileDialog calls this function when a system native window
 * event occurs.
 */

MRESULT PMFileDialog::dispatch( ULONG msg, MPARAM mp1, MPARAM mp2 ) {
  return dispatch_everything( msg, mp1, mp2 );
}

/* Dispatch all not catched events.
 *
 * A virtual method that provides default implementation for
 * all events that occur on file dialog window.
 */

MRESULT PMFileDialog::dispatch_everything( ULONG msg, MPARAM mp1, MPARAM mp2 ) {
  return FOCDefFileDlgProc( m_handle, msg, mp1, mp2 );
}

/* Events processing.
 */

MRESULT _System PMFileDialog::dlg_proc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
  PFOCFILEDLG focfiledlg = (PFOCFILEDLG)WinQueryWindowULong( hwnd, QWL_USER );

  if( focfiledlg && focfiledlg->ulUser )
  {
    PMFileDialog* pmfiledlg = (PMFileDialog*)focfiledlg->ulUser;

    pmfiledlg->m_handle = hwnd;
    return pmfiledlg->dispatch( msg, mp1, mp2 );
  } else {
    return 0;
  }
}

