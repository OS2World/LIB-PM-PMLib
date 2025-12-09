/*
 * Copyright (C) 2010 Dmitry A.Steklenev
 */

#include "pm_initfoc.h"
#include "pm_gui.h"
#include "pm_error.h"

#pragma library( "foc.lib" )

ULONG   PMFOCInit::m_initialized = 0;
HMODULE PMFOCInit::m_hmodule     = NULLHANDLE;

/* Registers the FOC control window classes.
 */

PMFOCInit::PMFOCInit()
{
  if( !m_initialized++ )
  {
    APIRET rc;
    char   error[512];

    rc = DosLoadModule( error, sizeof( error ), "foc", &m_hmodule );

    if( rc != NO_ERROR ) {
      PMGUI gui;
      throw PMError( rc , "DOS", "Unable to load FOC.DLL module.", PM_ERROR_LOCATION ).show();
    }
  }
}

/* Deregisters the FOC control window classes.
 */

PMFOCInit::~PMFOCInit()
{
  if( !--m_initialized ) {
    DosFreeModule( m_hmodule );
  }
}
