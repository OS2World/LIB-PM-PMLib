/*
 * Copyright (C) 2015 Dmitry A.Steklenev
 */

#include "pm_initslider.h"
#include "pm_slider.h"
#include "pm_gui.h"
#include "pm_error.h"

ULONG PMSliderInit::m_initialized = 0;

/* Registers the linear slider control window class.
 */

PMSliderInit::PMSliderInit()
{
  if( !m_initialized++ )
  {
    CLASSINFO cinfo;

    if( !WinQueryClassInfo( PMGUI::hab(), WC_SLIDER, &cinfo )) {
      PM_THROW_GUIERROR();
    }

    PMSlider::m_default_handler = cinfo.pfnWindowProc;

    if( !WinRegisterClass( PMGUI::hab(), PM_SLIDER, PMSlider::def_proc,
                           cinfo.flClassStyle & ~CS_PUBLIC, cinfo.cbWindowData ))
    {
     PM_THROW_GUIERROR();
    }
  }
}

