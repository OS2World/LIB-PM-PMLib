/*
 * Copyright (C) 2001-2016 Dmitry A.Steklenev
 */

#include "pm_frame.h"
#include "pm_gui.h"

#ifndef WS_TOPMOST
#define WS_TOPMOST  0x00200000
#endif

/* Wraps the frame window object around an existing
 * presentation window handle.
 */

PMFrame::PMFrame( HWND handle )

: PMWindow    ( handle     ),
  m_client    ( WinWindowFromID( handle, FID_CLIENT )),
  m_menu      ( WinWindowFromID( handle, FID_MENU   )),
  m_extensions( NULL       ),
  m_alignments( NULL       ),
  m_icon      ( NULLHANDLE )
{}

/* Constructs the frame window.
 */

PMFrame::PMFrame( SHORT id, const char* text, HWND hparent, HWND howner,
                  LONG x, LONG y, LONG cx, LONG cy, ULONG frame_style, ULONG style )

: PMWindow( WC_FRAME, id, text, hparent, howner,
            x, y, cx, cy, style, ( m_fcdata.cb = sizeof( m_fcdata ),
                                   m_fcdata.flCreateFlags = frame_style,
                                   m_fcdata.hmodResources = (USHORT)PMGUI::module_handle(),
                                   m_fcdata.idResources   = id,
                                  &m_fcdata )),
  m_client    ( NULLHANDLE ),
  m_menu      ( NULLHANDLE ),
  m_extensions( NULL       ),
  m_alignments( NULL       ),
  m_icon      ( NULLHANDLE )
{
  font( SYSFONT_WINDOWTEXT );

  // Change window background.
  WinSetWindowUShort( handle(), QWS_FLAGS, (USHORT)( WinQueryWindowUShort( handle(), QWS_FLAGS ) | FF_DIALOGBOX ));
  invalidate();

  // Needed for correct repainting of a frame controls.
  WinSendMsg( handle(), WM_UPDATEFRAME, MPFROMLONG( m_fcdata.flCreateFlags ), 0 );
}

/* Constructs the frame window from the dialog template.
 */

PMFrame::PMFrame( HWND hparent, HWND howner, SHORT res_id, HMODULE hmodule )

: PMWindow    ( hparent, howner, res_id, hmodule  ),
  m_client    ( WinWindowFromID( handle(), FID_CLIENT )),
  m_menu      ( NULLHANDLE ),
  m_extensions( NULL       ),
  m_alignments( NULL       )
{
  m_icon = WinLoadPointer( HWND_DESKTOP, hmodule, res_id );

  if( m_icon != NULLHANDLE  ) {
    send( WM_SETICON, MPFROMLONG( m_icon ), 0 );
  }

  // To prevent flashing white when you turn the pages of a notebook control.
  color( CCI_PAGEBACKGROUND, SYSCLR_DIALOGBACKGROUND );
}

/* Destructs the frame window object.
 *
 * Cleans up as this window object and destroying the
 * associated presentation window for this object if the object
 * was not wrapped around an existing window handle.
 */

PMFrame::~PMFrame()
{
  if( is_valid()) {
    if( m_icon ) {
      send( WM_SETICON, MPFROMLONG( NULLHANDLE ), 0 );
    }

    send( PM_SETEXTENSION, 0, MPFROM2SHORT( PMA_SE_DETACH_ALL, 0 ));
    send( PM_ALIGN, MPFROMSHORT( PMA_ALIGN_REMOVE_ALL ), 0 );
  }

  if( m_icon ) {
    WinDestroyPointer( m_icon );
  }
}

/* Sets the window used as the frame
 * window's client area.
 */

void PMFrame::client( HWND client )
{
  m_client = client;
  send( PM_FORMATFRAME, 0, 0 );

  if( has_focus()) {
    WinSetFocus( HWND_DESKTOP, m_client );
  }
}

/* Returns the handle of the window used as the
 * frame window's client area.
 */

HWND PMFrame::client() {
  return m_client;
}

/* Sets the menu used as the frame
 * window's menu bar.
 */

void PMFrame::menu( HWND hmenu )
{
  if( !WinSetWindowUShort( hmenu, QWS_ID, FID_MENU )) {
    PM_THROW_GUIERROR();
  }

  WinSendMsg( handle(), WM_UPDATEFRAME, MPFROMLONG( FCF_MENU ), 0 );
  m_menu = hmenu;
}

