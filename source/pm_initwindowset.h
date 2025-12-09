/*
 * Copyright (C) 2010 Dmitry A.Steklenev
 */

#ifndef PM_INITWINDOWSET_H
#define PM_INITWINDOWSET_H

#include "pm_os2.h"
#include "pm_noncopyable.h"

/**
 * Initializes and terminates the windows set class.
 *
 * This is an internal class type, used by other library classes.
 * You should not attempt to use it directly.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

class PMInitWindowSet : public PMNonCopyable
{
  public:
    PMInitWindowSet();
   ~PMInitWindowSet();

  private:
    static ULONG m_initialized;
};

static PMInitWindowSet pm_windowset_init_;
#endif
