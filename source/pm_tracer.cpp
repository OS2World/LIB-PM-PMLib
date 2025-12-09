/*
 * Copyright (C) 2010-2017 Dmitry A.Steklenev
 */

#include "pm_gui.h"
#include "pm_tracer.h"
#include <ptrace.h>

/* Wraps the log messages collector window object around
 * an existing presentation window handle.
 */

PMTracer::PMTracer( HWND handle )
: PMWindow( handle )
{}

/* Wraps the log messages collector window object around
 * an existing child window with the specified identifier.
 */

PMTracer::PMTracer( HWND hparent, SHORT id )
: PMWindow( hparent, id )
{}

/* Constructs the log messages collector window.
 */

PMTracer::PMTracer( SHORT id, HWND hparent,
                    LONG x, LONG y, LONG cx, LONG cy, ULONG style )

: PMWindow( WC_PTRACER, id, "", hparent, hparent,
            x, y, cx, cy, style, NULL )
{
  font( SYSFONT_PREFORMATTED );
}

/* Adds specified message to the log collector.
 */

void PMTracer::trace( const char* message, int options, int bs )
{
  if( pid() != PMGUI::pid())
  {
    PCHAR  sharedstr;
    APIRET rc;

    if(( rc = DosAllocSharedMem((VOID**)&sharedstr, NULL, strlen( message ) + 1,
                                PAG_WRITE | OBJ_GIVEABLE | PAG_COMMIT )) != NO_ERROR )
    {
      PM_THROW_DOSERROR( rc );
    }

    strcpy( sharedstr, message );

    if(( rc = DosGiveSharedMem( sharedstr, pid(), PAG_READ )) != NO_ERROR ) {
      PM_THROW_DOSERROR( rc );
    }

    send( PTM_TRACE, MPFROMP( sharedstr ), MPFROM2SHORT( options, bs ));
    DosFreeMem( sharedstr );
  } else {
    send( PTM_TRACE, MPFROMP( message ), MPFROM2SHORT( options, bs ));
  }
}

/* Deletes all the messages in the log collector..
 */

void PMTracer::clear() {
  send( PTM_CLEAR, 0, 0 );
}

/* Sets the selection state of all messages in a log collector.
 */

void PMTracer::select_all() {
  send( PTM_SELECTALL, 0, 0 );
}

/* Copies all selected messages into the clipboard.
 */

void PMTracer::copy() {
  send( PTM_COPY, 0, 0 );
}