/* Returns the handle of the menu used as the
 * frame window's menu bar.
 */

HWND PMFrame::menu() {
  return m_menu;
}

/* Calculates a client rectangle.
 *
 * This method calculates a client rectangle from a
 * frame rectangle.
 */

PMRect PMFrame::client_rectangle() const
{
   PMRect rect = rectangle();
   RECTL  rect_client = { 0, 0, rect.cx - 1, rect.cy - 1 };

   WinCalcFrameRect( handle(), &rect_client, TRUE );
   return PMRect( rect_client, PMRect::inclusive );
}

/* Makes the frame window visible.
 *
 * The frame window is made the topmost window also.
 */

void PMFrame::show( BOOL state )
{
  PMWindow::show( state );

  if( state ) {
    if( !WinSetActiveWindow( HWND_DESKTOP, handle())) {
      PM_THROW_GUIERROR();
    }
  }
}

/* Displays the frame window modally.
 *
 * While the frame window is being shown modally, its parent or owner or
 * both windows are disabled. Other top level windows belonging to the
 * application are not disabled.
 */

ULONG PMFrame::show_modally()
{
  // Insure WinProcessDlg does not return immediately.
  WinSetWindowUShort( handle(), QWS_FLAGS, (USHORT)( WinQueryWindowUShort( handle(), QWS_FLAGS ) & ~FF_DLGDISMISSED ));
  WinSetActiveWindow( HWND_DESKTOP, handle());
  return WinProcessDlg( handle());
}

/* Dismiss the frame window.
 *
 * This method hides the modeless frame window, and causes
 * show_modally to return.
 */

void PMFrame::dismiss( ULONG rc )
{
  if( !WinDismissDlg( handle(), rc )) {
    PM_THROW_GUIERROR();
  }
}

/* Attaches a window to frame as a frame extension.
 */

void PMFrame::attach_extension( HWND hwnd, location_type location ) {
  send( PM_SETEXTENSION, MPFROMHWND( hwnd ), MPFROM2SHORT( PMA_SE_ATTACH, location ));
}

/* Detaches a frame extension from frame.
 */

void PMFrame::detach_extension( HWND hwnd ) {
  send( PM_SETEXTENSION, MPFROMHWND( hwnd ), MPFROM2SHORT( PMA_SE_DETACH, 0 ));
}

/* Attaches a window to frame as a frame extension.
 * Must be called only from a window thread.
 */

void PMFrame::win_attach_extension( HWND hwnd, location_type location )
{
  WNDEXTS* ext;
  WNDEXTS* ext_new;

  if( hwnd ) {
    for( ext = m_extensions; ext; ext = ext->next ) {
      if( ext->hwnd == hwnd ) {
        return;
      }
      if( ext->next == NULL ) {
        break;
      }
    }

    ext_new           = new WNDEXTS();
    ext_new->hwnd     = hwnd;
    ext_new->location = location;
    ext_new->next     = NULL;

    if( ext ) {
      ext->next    = ext_new;
    } else {
      m_extensions = ext_new;
    }
    win_format();
  }
}

/* Detaches a frame extension from frame. Must be called
 * only from a window thread.
 */

void PMFrame::win_detach_extension( HWND hwnd )
{
  WNDEXTS* ext;
  WNDEXTS* ext_prev = NULL;

  for( ext = m_extensions; ext; ext = ext->next ) {
    if( ext->hwnd == hwnd ) {
      break;
    } else {
      ext_prev = ext;
    }
  }

  if( ext ) {
    if( ext_prev ) {
      ext_prev->next = ext->next;
    } else {
      m_extensions   = ext->next;
    }
    delete ext;
  }
  win_format();
}

/* Detaches all frame extensions from frame. Must be called
 * only from a window thread.
 */

void PMFrame::win_detach_all_extensions()
{
  if( m_extensions )
  {
    WNDEXTS* ext = m_extensions;
    WNDEXTS* ext_next;

    while( ext ) {
      ext_next = ext->next;
      delete ext;
      ext = ext_next;
    }

    m_extensions = NULL;
    win_format();
  }
}

/* Sets aligment for child window.
 */

