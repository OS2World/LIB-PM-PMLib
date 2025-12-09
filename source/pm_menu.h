/* Copyright (C) 2001-2017 Dmitry A.Steklenev
 */

#ifndef PM_MENU_H
#define PM_MENU_H

#include "pm_os2.h"
#include "pm_noncopyable.h"
#include "pm_font.h"

/**
 * Menu class.
 *
 * The PMMenu class creates a menu. A menu always is owned by
 * another window, usually a frame window. When a user makes a
 * choice from a menu, the menu posts a message WM_COMMAND
 * with CMDSRC_MENU type containing the unique identifier for
 * the menu item to its owner.
 *
 * You can construct and destruct objects of this class.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

class PMMenu : public PMNonCopyable {

  public:

    /**
     * Constructs the menu window from the dialog template.
     *
     * @param hparent       Specifies the oiwner- and parent-window handle.
     * @param res_id        Dialog-template identity within the resource file.
     *                      It is also used as the identity of the created menu.
     * @param hmodule       Module handle referencing a dynamic link library
     *                      containing the resource or NULLHANDLE for the
     *                      application's module.
     */

    PMMenu( HWND hparent, SHORT res_id, HMODULE hmodule );

    /**
     * Wraps the menu window object around an existing
     * presentation window handle.
     */

    PMMenu( HWND handle );

    /** Destructs the menu window object. */
    virtual ~PMMenu();

    /** Returns the menu window handle. */
    HWND  handle() const { return m_handle; }
    /** Returns the menu identifier. */
    SHORT id() const { return m_id; }

    /** Returns the number of items in the menu. */
    virtual SHORT count() const;

    /**
     * Adds an item into a menu.
     *
     * @param item_id     Item identity.
     * @param item_text   This points to a string containing the item text.
     * @param attribute   These menu item attributes are available:
     * <dl>
     * <dt><i>MIA_CHECKED </i><dd>If this attribute is TRUE, a check mark appears
     *                            next to the item (submenu only).
     * <dt><i>MIA_DISABLED</i><dd>This attribute is TRUE if the item is disabled
     *                            and cannot be selected. The item is drawn in a
     *                            disabled state.
     * </dl>
     */

    virtual void add_item( SHORT item_id, const char* item_text, USHORT attribute = 0 );

    /** Adds a separator into a menu. */
    virtual void add_separator( SHORT item_id );
    /** Removes an item from the menu. */
    virtual void remove( SHORT item_id );
    /** Removes all items from menu. */
    virtual void clear();

    /** Returns the text of the specified menu item. */
    virtual char* item_text( SHORT item_id, char* result, short size ) const;
    /** Returns the length of the text of the specified menu item. */
    virtual int item_text_len( SHORT item_id ) const;
    /** Sets the text of the specified menu item. */
    virtual void item_text( SHORT item_id, const char* text );

    /** Returns the handle of the specified submenu. */
    virtual HWND submenu( SHORT item_id ) const;

    /** Sets a new font to be used by the window. */
    virtual void font( const char* font );

    /**
     * Sets a new system font to be used by the window.
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

    virtual void font( int id );

    /**
     * Sets the check state of the specified menu item.
     *
     * @param item_id     Item identity.
     * @param state       Check flag.
     */

    virtual void check( SHORT item_id, BOOL state );

    /**
     * Sets the state of the specified menu item to the enable.
     *
     * @param item_id     Item identity.
     * @param state       Enable flag.
     */

    virtual void enable( SHORT item_id, BOOL state = TRUE );

    /**
     * Sets the state of the specified menu item to the disable.
     *
     * @param item_id     Item identity.
     * @param state       Enable flag.
     */

    virtual void disable( SHORT item_id );

    /**
     * Sets the default item in a conditional cascade menu.
     *
     * @param item_id     Item identity.
     */

    virtual void default_item( SHORT item_id );

    /**
     * Displays the pop-up menu.
     *
     * @param item_id     Item identity.
     */

    virtual void show( SHORT item_id );

    /**
     * Translates the whole menu.
     *
     * Translates the menu text and all the submenu texts using
     * for this purpose the current {@link PMLanguage::instance instance}
     * of the messages management object.
     */

    virtual void translate();

  private:

    HWND  m_handle;
    SHORT m_id;
    HWND  m_parent;
    int   m_sys_font;
    BOOL  m_wrapped;

    /** Translates the whole submenu. */
    void translate_submenu( HWND hwnd );
};

#endif
