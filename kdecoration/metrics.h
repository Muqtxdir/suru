/*
 * Design tokens for the Suru window decoration.
 *
 * Button size = gridUnit * Multiplier, so everything scales with the
 * system font / DPI.  At gridUnit=10 the button is 24px, circle 22px,
 * icon stroke area 8px — all integers for crisp rendering.
 */

#pragma once

#include <QtGlobal>

namespace Suru
{

/* Foundation radii */
inline constexpr qreal BaseRadius   = 3.0;
inline constexpr qreal WindowRadius = BaseRadius + 1.0;

/* Visual style categories for buttons */
enum class ButtonStyle {
    AlwaysCircle,   /* Close, Maximize, Minimize */
    CircleOnHover,  /* OnAllDesktops, ApplicationMenu, KeepAbove, KeepBelow, Shade */
    IconOnly,       /* Menu (app icon, no circle) */
};

/* Button sizing ratios (24px button, 22px circle, 8px stroke at gridUnit=10) */
namespace ButtonDimensions {
    inline constexpr qreal Multiplier  = 2.4;
    inline constexpr qreal CircleRatio = 11.0 / 12.0;
    inline constexpr qreal StrokeRatio = 1.0 / 3.0;
}

/* Titlebar spacing */
struct TitlebarMetrics {
    int topMargin;
    int bottomMargin;
    int sideMargin;
    int buttonSpacing;
};

namespace Titlebar {
    inline constexpr TitlebarMetrics Desktop{2, 2, 2, 2};
    inline constexpr TitlebarMetrics Tablet {2, 2, 2, 2};
}

/* Button-specific constants */
namespace Button {
    inline constexpr qreal MenuIconScale = 0.55;
    inline constexpr qreal RestoreGap    = 2.0;
}

/* Logical coordinate space for non-circle button icons */
namespace CircleSpace {
    inline constexpr qreal Units = 18.0;
}

/*
 * Pen width for icon strokes.
 * Symbol is the target stroke width at the Reference button size.
 * paint() scales it proportionally: Symbol * btnSize / Reference,
 * so strokes look identical across all DPI / scale / gridUnit values.
 */
namespace PenWidth {
    inline constexpr qreal Symbol    = 1.01; // Copied from Breeze
    inline constexpr qreal Reference = 24.0;
}

/* Shadow animation timing */
namespace Animation {
    inline constexpr int ShadowDuration = 100;
}

/* Button state alpha multipliers applied to foreground color */
namespace ButtonAlpha {
    inline constexpr qreal Normal       = 0.1;
    inline constexpr qreal Hovered      = 0.15;
    inline constexpr qreal Pressed      = 0.2;
    inline constexpr qreal Disabled     = 0.05;
    inline constexpr qreal DisabledFg   = 0.5;
}

/* Two-layer composite shadow parameters */
namespace Shadow {
    inline constexpr int Padding        = 48;
    inline constexpr int Layer1YOffset  = 12;
    inline constexpr int Layer1BlurDiv  = 3;
    inline constexpr qreal Layer1Alpha  = 0.8;
    inline constexpr int Layer2YOffset  = -4;
    inline constexpr int Layer2BlurDiv  = 6;
    inline constexpr qreal Layer2Alpha  = 0.35;
    inline constexpr int MaskInset      = 2;
}

} // namespace Suru