void PMFrame::align( HWND hwnd, alignment_type alignment, HWND href )
{
  WNDALGN set;

  if( hwnd )
  {
    set.hwnd = hwnd;
    set.alignment = alignment;
    set.href = href;

    send( PM_ALIGN, MPFROMSHORT( PMA_ALIGN_SET ), MPFROMP( &set ));
  }
}

/* Sets aligment for child window. Must be called
 * only from a window thread.
 */

void PMFrame::win_align( WNDALGN* set )
{
  WNDALGN* align;
  WNDALGN* align_prev = NULL;
  WNDALGN* align_new;
  PMRect   rect;
  PMRect   rect_ref;

  for( align = m_alignments; align && align->next; )
  {
    if( align->hwnd == set->hwnd && align->alignment == set->alignment )
    {
      WNDALGN* remove = align;

      if( align_prev ) {
        align_prev->next = align->next;
        align = align_prev;
      } else {
        m_alignments = align->next;
        align = m_alignments;
      }
      delete remove;
    } else {
      align_prev = align;
      align = align->next;
    }
  }

  align_new = new WNDALGN();

  align_new->hwnd      = set->hwnd;
  align_new->alignment = set->alignment;
  align_new->href      = set->href;
  align_new->next      = NULL;

  rect = rectangle( set->hwnd );
  rect_ref = rectangle( set->href ? set->href : handle());

  if( !set->href ) {
    rect_ref.x = rect_ref.y = 0;
  }

  switch( set->alignment ) {
    case align_top_on_top:       align_new->offset = rect.y + rect.cy - rect_ref.y - rect_ref.cy; break;
    case align_top_on_bottom:    align_new->offset = rect.y + rect.cy - rect_ref.y; break;
    case align_bottom_on_top:    align_new->offset = rect.y - rect_ref.y - rect_ref.cy; break;
    case align_bottom_on_bottom: align_new->offset = rect.y - rect_ref.y; break;
    case align_left_on_left:     align_new->offset = rect.x - rect_ref.x; break;
    case align_right_on_left:    align_new->offset = rect.x + rect.cx - rect_ref.x; break;
    case align_left_on_right:    align_new->offset = rect.x - rect_ref.x - rect_ref.cx; break;
    case align_right_on_right:   align_new->offset = rect.x + rect.cx - rect_ref.x - rect_ref.cx; break;
    case align_left_on_center:   align_new->offset = rect.x - rect_ref.x - rect_ref.cx / 2; break;
    case align_right_on_center:  align_new->offset = rect.x + rect.cx - rect_ref.x - rect_ref.cx / 2; break;
    case scale_width:            align_new->scale  = (float)rect.cx / (float)rect_ref.cx; break;
    case scale_height:           align_new->scale  = (float)rect.cy / (float)rect_ref.cy; break;
  }

  if( align ) {
    align->next  = align_new;
  } else {
    m_alignments = align_new;
  }
}

/* Removes all alignment definitions. Must be called
 * only from a window thread.
 */

void PMFrame::win_align_remove_all()
{
  if( m_alignments )
  {
    WNDALGN* align = m_alignments;
    WNDALGN* align_next;

    while( align ) {
      align_next = align->next;
      delete align;
      align = align_next;
    }

    m_alignments = NULL;
  }
}

/* Formats a frame window.
 * Must be called only from a window thread.
 */

