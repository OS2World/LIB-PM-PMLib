/*
 * Copyright (C) 2013-2015 Dmitry A.Steklenev
 */

#include "pm_url.h"
#include "pm_profile.h"
#include "pm_fileutils.h"
#include "pm_memory.h"
#include <stdlib.h>
#include <ctype.h>

APIRET PMURL::open( int app_id, const char* url )
{
  char program[CCHMAXPATH]    = "";
  char params[256]            = "";
  char workingdir[CCHMAXPATH] = "";

  PMProfile profile( HINI_USERPROFILE );
  APIRET rc = NO_ERROR;

  if( app_id == IAPP_MAIL ) {
    profile.get_string( "WPURLDEFAULTSETTINGS", "DefaultMailExe", program, sizeof(program));
    profile.get_string( "WPURLDEFAULTSETTINGS", "DefaultMailParameters", params, sizeof(params));
    profile.get_string( "WPURLDEFAULTSETTINGS", "DefaultMailWorkingDir", workingdir, sizeof(workingdir));
  } else {
    profile.get_string( "WPURLDEFAULTSETTINGS", "DefaultBrowserExe", program, sizeof(program));
    profile.get_string( "WPURLDEFAULTSETTINGS", "DefaultWorkingDir", workingdir, sizeof(workingdir));
  }

  if( !*program )
  {
    char  buf[128];

    if( profile.get_string( "Mozilla", "CurrentVersion", buf, sizeof(buf))) {
      if( profile.get_string( buf, "PathToExe", program, sizeof(program))) {
        sdrivedir( workingdir, program, sizeof(workingdir));
        if( *workingdir && !is_root( workingdir )) {
          // Remove the trailing slash.
          workingdir[strlen(workingdir)-1] = 0;
        }
        if( app_id == IAPP_MAIL ) {
          strcpy( params, "-compose" );
        }
      }
    }
  }

  if( *program )
  {
    char* args;
    char* p;
    char  curdir[CCHMAXPATH] = "";
    RESULTCODES res_codes;

    // The convention used by CMD.EXE is that the first of
    // these strings is the program name (as entered from the
    // command prompt or found in a batch file), and the second
    // string consists of the parameters for the program. The
    // second ASCIIZ string is followed by an additional byte of
    // zeros.

    args = (char*)xcalloc( strlen(program) + strlen(params) + strlen(url) + 64, 1);

    strcpy( args, program );
    p = args + strlen(args) + 1;

    if( *params ) {
      strcat( p, params );
      if( *url ) {
        strcat( p, " " );
      }
    }
    if( *url )
    {
      if( *workingdir ) {
        getcwd( curdir, sizeof( curdir ));
        chdrivedir( workingdir );
      }

      if( app_id == IAPP_BROWSER ) {
        strcat( p, "\"" );
        strcat( p, url );
        strcat( p, "\"" );
      } else if( app_id == IAPP_MAIL ) {
        if( strnicmp( url, "mailto:", 7 ) != 0 ) {
          strcat( p, "mailto:\"" );
        } else {
          strcat( p, "\"" );
        }
        strcat( p, url );
        strcat( p, "\"" );
      } else {
        strcat( p, url );
      }

      rc = DosExecPgm( NULL, 0, EXEC_ASYNC, args, NULL, &res_codes, program );
      xfree( args );

      if( *curdir ) {
        chdrivedir( curdir );
      }
    }
  } else {
    rc = ERROR_FILE_NOT_FOUND;
  }

  return rc;
}
