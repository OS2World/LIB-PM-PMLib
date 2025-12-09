/*
 * Copyright (C) 2013-2015 Dmitry A.Steklenev
 */

#include <stdlib.h>
#include "pm_filelist.h"
#include "pm_errors.h"
#include "pm_memory.h"

/* Constructs the empty list object.
 */

PMFileList::PMFileList()
{
  cb = sizeof( FILELIST );
  ulFQFCount = 0;
  papszFQFilename = NULL;
}

/* Destroys the list of files or directories.
 */

PMFileList::~PMFileList()
{
  for( ULONG i = 0; i < ulFQFCount; i++ ) {
    xfree((*papszFQFilename)[i]);
  }
  xfree( papszFQFilename );
}

/* Adds file or directory to the list.
 */

void PMFileList::add( const char* pathname, const char* prefix )
{
  if(!( ulFQFCount % 50 ))
  {
    papszFQFilename =
      (PAPSZ)xrealloc( papszFQFilename, ( ulFQFCount + 50 ) * sizeof( PSZ ));
  }

  (*papszFQFilename)[ulFQFCount] =
    (PSZ)xmalloc( strlen( pathname ) + ( prefix ? strlen( prefix ) : 0 ) + 1 );

  if( prefix ) {
    strcpy((*papszFQFilename)[ulFQFCount], prefix   );
    strcat((*papszFQFilename)[ulFQFCount], pathname );
  } else {
    strcpy((*papszFQFilename)[ulFQFCount], pathname );
  }

  ++ulFQFCount;
}