void PMFrame::win_format()
{
  PMRect   cli_rect = client_rectangle();
  WNDEXTS* ext;
  WNDALGN* align;

  for( ext = m_extensions; ext; ext = ext->next )
  {
    PMWindow ext_win( ext->hwnd );
    PMRect   ext_rect = ext_win.rectangle();

    switch( ext->location ) {
      case left:
        ext_rect.x   = cli_rect.x;
        ext_rect.y   = cli_rect.y;
        ext_rect.cy  = cli_rect.cy;
        cli_rect.x  += ext_rect.cx;
        cli_rect.cx -= ext_rect.cx;
        break;

      case right:
        ext_rect.x   = cli_rect.x + cli_rect.cx - ext_rect.cx;
        ext_rect.y   = cli_rect.y;
        ext_rect.cy  = cli_rect.cy;
        cli_rect.cx -= ext_rect.cx;
        break;

      case below:
        ext_rect.x   = cli_rect.x;
        ext_rect.y   = cli_rect.y;
        ext_rect.cx  = cli_rect.cx;
        cli_rect.y  += ext_rect.cy;
        cli_rect.cy -= ext_rect.cy;
        break;

      case above:
        ext_rect.x   = cli_rect.x;
        ext_rect.y   = cli_rect.y + cli_rect.cy - ext_rect.cy;
        ext_rect.cx  = cli_rect.cx;
        cli_rect.cy -= ext_rect.cy;
        break;
    }

    ext_win.rectangle( ext_rect );
  }

  if( m_client != NULLHANDLE && WinIsWindow( PMGUI::hab(), m_client )) {
    PMWindow( m_client ).rectangle( cli_rect );
  }

  for( align = m_alignments; align; align = align->next )
  {
    PMWindow align_win( align->hwnd );
    PMRect   rect = align_win.rectangle();
    PMRect   rect_ref = rectangle( align->href ? align->href : handle());

    if( !align->href ) {
      rect_ref.x = rect_ref.y = 0;
    }

    switch( align->alignment ) {
      case align_top_on_top:       rect.cy = rect_ref.y + rect_ref.cy + align->offset - rect.y; break;
      case align_bottom_on_top:    rect.y  = rect_ref.y + rect_ref.cy + align->offset; break;
      case align_top_on_bottom:    rect.cy = rect_ref.y + align->offset; break;
      case align_bottom_on_bottom: rect.y  = rect_ref.y + align->offset; break;
      case align_left_on_left:     rect.x  = rect_ref.x + align->offset; break;
      case align_right_on_left:    rect.cx = rect_ref.x + align->offset; break;
      case align_left_on_right:    rect.x  = rect_ref.x + rect_ref.cx + align->offset; break;
      case align_right_on_right:   rect.cx = rect_ref.x + rect_ref.cx + align->offset - rect.x; break;
      case align_left_on_center:   rect.x  = rect_ref.x + rect_ref.cx / 2 + align->offset; break;
      case align_right_on_center:  rect.cx = rect_ref.x + rect_ref.cx / 2 + align->offset - rect.x; break;
      case scale_width:            rect.cx = rect_ref.cx * align->scale; break;
      case scale_height:           rect.cy = rect_ref.cy * align->scale; break;
    }

    align_win.rectangle( rect );
  }

  WinUpdateWindow( handle());
}

/* Replaces the accelerator keys used by the window.
 */

void PMFrame::attach_accelerators( SHORT res_id, HMODULE hmodule )
{
  HACCEL hnew = WinLoadAccelTable ( PMGUI::hab(), hmodule, res_id );

  if( hnew == NULLHANDLE ) {
    PM_THROW_GUIERROR();
  }
  if( !send( PM_SETACCELTABLE, MPFROMLONG( hnew ), 0 )) {
    PM_THROW_GUIERROR();
  }
}

/* Sets the window as topmost window.
 */

void PMFrame::set_as_topmost( BOOL topmost )
{
  ULONG style = WinQueryWindowULong( handle(), QWL_STYLE );

  if( topmost ) {
    style |=  WS_TOPMOST;
  } else {
    style &= ~WS_TOPMOST;
  }

  WinSetWindowULong( handle(), QWL_STYLE, style );
}

/* Returns TRUE if the window is a topmost window.
 */

BOOL PMFrame::is_topmost() const {
  return ( WinQueryWindowULong( handle(), QWL_STYLE ) & WS_TOPMOST );
}

/* Returns the pointer handle of the window icon.
 */

HPOINTER PMFrame::icon() const {
  return (HPOINTER)WinSendMsg( handle(), WM_QUERYICON, 0, 0 );
}


/* Sets the application icon using an existing icon handle.
 */

void PMFrame::icon( HPOINTER hptr )
{
  send( WM_SETICON, MPFROMLONG( hptr ), 0 );

  if( m_icon )
  {
    HPOINTER old_hptr = m_icon;
    m_icon = NULLHANDLE;
    WinDestroyPointer( old_hptr );
  }
}

/* Set the icon by specifying a resource identifier.
 */

void PMFrame::icon( SHORT res_id, HMODULE hmodule )
{
  HPOINTER hptr = WinLoadPointer( HWND_DESKTOP, hmodule, res_id );

  if( !hptr ) {
    PM_THROW_GUIERROR();
  }

  icon( hptr );
  m_icon = hptr;
}

/* Adds this frame window's title as a window list entry.
 */

