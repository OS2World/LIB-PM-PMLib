/*
 * Copyright (C) 2010 Dmitry A.Steklenev
 */

#ifndef PM_INITPTRACER_H
#define PM_INITPTRACER_H

#include "pm_os2.h"
#include "pm_noncopyable.h"

/**
 * Register and unregister the log messages collector window class.
 *
 * This is an internal class type, used by other library classes.
 * You should not attempt to use it directly.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

class PMPTracerInit : public PMNonCopyable
{
  public:
    PMPTracerInit();
   ~PMPTracerInit();

  private:
    static ULONG   m_initialized;
    static HMODULE m_hmodule;
};

static PMPTracerInit pm_ptracer_init_;
#endif
