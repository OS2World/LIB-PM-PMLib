/*
 * Copyright (C) 2015 Dmitry A.Steklenev
 */

#ifndef PM_INITSLIDER_H
#define PM_INITSLIDER_H

#include "pm_os2.h"
#include "pm_noncopyable.h"

/**
 * Register and unregister the linear slider control window class.
 *
 * This is an internal class type, used by other library classes.
 * You should not attempt to use it directly.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

class PMSliderInit : public PMNonCopyable
{
  public:
    PMSliderInit();

  private:
    static ULONG m_initialized;
};

static PMSliderInit pm_slider_init_;
#endif
