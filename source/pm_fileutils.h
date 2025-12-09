/*
 * Copyright (C) 2004-2013 Dmitry A.Steklenev
 */

#ifndef PM_FILEUTIL_H
#define PM_FILEUTIL_H

#include <string.h>
#include <direct.h>
#include "pm_os2.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @pkg File_Utilities */

/**
 * File and directory management functions.
 * @pkgdoc File_Utilities
 */

/**
 * Returns the drive letter followed by a colon (:)
 * if a drive is specified in the location.
 *
 * Returns an empty string if specified path does not contain
 * definition of the drive.
 *
 * Note: Because the result string always less or is equal to a location
 * string all functions can safely use the same storage area for a
 * location and result.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

char* sdrive( char* result, const char* location, size_t size );

/**
 * Returns the scheme followed by a colon (:) of the specified location.
 *
 * Returns an empty string if specified location does not contain
 * definition of the url scheme.
 *
 * Note: Because the result string always less or is equal to a location
 * string all functions can safely use the same storage area for a
 * location and result.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

char* scheme( char* result, const char* location, size_t size );

/**
 * Returns the base file name without any extensions.
 *
 * Note: Because the result string always less or is equal to a location
 * string all functions can safely use the same storage area for a
 * location and result.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

char* sfname( char* result, const char* location, size_t size );

/**
 * Returns the file name extension, if any, including the leading period (.).
 *
 * Note: Because the result string always less or is equal to a location
 * string all functions can safely use the same storage area for a
 * location and result.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

char* sfext( char* result, const char* location, size_t size );

/**
 * Replaces an extension of the specified file.
 *
 * The specified extension may include the leading period (.).
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

char* sfextrep( char* filename, const char* ext, int size );

/**
 * Returns the base file name with file extension.
 *
 * The specified extension must include the leading period (.).
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

char* sfnameext( char* result, const char* location, size_t size );

/**
 * Returns the drive letter or scheme and the path of subdirectories, if any, including the trailing slash.
 *
 * Slashes (/), backslashes (\), or both may be present in location.
 *
 * Returns an empty string if specified location does not contain
 * any path definition.
 *
 * Note: Because the result string always less or is equal to a location
 * string all functions can safely use the same storage area for a
 * location and result.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

char* sdrivedir( char* result, const char* location, size_t size );

/**
 * Passed any string value, decode from URL transmission.
 *
 * Note: Because the result string always less or is equal to a location
 * string all functions can safely use the same storage area for a
 * location and result.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

char* sdecode( char* result, const char* location, size_t size );

/**
 * Returns TRUE if the specified location is a regular file.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

BOOL is_file( const char* location );

/**
 * Returns TRUE if the specified location is a URL.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

BOOL is_url ( const char* location );

/**
 * Returns TRUE if the specified location is a root directory.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

BOOL is_root( const char* location );

/**
 * Returns TRUE if the specified location is a directory.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

BOOL is_dir ( const char* location );

/**
 * Creates a single path name, composed of a base path name and file
 * or directory name.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

char* smakepath( char* result, const char* pathname, const char* name, int size );

/**
 * Change current default drive and working directory.
 *
 * Causes the current default drive and working directory to change to the
 * directory specified by <i>pathname</i>. The <i>pathname</i> must refer to
 * an existing directory.
 *
 * Returns a value of 0 if the default drive and working directory was
 * successfully changed. A return value of -1 indicates an error.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

int chdrivedir( const char* pathname );

#ifdef __cplusplus
}
#endif

#endif /* PM_FILEUTIL_H */
