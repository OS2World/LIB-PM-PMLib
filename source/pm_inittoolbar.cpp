/*
 * Copyright (C) 2010 Dmitry A.Steklenev
 */

#include "pm_inittoolbar.h"
#include "pm_gui.h"
#include "pm_error.h"

#pragma library( "etoolbar.lib" )
#pragma library( "eicons.lib"   )
#pragma library( "ecomedia.lib" )

ULONG   PMToolBarInit::m_initialized = 0;
HMODULE PMToolBarInit::m_hmodule     = NULLHANDLE;

/* Registers the toolbar control window class.
 */

PMToolBarInit::PMToolBarInit()
{
  if( !m_initialized++ )
  {
    APIRET rc;
    char   error[512];

    rc = DosLoadModule( error, sizeof( error ), "etoolbar", &m_hmodule );

    if( rc != NO_ERROR ) {
      PMGUI gui;
      throw PMError( rc , "DOS", "Unable to load ETOOLBAR.DLL module.", PM_ERROR_LOCATION ).show();
    }
  }
}

/* Unregisters the toolbar control window class.
 */

PMToolBarInit::~PMToolBarInit()
{
  if( !--m_initialized ) {
    DosFreeModule( m_hmodule );
  }
}

