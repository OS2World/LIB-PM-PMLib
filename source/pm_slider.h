/*
 * Copyright (C) 2015 Dmitry A.Steklenev
 */

#ifndef PM_SLIDER_H
#define PM_SLIDER_H

#include "pm_os2.h"
#include "pm_window.h"
#include "pm_initslider.h"

/**
 * Linear slider control window class.
 *
 * The PMSlider class creates and manages slider control windows.
 * A slider control is a visual component that displays a range of
 * values and allows a user to set, display, or modify a value by
 * moving a slider arm. The linear slider is represented as a shaft
 * along which the slider arm can be moved by the user to set
 * a value.
 *
 * You can construct and destruct objects of this class.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

class PMSlider : public PMWindow
{
  friend class PMSliderInit;

  public:

    /**
     * Wraps the slider control window object around
     * an existing presentation window handle.
     *
     * The wrapped window object usually have a limited functionality.
     * You can query and set the window attributes, size, position
     * and visibility status.
     */

    PMSlider( HWND handle );

    /**
     * Wraps the slider control window object around
     * an existing child window with the specified identity.
     *
     * The wrapped window object usually have a limited functionality.
     * You can query and set the window attributes, size, position
     * and visibility status.
     *
     * @param hparent     Specifies the parent-window handle.
     * @param id          Specifies the identity of the child window.
     */

    PMSlider( HWND hparent, SHORT id );

    /**
     * Constructs the slider control window.
     *
     * @param id          Specifies the window identifier.
     * @param hparent     Specifies the parent-window handle.
     * @param x,y,cx,cy   Specifies the window shape.
     * @param scale1      Scale increments. The number of increments to set
     *                    for the slider control. This number represents the
     *                    range of values that can be selected within the
     *                    slider when the SLS_PRIMARYSCALE1 style bit is
     *                    specified.
     * @param scale2      Alternate scale increments. An alternate number of
     *                    increments to set for the slider control. This number
     *                    represents the range of values that can be selected
     *                    within the slider when the SLS_PRIMARYSCALE2 style bit
     *                    is specified.
     * @param style       Specifies the window style:
     *
     * <dl>
     * <dt><i>SLS_BOTTOM         </i><dd>Positions the slider at the bottom of
     *                                   the slider window. Valid only for
     *                                   horizontal sliders.
     * <dt><i>SLS_BUTTONSBOTTOM  </i><dd>Specifies that the optional slider
     *                                   buttons are to be used and places them
     *                                   at the bottom of the slider shaft. The
     *                                   buttons move the slider arm by one
     *                                   position, up or down, in the direction
     *                                   selected. Valid only for vertical
     *                                   sliders.
     * <dt><i>SLS_BUTTONSLEFT    </i><dd>Specifies that the optional slider
     *                                   buttons are to be used and places them
     *                                   to the left of the slider shaft. The
     *                                   buttons move the slider arm by one
     *                                   position, left or right, in the
     *                                   direction selected. Valid only for
     *                                   horizontal sliders.
     * <dt><i>SLS_BUTTONSRIGHT   </i><dd>Specifies that the optional slider
     *                                   buttons are to be used and places them
     *                                   to the right of the slider shaft. The
     *                                   buttons move the slider arm by one
     *                                   position, left or right, in the
     *                                   direction selected. Valid only for
     *                                   horizontal sliders.
     * <dt><i>SLS_BUTTONSTOP     </i><dd>Specifies that the optional slider
     *                                   buttons are to be used and places them
     *                                   at the top of the slider shaft. The
     *                                   buttons move the slider arm by one
     *                                   position, up or down, in the direction
     *                                   selected. Valid only for vertical
     *                                   sliders.
     * <dt><i>SLS_CENTER         </i><dd>Centers the slider within the slider
     *                                   window. This is the default position
     *                                   of the slider.
     * <dt><i>SLS_HOMEBOTTOM     </i><dd>Specifies the slider arm's home
     *                                   position. The bottom of the slider is
     *                                   used as the base value for
     *                                   incrementing. Valid only for vertical
     *                                   sliders.
     * <dt><i>SLS_HOMELEFT       </i><dd>Specifies the slider arm's home
     *                                   position. The left edge is used as the
     *                                   base value for incrementing. This is
     *                                   the default for horizontal sliders and
     *                                   is valid only for horizontal sliders.
     * <dt><i>SLS_HOMERIGHT      </i><dd>Specifies the slider arm's home
     *                                   position. The right edge is used as
     *                                   the base value for incrementing. Valid
     *                                   only for horizontal sliders.
     * <dt><i>SLS_HOMETOP        </i><dd>Specifies the slider arm's home
     *                                   position. The top of the slider is
     *                                   used as the base value for
     *                                   incrementing. Valid only for vertical
     *                                   sliders.
     * <dt><i>SLS_HORIZONTAL     </i><dd>Positions the slider horizontally. The
     *                                   slider arm can move left and right on
     *                                   the slider shaft. A scale can be
     *                                   placed on top of the slider shaft,
     *                                   below the slider shaft, or in both
     *                                   places. This is the default
     *                                   orientation of the slider.
     * <dt><i>SLS_LEFT           </i><dd>Positions the slider at the left edge
     *                                   of the slider window. Valid only for
     *                                   vertical sliders.
     * <dt><i>SLS_OWNERDRAW      </i><dd>Notifies the application whenever the
     *                                   slider shaft, the ribbon strip, the
     *                                   slider arm, and the slider background
     *                                   are to be drawn.
     * <dt><i>SLS_PRIMARYSCALE1  </i><dd>Determines the location of the scale
     *                                   on the slider shaft by using increment
     *                                   and spacing specified for scale 1 as
     *                                   the incremental value for positioning
     *                                   the slider arm. Scale 1 is displayed
     *                                   above the slider shaft of a horizontal
     *                                   slider and to the right of the slider
     *                                   shaft of a vertical slider. This is
     *                                   the default for a slider.
     * <dt><i>SLS_PRIMARYSCALE2  </i><dd>Determines the location of the scale
     *                                   on the slider shaft by using increment
     *                                   and spacing specified for scale 2 as
     *                                   the incremental value for positioning
     *                                   the slider arm. Scale 2 is displayed
     *                                   below the slider shaft of a horizontal
     *                                   slider and to the left of the slider
     *                                   shaft of a vertical slider.
     * <dt><i>SLS_READONLY       </i><dd>Creates a read-only slider, which
     *                                   presents information to the user but
     *                                   allows no interaction with the user.
     * <dt><i>SLS_RIBBONSTRIP    </i><dd>Fills, as the slider arm moves, the
     *                                   slider shaft between the home position
     *                                   and the slider arm with a color value
     *                                   different from slider shaft color,
     *                                   similar to mercury in a thermometer.
     * <dt><i>SLS_RIGHT          </i><dd>Positions the slider at the right edge
     *                                   of the slider window. Valid only for
     *                                   vertical sliders.
     * <dt><i>SLS_SNAPTOINCREMENT</i><dd>Causes the slider arm, when positioned
     *                                   between two values, to be positioned
     *                                   to the nearest value and redrawn at
     *                                   that position.
     * <dt><i>SLS_TOP            </i><dd>Positions the slider at the top of the
     *                                   slider window. Valid only for
     *                                   horizontal sliders.
     * <dt><i>SLS_VERTICAL       </i><dd>Positions the slider vertically. The
     *                                   slider arm can move up and down the
     *                                   slider shaft. A scale can be placed on
     *                                   the left side of theLTNR 
     * </dl>
     *
     * @see PMWindow::PMWindow 2
     */

    PMSlider( SHORT id, HWND hparent, LONG x, LONG y, LONG cx, LONG cy,
                             USHORT scale1, USHORT scale2, ULONG style );
    /**
     * Sets the size of a tick mark for the primary scale.
     *
     * All tick marks are initially set to a size of 0 (invisible).
     * Each tick mark along a scale can be set to the size desired.
     *
     * @param location    Tick mark location whose size is to be changed. If
     *                    the <i>SMA_SETALLTICKS</i> attribute is specified
     *                    for this parameter, all tick marks on the primary scale
     *                    are set to the size specified.
     * @param size        Length of the tick mark, in pixels. If set to 0, the
     *                    tick mark will not be drawn.
     */

    virtual void tick_mark( SHORT size, SHORT location = SMA_SETALLTICKS );

    /**
     * Returns scale increments.
     *
     * The number of increments to set for the slider control. This number
     * represents the range of values that can be selected within the
     * slider when the SLS_PRIMARYSCALE1 style bit is specified.
     */

    virtual USHORT scale1() const;

    /**
     * Returns alternate scale increments.
     *
     * An alternate number of increments to set for the slider control.
     * This number represents the range of values that can be selected
     * within the slider when the SLS_PRIMARYSCALE2 style bit is specified.
     */

    virtual USHORT scale2() const;

    /**
     * Returns the slider arm pos.
     *
     * The value returned represents an increment position using the primary scale.
     */

    virtual USHORT arm_pos() const;

    /**
     * Moves the slider arm to the specified pos.
     *
     * @param pos     Represents an increment position using the primary scale.
     */

    virtual void arm_to( USHORT pos );

  private:

    static MRESULT _System def_proc( HWND, ULONG, MPARAM, MPARAM );
    static PFNWP   m_default_handler;

    SLDCDATA m_cdata;
};

#endif
