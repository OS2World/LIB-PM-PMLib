/*
 * Copyright (C) 2015 Dmitry A.Steklenev
 */

#include "pm_slider.h"
#include "pm_debuglog.h"

PFNWP PMSlider::m_default_handler = NULL;

/* Wraps the slider control window object around
 * an existing presentation window handle.
 */

PMSlider::PMSlider( HWND handle )
: PMWindow( handle )
{}

/* Wraps the slider control window object around
 * an existing child window with the specified identity.
 */

PMSlider::PMSlider( HWND hparent, SHORT id )
: PMWindow( hparent, id )
{}

/* Constructs the slider control window.
 */

PMSlider::PMSlider( SHORT id, HWND hparent,
                    LONG x, LONG y, LONG cx, LONG cy, USHORT scale1, USHORT scale2, ULONG style )

: PMWindow( PM_SLIDER, id, "", hparent, hparent,
            x, y, cx, cy, style,
          ( m_cdata.cbSize = sizeof( m_cdata ),
            m_cdata.usScale1Increments = scale1,
            m_cdata.usScale1Spacing = 0,
            m_cdata.usScale2Increments = scale2,
            m_cdata.usScale2Spacing = 0,
           &m_cdata ))
{}

/* Sets the size of a tick mark for the primary scale.
 */

void PMSlider::tick_mark( SHORT size, SHORT location )
{
  if( !send( SLM_SETTICKSIZE, MPFROM2SHORT( location, size ), 0 )) {
    PM_THROW_GUIERROR();
  }
}

/* Returns scale increments.
 */

USHORT PMSlider::scale1() const {
  return m_cdata.usScale1Increments;
}

/* Returns alternate scale increments.
 */

USHORT PMSlider::scale2() const {
  return m_cdata.usScale2Increments;
}

/* Returns the slider arm pos.
 */

USHORT PMSlider::arm_pos() const {
  return (USHORT)WinSendMsg( handle(), SLM_QUERYSLIDERINFO,
                             MPFROM2SHORT( SMA_SLIDERARMPOSITION, SMA_INCREMENTVALUE ), 0 );
}

/* Moves the slider arm to the specified pos.
 */

void PMSlider::arm_to( USHORT pos )
{
  if( !send(SLM_SETSLIDERINFO,
       MPFROM2SHORT( SMA_SLIDERARMPOSITION, SMA_INCREMENTVALUE ), MPFROMSHORT( pos )))
  {
    PM_THROW_GUIERROR();
  }
}

/* Default window procedure.
 *
 * IBM developers calculated size of the slider shaft and handle based on
 * the screen size. This is produces too enormous sliders on modern
 * high-resolution displays. Here I try recalculate slider size based on
 * current DPI value.
 */

MRESULT _System PMSlider::def_proc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
  switch( msg ) {
    case WM_CREATE:
    {
      MRESULT rc  = m_default_handler( hwnd, msg, mp1, mp2 );
      HPS     hps = WinGetPS( hwnd );
      HDC     hdc = GpiQueryDevice( hps );
      LONG    dpi = 96;

      DevQueryCaps( hdc, CAPS_HORIZONTAL_FONT_RES, 1L, &dpi );
      WinReleasePS( hps );

      WinSendMsg( hwnd, SLM_SETSLIDERINFO,
                  MPFROM2SHORT( SMA_SHAFTDIMENSIONS, SMA_RANGEVALUE ),
                  MPFROM2SHORT(( 12 * dpi / 96 + 1 ) & 0xFE, 0 ));
      WinSendMsg( hwnd, SLM_SETSLIDERINFO,
                  MPFROM2SHORT( SMA_SLIDERARMDIMENSIONS, SMA_RANGEVALUE ),
                  MPFROM2SHORT(( 12 * dpi / 96 + 1 ) & 0xFE, ( 24 * dpi / 96 + 1 ) & 0xFE ));

      return rc;
    }
  }

  return m_default_handler( hwnd, msg, mp1, mp2 );
}

