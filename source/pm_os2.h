/*
 * Copyright (C) 2011-2016 Dmitry A.Steklenev
 */

#ifndef PM_OS2_H
#define PM_OS2_H

#define INCL_BASE
#define INCL_DOSDEVIOCTL
#define INCL_WIN
#define INCL_GPIPRIMITIVES
#define INCL_GPILCIDS
#define INCL_GPIBITMAPS
#define INCL_GPILOGCOLORTABLE
#define INCL_ERRORS
#define INCL_LONGLONG
#define INCL_SHLERRORS
#define INCL_GPIERRORS
#define INCL_SPLERRORS
#include <os2.h>

#define PM_BASE           (0xFF00   )
#define PM_SETFOCUS       (PM_BASE+0) /* Replacement of the WM_SETFOCUS for safe     */
                                      /* focus change during the message processing. */
#define PM_TEXTEDIT       (PM_BASE+1) /* Begin direct name edit.                     */
#define PM_FORMATFRAME    (PM_BASE+2) /* OS/2 requires formatting the frame window   */
                                      /* after processing WM_FORMATFRAME.            */
#define PM_THREAD_END     (PM_BASE+3) /* Notify parent thread about termination of   */
                                      /* the specified thread.                       */
#define PM_SETEXTENSION   (PM_BASE+4) /* Attaches a window as a window extension.    */
#define PM_SETACCELTABLE  (PM_BASE+5) /* Establishes the window accelerator table.   */
#define PM_GETEXTENSION   (PM_BASE+6) /* Queries a window extension.                 */
#define PM_SETTEXT        (PM_BASE+7) /* Sets a window text.                         */
#define PM_QUERYOBJECT    (PM_BASE+8) /* Queries a window object.                    */
#define PM_ALIGN          (PM_BASE+9) /* Sets aligment for window.                   */

/* Additional dialog codes: returned by WM_QUERYDLGCODE message */

#define DLGC_SPCANVAS     0x0800      /* Split canvas. */

/* PM_SETEXTENSION message parameter flags */

#define PMA_SE_ATTACH     0
#define PMA_SE_DETACH     1
#define PMA_SE_DETACH_ALL 2

/* PM_GETEXTENSION message parameter flags */

#define PMA_GE_FIRST     -1
#define PMA_GE_LAST      -2
#define PMA_GE_NEXT      -3
#define PMA_GE_PREV      -4

/* PM_ALIGN message parameter flags */

#define PMA_ALIGN_SET         0
#define PMA_ALIGN_REMOVE_ALL -1

/* Symbolic constants for I/O */

#define PM_STDIN  0
#define PM_STDOUT 1
#define PM_STDERR 2

/* Window classes */

#define PM_WINDOW "PMWindow"
#define PM_SLIDER "PMSlider"

#endif
