/*
 * Copyright (C) 2010 Dmitry A.Steklenev
 */

#ifndef PM_INITNLS_H
#define PM_INITNLS_H

#include "pm_os2.h"
#include "pm_noncopyable.h"

/**
 * Initializes and terminates the national language support class.
 *
 * This is an internal class type, used by other library classes.
 * You should not attempt to use it directly.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

class PMInitNLS : public PMNonCopyable
{
  public:
    PMInitNLS();
   ~PMInitNLS();

  private:
    static ULONG m_initialized;
};

static PMInitNLS pm_nls_init_;
#endif
