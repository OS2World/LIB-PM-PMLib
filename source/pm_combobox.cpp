/*
 * Copyright (C) 2001-2008 Dmitry A.Steklenev
 */

#include "pm_combobox.h"

/* Wraps the combination box control window object around
 * an existing presentation window handle.
 */

PMComboBox::PMComboBox( HWND handle )
: PMListBox( handle )
{}

/* Wraps the combination box control window object around
 * an existing child window with the specified identifier.
 */

PMComboBox::PMComboBox( HWND hparent, SHORT id )
: PMListBox( hparent, id )
{}

/* Constructs the combination box control window.
 */

PMComboBox::PMComboBox( SHORT id, HWND hparent,
                        LONG x, LONG y, LONG cx, LONG cy, ULONG style )

: PMListBox( WC_COMBOBOX, id, "", hparent, hparent,
             x, y, cx, cy, style, NULL )
{}

/* Removes all items from the combination box control.
 */

void PMComboBox::clear()
{
  if( !WinSendMsg( handle(), LM_DELETEALL, 0, 0 ) ||
      !WinSendMsg( handle(), EM_SETSEL, MPFROM2SHORT( 0, 65535 ), 0 ) ||
      !WinSendMsg( handle(), EM_CLEAR, 0, 0 ))
  {
    PM_THROW_GUIERROR();
  }
}

