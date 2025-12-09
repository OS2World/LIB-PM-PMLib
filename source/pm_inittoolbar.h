/*
 * Copyright (C) 2010 Dmitry A.Steklenev
 */

#ifndef PM_INITTOOLBAR_H
#define PM_INITTOOLBAR_H

#include "pm_os2.h"
#include "pm_noncopyable.h"

/**
 * Register and unregister the toolbar control window class.
 *
 * This is an internal class type, used by other library classes.
 * You should not attempt to use it directly.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

class PMToolBarInit : public PMNonCopyable
{
  public:
    PMToolBarInit();
   ~PMToolBarInit();

  private:
    static ULONG   m_initialized;
    static HMODULE m_hmodule;
};

static PMToolBarInit pm_toolbar_init_;
#endif
