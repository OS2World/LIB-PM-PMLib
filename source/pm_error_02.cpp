/*
 * Copyright (C) 2001-2016 Dmitry A.Steklenev
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Methods of the PMError class are divided between two modules so that
// non-PM applications can safe use of objects of this class.

#include "pm_gui.h"
#include "pm_error.h"
#include "pm_errorinfo.h"
#include <progress.h>

extern const char* application_name;
#pragma library( "progress.lib" )

/* Constructs an error object describing a
 * current GUI (PM) error.
 */

PMError::PMError( const char* file, const char* func, int line )

: m_line( line ),
  m_tid ( PMGUI::tid())
{
  int i;

  strlcpy( m_file,  file,  sizeof( m_file  ));
  strlcpy( m_func,  func,  sizeof( m_func  ));
  strlcpy( m_group, "GUI", sizeof( m_group ));

  m_code = WinGetLastError( PMGUI::hab());

  for( i = 0; i < sizeof( errorinfo ) / sizeof( errorinfo[0] ) - 1; i++ ) {
    if( errorinfo[i].id == ERRORIDERROR( m_code )) {
      break;
    }
  }

  strlcpy( m_what, errorinfo[i].info, sizeof( m_what ));
}

/* Displays the error information using a message box window.
 */

const PMError& PMError::show() const
{
  const char* msg_format = "Unexpected error at:\r\n"
                           "   file: %s\r\n"
                           "   function: %s\r\n"
                           "   line: %u\r\n\r\n"
                           "Error group is: %s\r\n"
                           "Thread Id is: %d (%x)\r\n"
                           "Error Id is: %u (%x)\r\n"
                           "Exception text is:\r\n\r\n"
                           "%s\r\n";
  char msg_buffer[1024];

  snprintf( msg_buffer, sizeof( msg_buffer ), msg_format,
            m_file, m_func, m_line, m_group, m_tid, m_tid, m_code, m_code, m_what );

  ExMessageBox ( HWND_DESKTOP, HWND_DESKTOP, "Critical Error", (PSZ)application_name, 100,
                 MB_OK | MB_ERROR | MB_COMMENT | MB_APPLMODAL | MB_PREFORMAT | MB_ANIMATED, msg_buffer );
  return *this;
}

