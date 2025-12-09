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

extern const char* application_name;

/* Constructs an error object from the specified error
 * id and information.
 */

PMError::PMError( int code, const char* group, const char* what,
                            const char* file,  const char* func, int line )
: m_line( line ),
  m_code( code ),
  m_tid ( PMGUI::tid())
{
  strlcpy( m_file,  file,  sizeof( m_file  ));
  strlcpy( m_func,  func,  sizeof( m_func  ));
  strlcpy( m_group, group, sizeof( m_group ));

  if( !what ) {
    sys_info( code, m_what, sizeof( m_what ));
  } else {
    strlcpy( m_what, what, sizeof( m_what ));
  }
}

/* Displays the error information.
 */

const PMError& PMError::show( HFILE hout ) const
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
  char  msg_buffer[1024];
  ULONG done;

  snprintf( msg_buffer, sizeof( msg_buffer ), msg_format,
            m_file, m_func, m_line, m_group, m_tid, m_tid, m_code, m_code, m_what );

  DosWrite( hout, msg_buffer, strlen( msg_buffer ), &done );
  return *this;
}

/* Returns detailed error description of the standard system (DOS) error.
 */

char* PMError::sys_info( int code, char* buffer, int size )
{
  ULONG  ulMessageLength;
  APIRET rc;

  rc = DosGetMessage( 0, 0, (PCHAR)buffer, size,
                      code, (PSZ)"OSO001.MSG", &ulMessageLength );
  if( rc == NO_ERROR ) {
    if( ulMessageLength > 2 && buffer[ulMessageLength-2] == '\r' ) {
      buffer[ulMessageLength-2] = 0;
    } else {
      buffer[ulMessageLength] = 0;
    }
  } else {
    strlcpy( buffer, "No error description is available.", size );
  }

  return buffer;
}