void PMFrame::add_to_window_list() const
{
  HSWITCH hswitch;
  SWCNTRL swctrl;

  if(( hswitch = WinQuerySwitchHandle( handle(), PMGUI::pid())) != NULLHANDLE ) {
    if( WinQuerySwitchEntry( hswitch, &swctrl ) != 0 ) {
      PM_THROW_GUIERROR();
    }
    text( swctrl.szSwtitle, sizeof( swctrl.szSwtitle ));
    swctrl.hwndIcon = icon();
    if( WinChangeSwitchEntry( hswitch, &swctrl ) != 0 ) {
      PM_THROW_GUIERROR();
    }
  } else {
    memset( &swctrl, 0, sizeof(swctrl));

    swctrl.hwnd          = handle();
    swctrl.hwndIcon      = icon();
    swctrl.idProcess     = PMGUI::pid();
    swctrl.uchVisibility = SWL_VISIBLE;
    swctrl.fbJump        = SWL_JUMPABLE;
    swctrl.bProgType     = PROG_PM;
    text( swctrl.szSwtitle, sizeof( swctrl.szSwtitle ));

    if( !WinAddSwitchEntry( &swctrl )) {
      PM_THROW_GUIERROR();
    }
  }
}

/* Removes this frame's entry from the window list.
 */

void PMFrame::remove_from_window_list() const
{
  HSWITCH hswitch;

  if(( hswitch = WinQuerySwitchHandle( handle(), PMGUI::pid())) != NULLHANDLE ) {
    if( WinRemoveSwitchEntry( hswitch ) != 0 ) {
      PM_THROW_GUIERROR();
    }
  }
}

/*
 * Dispatch system native events.
 *
 * PMFrame calls this function when a system native window
 * event occurs.
 */

MRESULT PMFrame::dispatch( ULONG msg, MPARAM mp1, MPARAM mp2 )
{
  switch( msg ) {
    case WM_DESTROY:
      // Extensions must be also detached here because presentation window can
      // be destroyed before object destruction. In other cases the WM_DESTROY
      // message can not be received here.
      win_detach_all_extensions();
      win_align_remove_all();
      break;

    case WM_ERASEBACKGROUND:
      if( m_client != NULLHANDLE ) {
        return 0;
      }
      break;

    case PM_FORMATFRAME:
      if( m_client != NULLHANDLE || m_extensions || m_alignments ) {
        win_format();
      }
      return 0;

    case PM_SETEXTENSION:
    {
      HWND          hwnd     = HWNDFROMMP(mp1);
      location_type location = location_type( SHORT2FROMMP(mp2));
      SHORT         action   = SHORT1FROMMP(mp2);

      switch( action ) {
        case PMA_SE_ATTACH:
          win_attach_extension( hwnd, location );
          break;

        case PMA_SE_DETACH:
          win_detach_extension( hwnd );
          break;

        case PMA_SE_DETACH_ALL:
          win_detach_all_extensions();
          break;
      }

      return 0;
    }

    case PM_ALIGN:
    {
      SHORT     action = SHORT1FROMMP(mp1);
      WNDALGN*  set    = (WNDALGN*)mp2;

      switch( action ) {
        case PMA_ALIGN_SET:
          win_align( set );
          break;

        case PMA_ALIGN_REMOVE_ALL:
          win_align_remove_all();
          break;
      }

      return 0;
    }

    case WM_FORMATFRAME:
    {
      USHORT i, count = (USHORT)PMWindow::dispatch( msg, mp1, mp2 );
      PSWP pswp = (PSWP)mp1;

      // OS/2 requires formatting the frame window after
      // processing WM_FORMATFRAME.

      for( i = 0; i < count; i++ ) {
        WinSetWindowPos( pswp[i].hwnd,
                         pswp[i].hwndInsertBehind,
                         pswp[i].x,
                         pswp[i].y,
                         pswp[i].cx,
                         pswp[i].cy,
                         pswp[i].fl );
      }

      send( PM_FORMATFRAME, 0, 0 );
      return 0;
    }

    case PM_SETFOCUS:
    {
      if( m_client != NULLHANDLE ) {
        WinSetFocus( HWND_DESKTOP, m_client );
        return 0;
      }
      break;
    }
  }

  return PMWindow::dispatch( msg, mp1, mp2 );
}


