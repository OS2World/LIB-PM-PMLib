/*
 * Copyright (C) 2012 Dmitry A.Steklenev
 */

#ifndef PM_URL_H
#define PM_URL_H

#include "pm_os2.h"

#ifndef __ccdoc__

#define IAPP_BROWSER 0x00000001UL
#define IAPP_MAIL    0x00000002UL
#endif


/**
 * URL control window class.
 *
 * The PMURL class creates and manages URL link control windows.
 *
 * You can construct and destruct objects of this class.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

class PMURL
{
  public:

    /**
     * Starts internet application that is appropriate to the specified URL link.
     *
     * @param app_id  Internet application identity:
     *
     * <dl>
     * <dt><i>IAPP_BROWSER</i>
     *        <dd>Web browser.
     * <dt><i>IAPP_MAIL</i>
     *        <dd>E-mail client.
     * </dl>
     *
     * @param url     URL or e-mail address.
     */

    static APIRET open( int app_id, const char* url );
};

#endif
