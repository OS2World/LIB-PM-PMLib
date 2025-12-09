/*
 * Copyright (C) 2010-2016 Dmitry A.Steklenev
 */

#include <string.h>
#include "pm_initnls.h"
#include "pm_nls.h"
#include "pm_errors.h"
#include "pm_gui.h"

ULONG PMInitNLS::m_initialized = 0;

/* Initializes the national language support class.
 */

PMInitNLS::PMInitNLS()
{
  if( !m_initialized++ )
  {
    COUNTRYCODE countrycode = { 0, 0 };

    int   i;
    ULONG bytes;

    for( i = 0; i < 256; i++ ) {
      PMNLS::m_mapcase[i] = (CHAR)i;
    }

    DosMapCase( 256, &countrycode, PMNLS::m_mapcase );

    if( DosQueryCtryInfo( sizeof( PMNLS::m_country_info ), &countrycode,
                                 &PMNLS::m_country_info, &bytes ) != NO_ERROR )
    {
      strcpy( PMNLS::m_country_info.szThousandsSeparator, " " );
      strcpy( PMNLS::m_country_info.szTimeSeparator, ":" );
    }

    PrfQueryProfileString( HINI_USERPROFILE, "PM_National", "sThousand", NULL,
                           PMNLS::m_country_info.szThousandsSeparator,
                           sizeof( PMNLS::m_country_info.szThousandsSeparator ));

    PrfQueryProfileString( HINI_USERPROFILE, "PM_National", "sTime", NULL,
                           PMNLS::m_country_info.szTimeSeparator,
                           sizeof( PMNLS::m_country_info.szTimeSeparator ));

    ULONG   cpage[1] = { 0 };
    ULONG   cb = sizeof( cpage );
    UniChar cpname[16];
    APIRET  rc;

    DosQueryCp( cb, cpage, &cb );

    if(( rc = UniMapCpToUcsCp( cpage[0], cpname, 16 )) != ULS_SUCCESS ||
       ( rc = UniCreateUconvObject( cpname, &PMNLS::m_uc_curcp )) != ULS_SUCCESS )
    {
      PMGUI gui;
      throw PMError( rc , "DOS",
              "Unable to create and initialize of an unicode conversion object.",
              PM_ERROR_LOCATION ).show();
    }

    if(( rc = UniCreateUconvObject( UTF_8, &PMNLS::m_uc_utf8 )) != ULS_SUCCESS )
    {
      PMGUI gui;
      throw PMError( rc , "DOS",
              "Unable to create and initialize of an unicode conversion object.",
              PM_ERROR_LOCATION ).show();
    }
  }
}

/* Terminates the national language support class.
 */

PMInitNLS::~PMInitNLS() {
  if( !--m_initialized ) {
    UniFreeUconvObject( PMNLS::m_uc_curcp );
    UniFreeUconvObject( PMNLS::m_uc_utf8  );
  }
}

