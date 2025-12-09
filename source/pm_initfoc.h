/*
 * Copyright (C) 2010 Dmitry A.Steklenev
 */

#ifndef PM_INITFOC_H
#define PM_INITFOC_H

#include "pm_os2.h"
#include "pm_noncopyable.h"
#include <foc.h>

/**
 * Register and unregister the FOC control window classes.
 *
 * This is an internal class type, used by other library classes.
 * You should not attempt to use it directly.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

class PMFOCInit : public PMNonCopyable
{
  public:
    PMFOCInit();
   ~PMFOCInit();

  private:
    static ULONG   m_initialized;
    static HMODULE m_hmodule;
};

static PMFOCInit pm_foc_init_;
#endif
