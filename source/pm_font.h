/*
 * Copyright (C) 2011-2017 Dmitry A.Steklenev
 */

#ifndef PM_FONT_H
#define PM_FONT_H

#include "pm_os2.h"
#include <eicons.h>

#ifndef __ccdoc__
#define SYSFONT_PREFORMATTED  4
#endif

/**
 * Font class.
 *
 * The PMFont class manages the use of fonts.
 * You can construct, destruct, copy, and assign objects of this class.
 *
 * @author Dmitry A Steklenev
 */

class PMFont
{
  public:

    /**
     * Constructs the font object from characters string.
     *
     * @param fontname The format of the font name string is as follows:
     * <p>
     * &lt;point size&gt;.&ltface name&gt(.&ltmodifer&gt(.&ltmodifier&gt...
     * <p>
     * where:
     * <dl>
     * <dt>&lt;point size&gt; is the point size of the font
     * <dt>&lt;face name&gt; is the face name of the font
     * <dt>&lt;modifier&gt; is one of the following:
     *
     * <ul>
     * <li>Bold
     * <li>Italic
     * <li>Underscore
     * <li>Outline
     * <li>Strikeout
     * </ul>
     * </dl>
     *
     * Examples are "12.New Times Roman" and "10.Helvetica.Bold.Italic"
     *
     * <b>Note:</b> Modifiers can be used to create a font with a combination of
     * the listed attributes. Do not use modifiers if a true font of that
     * type is already available. For example, use "10.Helvetica Bold"
     * instead of "10.Helvetica.Bold."
     */

    PMFont( const char* fontname );

    /**
     * Returns the face name of the font.
     *
     * @param result  A buffer in which the face name is returned.
     * @param size    This is the size of the buffer specified by the <i>result</i>
     *                parameter. If the call is successful, this is overwritten
     *                with the number of bytes copied into the buffer.
     */

    char* name( char* result, int size ) const;

    /** Returns the point size of the font. */
    LONG size() const;
    /** Returns the font attributes. */
    const FATTRS* attrs() const;

    /**
     * Returns a system font name.
     *
     * Allows the application to ask for details about
     * the user interface defined fonts.
     *
     * @param id  Font identity:
     *
     * <dl>
     * <dt><i>SYSFNT_WINDOWTEXT</i>
     *        <dd>System font for window text.
     * <dt><i>SYSFNT_WINDOWTITLES</i>
     *        <dd>System font for window title text.
     * <dt><i>SYSFNT_MENU</i>
     *        <dd>System font for menu text.
     * <dt><i>SYSFNT_ICONTEXT</i>
     *        <dd>System font for icon text.
     * <dt><i>SYSFNT_PREFORMATTED</i>
     *        <dd>System-fixed width font for preformatted text.
     * </dl>
     */

    static const char* sys_font( int id );

  private:

    FATTRS m_attrs;
    LONG   m_points;
    SIZEF  m_sizef;

    static char m_sys_fonts[5][SYSFONT_MAXLEN];
};

/* Returns the point size of the font.
 */

inline LONG PMFont::size() const {
  return m_points;
}

/* Returns the font attributes.
 */

inline const FATTRS* PMFont::attrs() const {
  return &m_attrs;
}

#endif
