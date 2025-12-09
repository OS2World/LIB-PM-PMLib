/*
 * Copyright (C) 2013 Dmitry A.Steklenev
 */

#ifndef PM_FILELIST_H
#define PM_FILELIST_H

#include "pm_os2.h"
#include "pm_noncopyable.h"
#include <foc.h>

/**
 * List of files or directories.
 *
 * The PMFilList class represents a FILELIST structures that contains
 * information about list of files or directories and used by File Open
 * Container controls.
 *
 * You can construct and destruct objects of this class.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

class PMFileList : public FILELIST, public PMNonCopyable
{
  public:

    /** Constructs the empty list object. */
    PMFileList();
    /** Destroys the list of files or directories. */
   ~PMFileList();

    /** Adds file or directory to the list. */
    void add( const char* pathname, const char* prefix = NULL );
    /** Is a list empty. */
    BOOL empty() const;
};

/* Is a list empty.
 */

inline BOOL PMFileList::empty() const {
  return ( ulFQFCount == 0 );
}

#endif
