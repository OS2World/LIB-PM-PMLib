/*
 * Copyright (C) 2001-2017 Dmitry A.Steklenev
 */

#ifndef  PM_PROFILE_H
#define  PM_PROFILE_H

#include "pm_os2.h"
#include "pm_noncopyable.h"
#include <eicons.h>

/**
 * Profile management class.
 *
 * Class that an application would use to query or
 * write user-specific initialization files.
 *
 * You can construct and destruct objects of this class.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

class PMProfile : public PMNonCopyable {

  public:

    /**
     * Constructs a profile object.
     *
     * @param filename  User-profile file name.
     *                  This must not be the same as the current
     *                  user (OS2.INI) or system initialization (OS2SYS.INI)
     *                  file name.
     */

    PMProfile( const char* filename );

    /** Constructs a profile object of a current module. */
    PMProfile();

    /** Constructs a profile object from an existing profile handle. */
    PMProfile( HINI handle );

    /** Destructs the profile object. */
   ~PMProfile();

    /**
     * This method writes a binary data into the profile.
     *
     * @param app     Application name. The case-dependent name of
     *                the application for which profile data is to be written.
     *                Names starting with the characters "PM_" are reserved
     *                for system use.
     * @param key     The case-dependent name of the key for which profile
     *                data is to be written. This parameter can be NULL in
     *                which case all the <i>key</i> or <i>data</i> pairs associated
     *                with <i>app</i> are deleted.
     * @param data    Value data. If this parameter is NULL, the data associated with
     *                the <i>key</i> parameter is deleted.
     * @param len     The size of the data to be written. This length should not
     *                exceed 64K bytes.
     */

    void put_data( const char* app, const char* key, const void* data, unsigned long len );

    /**
     * This method writes a long integer value into the profile.
     *
     * @param app     The case-dependent application name.
     * @param key     The case-dependent name of the key.
     * @param value   The long integer value.
     *
     * Also see more detailed description of
     * the {@link PMProfile::put_data put_data} parameters.
     */

    void put_integer( const char* app, const char* key, long value );

    /**
     * This method writes a unsigned long integer value into the profile.
     *
     * @param app     The case-dependent application name.
     * @param key     The case-dependent name of the key.
     * @param value   The unsigned long integer value.
     *
     * Also see more detailed description of
     * the {@link PMProfile::put_data put_data} parameters.
     */

    void put_integer( const char* app, const char* key, unsigned long value );

    /**
     * This method writes a short integer value into the profile.
     *
     * @param app     The case-dependent application name.
     * @param key     The case-dependent name of the key.
     * @param value   The short integer value.
     *
     * Also see more detailed description of
     * the {@link PMProfile::put_data put_data} parameters.
     */

    void put_integer( const char* app, const char* key, short value );

    /**
     * This method writes a unsigned short integer value into the profile.
     *
     * @param app     The case-dependent application name.
     * @param key     The case-dependent name of the key.
     * @param value   The unsigned short integer value.
     *
     * Also see more detailed description of
     * the {@link PMProfile::put_data put_data} parameters.
     */

    void put_integer( const char* app, const char* key, unsigned short value );

    /**
     * This method writes a character string into the profile.
     *
     * @param app     The case-dependent application name.
     * @param key     The case-dependent name of the key.
     * @param string  Pointer to a text string that ends with a null character.
     *
     * Also see more detailed description of
     * the {@link PMProfile::put_data put_data} parameters.
     */

    void put_string( const char* app, const char* key, const char* string );

    /**
     * Returns the size in bytes of the value of a specified key for a specified application in the profile.
     *
     * @param app     The name of the application for which the profile data is required.
     *
     *                If the <i>app</i> parameter is NULL, then the method returns the
     *                length of the buffer required to hold the enumerated list of
     *                application names, as returned by the <i>get_data</i> method
     *                when its <i>app</i> parameter is NULL. In this case, the <i>key</i>
     *                parameter is ignored.
     *
     * @param key     Key name. The name of the key for which the size of the data is to be returned.
     *
     *                If the <i>key</> parameter is NULL, and if the <i>app</i> parameter is not NULL,
     *                the method returns the length of the buffer required to hold the enumerated list
     *                of key names for that application name, as returned by the <i>get_data</i>
     *                method when its <i>key</i> parameter is NULL, and its <i>app</i> parameter is not
     *                NULL.
     *
     * @return        The length of the value data related to the <i>key</i> parameter. If an error
     *                occurs, returns 0. The maximum size of data than can be returned is 64K bytes.
     */

    unsigned long get_size( const char* app, const char* key ) const;

    /**
     * Returns a binary data from the profile.
     *
     * @param app     Application name. The name of the application for which
     *                the profile data is required. The name must match exactly
     *                with the name stored in the profile. There is no
     *                case-independent searching.
     *
     *                If this parameter is NULL, this function enumerates all
     *                the application names present in the profile and returns
     *                the names as a list in the <i>data</i> parameter. Each
     *                application name is terminated with a NULL character and
     *                the last name is terminated with two successive NULL characters.
     *
     * @param key     Key name. The name of the key for which the profile data
     *                is required. The name must match exactly with the name stored
     *                in the profile. There is no case-independent searching.
     *
     *                If this parameter is NULL, and if <i>app</i> is not equal to NULL,
     *                this call enumerates all key names associated with the named
     *                application and returns the key names, but not their values,
     *                as a list in the <i>data</i> parameter. Each key name is
     *                terminated with a NULL character and the last name is terminated
     *                with two successive NULL characters.
     *
     * @param data    A buffer in which the value corresponding to the key name
     *                is returned.
     * @param len     Size of value data. This is the size of the buffer specified
     *                by the <i>data</i> parameter. If the call is successful, this is
     *                overwritten with the number of bytes copied into the buffer.
     *                The maximum amount of data that can be returned is 64K bytes.
     *
     * @return        Success indicator.
     */

    BOOL get_data( const char* app, const char* key, void* data, unsigned long* len ) const;

    /**
     * Returns a long integer value from the profile.
     *
     * @param app     The case-dependent application name.
     * @param key     The case-dependent name of the key.
     * @param value   Pointer to the long integer value.
     *
     * Also see more detailed description of
     * the {@link PMProfile::get_data get_data} parameters.
     *
     * @return        Success indicator.
     */

    BOOL get_integer( const char* app, const char *key, long* value ) const;

    /**
     * Returns a unsigned long integer value from the profile.
     *
     * @param app     The case-dependent application name.
     * @param key     The case-dependent name of the key.
     * @param value   Pointer to the unsigned long integer value.
     *
     * Also see more detailed description of
     * the {@link PMProfile::get_data get_data} parameters.
     *
     * @return        Success indicator.
     */

    BOOL get_integer( const char* app, const char *key, unsigned long* value ) const;

    /**
     * Returns a short integer value from the profile.
     *
     * @param app     The case-dependent application name.
     * @param key     The case-dependent name of the key.
     * @param value   Pointer to the short integer value.
     *
     * Also see more detailed description of
     * the {@link PMProfile::get_data get_data} parameters.
     *
     * @return        Success indicator.
     */

    BOOL get_integer( const char* app, const char *key, short* value ) const;

    /**
     * Returns a unsigned short integer value from the profile.
     *
     * @param app     The case-dependent application name.
     * @param key     The case-dependent name of the key.
     * @param value   Pointer to the unsigned short integer value.
     *
     * Also see more detailed description of
     * the {@link PMProfile::get_data get_data} parameters.
     *
     * @return        Success indicator.
     */

    BOOL get_integer( const char* app, const char *key, unsigned short* value ) const;

    /**
     * Returns a character string from the profile.
     *
     * @param app     The case-dependent application name.
     * @param key     The case-dependent name of the key.
     * @param string  The text string obtained from the profile.
     * @param len     Maximum string length.
     *
     * Also see more detailed description of
     * the {@link PMProfile::get_data get_data} parameters.
     *
     * @return        Success indicator.
     */

    BOOL get_string( const char* app, const char *key, char* string, unsigned long len ) const;

    /**
     * Saves the current size and position of the window.
     *
     * Saves the current size and position of the window
     * specified by <i>hwnd</i>. This function will also
     * save the presentation parameters.
     *
     * @param app     The case-dependent application name.
     * @param key     The case-dependent name of the key.
     * @param hwnd    Window handle for the window to be stored.
     * @param options Options. Reserved. Must be 0.
     */

    BOOL save_window( const char* app, const char *key, HWND hwnd, int options = 0 );

    /**
     * Restores the size and position of the window.
     *
     * Restores the size and position of the window specified
     * by <i>hwnd</i> to the state it was in when <i>save_window</i>
     * was last called. This function will also restore presentation
     * parameters.
     *
     * @param app     The case-dependent application name.
     * @param key     The case-dependent name of the key.
     * @param hwnd    Window handle for the window to be stored.
     * @param options Options:
     * <dl>
     * <dt><i>WP_HCONSTRAIN</i><dd>Constrain the window so that its width
     *                             is wholly visible on the desktop.
     * <dt><i>WP_VCONSTRAIN</i><dd>Constrain the window so that its height
     *                             is wholly visible on the desktop.
     * </dl>
     */

    BOOL rest_window( const char* app, const char *key, HWND hwnd, int options = 0 );

    #ifndef INI_SECTION
    /** Defines a section name for easy way profile macros. */
    #define INI_SECTION  "Settings"
    #endif

    /** Easy way to save a characters string to the profile. */
    #define save_string( var ) \
      put_string( INI_SECTION, #var, var )
    /** Easy way to save a integer value to the profile. */
    #define save_integer( var ) \
      put_integer( INI_SECTION, #var, var )
    /** Easy way to restore a characters string from the profile. */
    #define load_string( var ) \
      get_string( INI_SECTION, #var, var, sizeof( var ))
    /** Easy way to restore a integer value from the profile. */
    #define load_integer( var ) \
      get_integer( INI_SECTION, #var, &var )

  private:

    HINI m_handle;
    BOOL m_wrapped;

    /** Copies the specified data from one profile to another. */
    BOOL copy( HINI ini_from, const char* app_from, const char* key_from,
               HINI ini_to,   const char* app_to,   const char* key_to );
    /** Moves  the specified data from one profile to another. */
    BOOL move( HINI ini_from, const char* app_from, const char* key_from,
               HINI ini_to,   const char* app_to,   const char* key_to );
};

#endif
