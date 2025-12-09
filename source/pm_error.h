/*
 * Copyright (C) 2001-2016 Dmitry A.Steklenev
 */

#ifndef PM_ERROR_H
#define PM_ERROR_H

#include <string.h>
#include <errno.h>
#include <stdio.h>

#include "pm_os2.h"

#ifndef __ccdoc__
#define PM_ERR_TOO_MANY_THREADS 1
#define PM_ERR_INITIALIZE       2
#define PM_ERR_SUBCLASS_WINDOW  3
#define PM_ERR_TOO_MANY_WINDOWS 4
#endif

/**
 * Base error class.
 *
 * The PMError class is the base class from which all
 * exception objects thrown in the library are derived.
 * These classes retrieve error information and text that
 * you can subsequently use to create an exception object.
 *
 * None of the functions in this class throws exceptions because
 * an exception probably has been thrown already or is about to be
 * thrown.
 *
 * You can construct, destruct and copy objects of this class.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

class PMError
{
  public:

    /**
     * Constructs an error object describing a current
     * GUI (PM) error.
     *
     * @param file      Specifies the name of the source file.
     * @param func      Specifies the name of the function.
     * @param line      Specifies the line number.
     */

    PMError( const char* file, const char* func, int line );

    /**
     * Constructs an error object from the specified error
     * id and information.
     *
     * @param code      Specifies the error identifier (error code).
     * @param group     Specifies the the group the error originated from.
     * @param what      Specifies the error description. If this parameter is
     *                  omitted (NULL) then the description of the standard OS/2
     *                  DOS error code is used.
     * @param file      Specifies the name of the source file.
     * @param func      Specifies the name of the function.
     * @param line      Specifies the line number.
     */

    PMError( int code, const char* group, const char* what,
                       const char* file,  const char* func, int line );

    /** Returns the source file name. */
    const char* file () const { return m_file;  }
    /** Returns the name of the function. */
    const char* func () const { return m_func;  }
    /** Returns the line number. */
    int         line () const { return m_line;  }
    /** Returns the error description. */
    const char* what () const { return m_what;  }
    /** Returns the error identifier. */
    int         code () const { return m_code;  }
    /** Returns the group the error originated from. */
    const char* group() const { return m_group; }

    /**
     * Displays the error information.
     *
     * @param out     Specifies the output stream. If this parameter is omitted (NULL)
     *                then displays a message box window.
     */

    const PMError& show( HFILE hout ) const;

    /**
     * Displays the error information using a message box window.
     */

    const PMError& show() const;

    /** Returns detailed error description of the standard system (DOS) error. */
    static char* sys_info( int code, char* buffer, int size );

  private:

    char m_file [CCHMAXPATH];
    char m_func [128];
    char m_group[64];
    int  m_line;
    char m_what [256];
    int  m_code;
    TID  m_tid;
};

/**
 * Current error location.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

#define PM_ERROR_LOCATION  __FILE__, __FUNCTION__, __LINE__

/**
 * Throws exception.
 *
 * The PM_THROW_ERROR macro accepts as input an error identifier and
 * error description. It expands to set the location information to the
 * instance, logs all instance data, and then throws the exception.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

#define PM_THROW_ERROR( id, group, what ) \
        throw( PMError( id, group, what, PM_ERROR_LOCATION ).show( PM_STDERR ))

/**
 * Throws the GUI (WIN) exception.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

#define PM_THROW_GUIERROR() \
        throw( PMError( PM_ERROR_LOCATION ).show( PM_STDERR ))

/**
 * Throws the Operating System (DOS) exception.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

#define PM_THROW_DOSERROR( rc ) PM_THROW_ERROR( rc, "DOS", NULL )

/**
 * Throws the C Library (CLIB) exception.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

#define PM_THROW_CLIBERROR() PM_THROW_ERROR( errno, "CLIB", strerror( errno ))
#endif
