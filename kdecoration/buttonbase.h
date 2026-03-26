/*
 * Abstract base for all Suru window-decoration buttons.
 *
 * paint() computes circle and stroke rects from ButtonDimensions ratios,
 * then delegates icon drawing to subclasses via drawIcon().
 */

#pragma once

#include "metrics.h"

#include <KDecoration3/DecorationButton>

class QPainter;

namespace Suru
{

class Decoration;

class ButtonBase : public KDecoration3::DecorationButton
{
    Q_OBJECT

public:
    explicit ButtonBase(KDecoration3::DecorationButtonType type,
                        Decoration *decoration, QObject *parent = nullptr);
    ~ButtonBase() override = default;

    void paint(QPainter *painter, const QRectF &repaintArea) override;

protected:
    virtual ButtonStyle buttonStyle() const { return ButtonStyle::CircleOnHover; }

    /*
     * Draw icon. Painter origin is at button top-left.
     * circleRect: the circle background area, centered in the button.
     * strokeRect: the icon drawing area, centered in the button.
     * Default implementation is empty (for buttons that override paint entirely).
     */
    virtual void drawIcon(QPainter *painter,
                          const QRectF &circleRect,
                          const QRectF &strokeRect) const;

    /* Transform into 18-unit coordinate space mapped to circleRect.
     * Must be paired with painter->restore(). */
    static void beginCircleSpace(QPainter *painter, const QRectF &circleRect);

    KDecoration3::ColorGroup colorGroup() const;
    QColor foregroundColor() const;
    QColor backgroundColor() const;
};

} // namespace Suru
