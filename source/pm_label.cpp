/*
 * Copyright (C) 2008 Dmitry A.Steklenev
 */

#include "pm_label.h"

/* Wraps the static text control window object around
 * an existing presentation window handle.
 */

PMLabel::PMLabel( HWND handle )
: PMWindow( handle )
{}

/* Wraps the static text control window object around
 * an existing child window with the specified identity.
 */

PMLabel::PMLabel( HWND hparent, SHORT id )
: PMWindow( hparent, id )
{}

/* Constructs the static text control window.
 */

PMLabel::PMLabel( SHORT id, const char* text, HWND hparent,
                  LONG x, LONG y, LONG cx, LONG cy, ULONG style )

: PMWindow( WC_STATIC, id, text, hparent, hparent,
            x, y, cx, cy, SS_TEXT | style, NULL )
{}

