/*
 * Copyright (C) 2001-2016 Dmitry A.Steklenev
 */

#include "pm_notebook.h"
#include "pm_gui.h"
#include "pm_frame.h"
#include "pm_language.h"
#include "pm_debuglog.h"

/* Wraps the notebook control window object around an existing
 * presentation window handle.
 */

PMNoteBook::PMNoteBook( HWND handle )
: PMWindow( handle )
{
  handle_events();
}

/* Wraps the notebook control window object around an existing
 * child window with the specified identity.
 */

PMNoteBook::PMNoteBook( HWND hparent, SHORT id )
: PMWindow( hparent, id )
{
  handle_events();
}

/* Constructs the notebook control window.
 */

PMNoteBook::PMNoteBook( SHORT id, HWND hparent,
                        LONG x, LONG y, LONG cx, LONG cy, ULONG style )

: PMWindow( WC_NOTEBOOK, id, "", hparent, hparent,
            x, y, cx, cy, style, NULL )
{
  // To prevent flashing white when you turn the pages of a notebook control.
  color( CCI_PAGEBACKGROUND, SYSCLR_DIALOGBACKGROUND );
}

/* Inserts the specified page into the notebook data list.
 */

void PMNoteBook::insert( HWND hpage, const char* caption, SHORT order )
{
  MRESULT id;
  RECTL   rectl;

  if( !WinQueryWindowRect( handle(), &rectl )) {
    PM_THROW_GUIERROR();
  }
  if( WinSendMsg( handle(), BKM_CALCPAGERECT, MPFROMP(&rectl), MPFROMLONG(TRUE))) {
    WinSetWindowPos( hpage, NULLHANDLE, 0, 0,
                     rectl.xRight - rectl.xLeft + 1, rectl.yTop - rectl.yBottom + 1, SWP_SIZE );
  }

  id = WinSendMsg( handle(), BKM_INSERTPAGE, 0,
                   MPFROM2SHORT( BKA_AUTOPAGESIZE | BKA_MAJOR | BKA_STATUSTEXTON, order ));

  if( id == NULL ) {
    PM_THROW_GUIERROR();
  }

  if( !WinSendMsg( handle(), BKM_SETPAGEWINDOWHWND, MPFROMLONG( id ), MPFROMLONG( hpage ))) {
    PM_THROW_GUIERROR();
  }

  if( !WinSendMsg( handle(), BKM_SETTABTEXT, MPFROMLONG( id ), MPFROMP( caption ))) {
    PM_THROW_GUIERROR();
  }

  WinSetOwner( hpage, handle());
}

/* Brings the specified page to the top of the notebook.
 */

void PMNoteBook::turn_to( int pos )
{
  ULONG id = (ULONG)send( BKM_QUERYPAGEID, 0, MPFROM2SHORT( BKA_FIRST, 0 ));

  while( id && id != BOOKERR_INVALID_PARAMETERS ) {
    if( pos-- == 0 ) {
      send( BKM_TURNTOPAGE, MPFROMLONG( id ), 0 );
      return;
    }

    id = (ULONG)send( BKM_QUERYPAGEID, MPFROMLONG( id ), MPFROM2SHORT( BKA_NEXT, 0 ));
  }
}

/* Brings the next page to the top of the notebook.
 */

void PMNoteBook::turn_to_next()
{
  ULONG id = (ULONG)send( BKM_QUERYPAGEID, 0, MPFROM2SHORT( BKA_TOP, 0 ));

  if( id && id != BOOKERR_INVALID_PARAMETERS ) {
    ULONG next_id = (ULONG)send( BKM_QUERYPAGEID, MPFROMLONG(id), MPFROM2SHORT( BKA_NEXT, 0 ));
    if( next_id && next_id != BOOKERR_INVALID_PARAMETERS ) {
      send( BKM_TURNTOPAGE, MPFROMLONG( next_id ), 0 );
    }
  }
}

/* Brings the previous page to the top of the notebook.
 */

void PMNoteBook::turn_to_prev()
{
  ULONG id = (ULONG)send( BKM_QUERYPAGEID, 0, MPFROM2SHORT( BKA_TOP, 0 ));

  if( id && id != BOOKERR_INVALID_PARAMETERS ) {
    ULONG prev_id = (ULONG)send( BKM_QUERYPAGEID, MPFROMLONG(id), MPFROM2SHORT( BKA_PREV, 0 ));
    if( prev_id && prev_id != BOOKERR_INVALID_PARAMETERS ) {
      send( BKM_TURNTOPAGE, MPFROMLONG( prev_id ), 0 );
    }
  }
}

/* Translates the notebook window text and all the pages text
 * using for this purpose the current instance of the messages
 * management object.
 */

void PMNoteBook::translate()
{
  ULONG id = (ULONG)send( BKM_QUERYPAGEID, 0, MPFROM2SHORT( BKA_FIRST, 0 ));

  while( id != BOOKERR_INVALID_PARAMETERS && id != 0 )
  {
    char     old_text[1024];
    char     new_text[1024];
    USHORT   text_len;
    BOOKTEXT book_text;
    HWND     hwnd = (HWND)send( BKM_QUERYPAGEWINDOWHWND, MPFROMLONG( id ), 0 );

    if( hwnd != (HWND)BOOKERR_INVALID_PARAMETERS && hwnd != NULLHANDLE ) {
      PMFrame( hwnd ).translate();
    }

    book_text.pString = old_text;
    book_text.textLen = sizeof( old_text );
    text_len = (USHORT)send( BKM_QUERYTABTEXT, MPFROMLONG( id ), MPFROMP( &book_text ));

    if( text_len != (USHORT)BOOKERR_INVALID_PARAMETERS &&
        text_len != 0 )
    {
      MSGCPY( new_text, old_text );
      send( BKM_SETTABTEXT, MPFROMLONG( id ), MPFROMP( new_text ));
    }

    book_text.pString = old_text;
    book_text.textLen = sizeof( old_text );
    text_len = (USHORT)send( BKM_QUERYSTATUSLINETEXT, MPFROMLONG( id ), MPFROMP( &book_text ));

    if( text_len != (USHORT)BOOKERR_INVALID_PARAMETERS &&
        text_len != 0 )
    {
      PMLanguage::instance->message( new_text, sizeof( new_text ), old_text );
      send( BKM_SETSTATUSLINETEXT, MPFROMLONG( id ), MPFROMP( new_text ));
    }

    id = (ULONG)send( BKM_QUERYPAGEID, MPFROMLONG( id ), MPFROM2SHORT( BKA_NEXT, 0 ));
  }
}

