/*
 * Copyright (C) 2010-2020 Dmitry A.Steklenev
 */

#ifndef PM_NLS_H
#define PM_NLS_H

#include "pm_os2.h"
#include "pm_noncopyable.h"
#include "pm_initnls.h"
#include <uconv.h>

/**
 * National language support class.
 *
 * Provides national language support via
 * various conversion and comparision member functions.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

class PMNLS : public PMNonCopyable
{
  friend class PMInitNLS;

  public:

    /**
     * Converts the long long integer to the
     * decimal ASCII string separated by thousands.
     *
     * @param result  A buffer in which the converted text is returned.
     * @param size    This is the size of the buffer specified by the <i>result</i>.
     *                parameter.
     *
     * @return A pointer to <i>result</i>. There is no error return value.
     */

    static char* lltoa( char* result, int size, long long number );

    /**
     * Converts the long long integer to the human readable format.
     *
     * @param result  A buffer in which the converted text is returned. The current
     *                {@link PMLanguage::instance instance} of the message management
     *                object is used to translate the unit name.
     * @param size    This is the size of the buffer specified by the <i>result</i>.
     *                parameter.
     *
     * @return A pointer to <i>result</i>. There is no error return value.
     */

    static char* lltohuman( char* result, int size, long long number );

    /**
     * Converts time to formatted character string containing
     * separated hours, minutes and seconds (not daytime).
     *
     * @param result  A buffer in which the converted text is returned.
     * @param size    This is the size of the buffer specified by the <i>result</i>.
     *                parameter.
     *
     * @return A pointer to <i>result</i>. There is no error return value.
     */

    static char* timetohms( char* result, int size, unsigned long seconds );

    /**
     * Converts time to formatted character string containing
     * separated minutes and seconds.
     *
     * @param result  A buffer in which the converted text is returned.
     * @param size    This is the size of the buffer specified by the <i>result</i>.
     *                parameter.
     *
     * @return A pointer to <i>result</i>. There is no error return value.
     */

    static char* timetoms( char* result, int size, unsigned long seconds );

    /** Compare strings without case sensitivity. */
    static int stricmp( const char* string1, const char* string2 );
    /** Compare strings without case sensitivity. */
    static int strnicmp( const char* string1, const char* string2, int n );

    /**
     * Converts any lowercase letters in string to uppercase.
     * Other characters are not affected.
     */

    static char* strupr( char* string );

    /**
     * Converts the lowercase letter to the corresponding
     * uppercase letter.
     */

    static int toupper( int c );

    /**
     * Converts the string from current codepage to the unicode string.
     *
     * @param string  A pointer to the input null terminated string.
     * @param result  A buffer in which the converted string is returned.
     * @param size    This is the size in unicode characters of the buffer
     *                 specified by the <i>result</i> parameter.
     *
     * @return If successfully converts the source string, it returns the number
     *         of converted characters. If encounters an error, it returns -1.
     */

    static int strtoucs( const char* string, UniChar* result, int size );

    /**
     * Converts the unicode string to current codepage string.
     *
     * @param string  A pointer to the input null terminated string.
     * @param result  A buffer in which the converted string is returned.
     * @param size    This is the size in bytes of the buffer
     *                 specified by the <i>result</i> parameter.
     *
     * @return If successfully converts the source string, it returns the number
     *         of converted characters. If encounters an error, it returns -1.
     */

    static int ucstostr( const UniChar* string, char* result, int size );

    /**
     * Converts the string from current codepage to the UTF-8 string.
     *
     * @param string  A pointer to the input null terminated string.
     * @param result  A buffer in which the converted string is returned.
     * @param size    This is the size in bytes of the buffer
     *                specified by the <i>result</i> parameter.
     *
     * @return If successfully converts the source string, it returns the number
     *         of converted characters. If encounters an error, it returns -1.
     */

    static int strtoutf8( const char* string, char* result, int size );

    /**
     * Converts the UTF-8 string to current codepage string.
     *
     * @param string  A pointer to the input null terminated string.
     * @param result  A buffer in which the converted string is returned.
     * @param size    This is the size in bytes of the buffer
     *                specified by the <i>result</i> parameter.
     *
     * @return If successfully converts the source string, it returns the number
     *         of converted characters. If encounters an error, it returns -1.
     */

    static int utf8tostr( const char* string, char* result, int size );

  private:

    static COUNTRYINFO m_country_info;
    static CHAR        m_mapcase[256];
    static UconvObject m_uc_curcp;
    static UconvObject m_uc_utf8;
};

/* Converts the lowercase letter to the corresponding
 * uppercase letter.
 */

inline int PMNLS::toupper( int c ) {
  return m_mapcase[(unsigned char)c];
}

#endif
