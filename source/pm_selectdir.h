/*
 * Copyright (C) 2010-2013 Dmitry A.Steklenev
 */

#ifndef PM_SELECTDIR_FOC_H
#define PM_SELECTDIR_FOC_H

#include "pm_os2.h"
#include "pm_noncopyable.h"
#include <foc.h>

/**
 * Directory selection dialog class.
 *
 * The directory selection dialog class creates a directory selection
 * dialog object for the user to choose a directory. Once the user has
 * chosen a directory, you can use member functions of this class to
 * retrieve information about the chosen directory.
 *
 * You can construct and destruct objects of this class.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.1
 */

class PMSelectDir : public PMNonCopyable
{
  public:

    /**
     * Constructs the directory selection dialog object.
     *
     * This constructor create directory selection dialog object.
     * You must use <i>show_modally</i> to activate it.
     *
     * @param caption  The dialog window title
     * @param howner   Specifies the owner-window handle.
     * @param pathname The directory name to use as the default.
     * @param style    Specifies the dialog style:
     *
     * <dl>
     * <dt><i>FDS_CENTER  </i><dd>The dialog is positioned in the center of
     *                            its owner window.
     * </dl>
     */

    PMSelectDir( const char* caption, HWND howner, const char* pathname, ULONG style );

    /**
     * Constructs the custom selection dialog object.
     *
     * This constructor create directory selection dialog object.
     * You must use <i>show_modally</i> to activate it.
     *
     * @param caption  The dialog window title
     * @param howner   Specifies the owner-window handle.
     * @param pathname The directory name to use as the default.
     * @param hmodule  This is the HMODULE from which the custom file dialog template is
     *                 loaded. NULLHANDLE causes the dialog resource to be pulled from
     *                 the module of the current EXE.
     * @param res_id   Contains the ID of the resource containing the custom dialog template.
     * @param style    Specifies the dialog style:
     *
     * <dl>
     * <dt><i>FDS_CENTER  </i><dd>The dialog is positioned in the center of
     *                            its owner window.
     * </dl>
     */

    PMSelectDir( const char* caption, HWND howner, const char* pathname,
                 HMODULE hmodule, SHORT res_id, ULONG style );
    /**
     * Destructs the directory selection dialog object.
     *
     * Cleans up as this dialog object and destroying the
     * associated presentation window for this object.
     */

    virtual ~PMSelectDir();

    /**
     * Displays the directory selection dialog.
     *
     * While the dialog window is being shown modally, its parent or owner or
     * both windows are disabled. Other top level windows belonging to the
     * application are not disabled.
     *
     * @return Result code from dialog dismissal. This field contains the
     *         ID of the push button pressed to dismiss the dialog, DID_OK
     *         or DID_CANCEL. If an error occurs on dialog invocation,
     *         this value is set to zero.
     */

    virtual ULONG show_modally();

    /**
     * Returns the chosen directory.
     *
     * @param   filename  A buffer in which the chosen directory name is returned.
     * @param   size      This is the size of the buffer specified by
     *                    the <i>filename</i> parameter.
     *
     * @return  Success indicator.
     */

    virtual BOOL selected( char* pathname, int size );

    /** Returns the presentation window handle. */
    HWND handle() const { return m_handle; }

  protected:

    /**
     * Dispatch system native events.
     *
     * PMSelectDir calls this function when a system native window
     * event occurs.
     */

    virtual MRESULT dispatch( ULONG msg, MPARAM mp1, MPARAM mp2 );

    /**
     * Dispatch all not catched events.
     *
     * A virtual method that provides default implementation for
     * all events that occur on directory selection dialog window.
     */

    virtual MRESULT dispatch_everything( ULONG msg, MPARAM mp1, MPARAM mp2 );

  private:

    FOCSELDIR m_seldir;

    HWND  m_owner;
    HWND  m_handle;

    static MRESULT _System dlg_proc( HWND, ULONG, MPARAM, MPARAM );
};

#endif
