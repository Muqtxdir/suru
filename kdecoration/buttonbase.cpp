#include "buttonbase.h"
#include "surudecoration.h"

#include <KDecoration3/DecoratedWindow>

#include <QPainter>

namespace Suru
{

ButtonBase::ButtonBase(KDecoration3::DecorationButtonType type,
                       Decoration *decoration, QObject *parent)
    : KDecoration3::DecorationButton(type, decoration, parent)
{
    const int sz = decoration->buttonSize();
    setGeometry(QRectF(0, 0, sz, sz));
    setAcceptedButtons(Qt::LeftButton);
}

/* Color helpers */

KDecoration3::ColorGroup ButtonBase::colorGroup() const
{
    return decoration()->window()->isActive() ? KDecoration3::ColorGroup::Active
                                              : KDecoration3::ColorGroup::Inactive;
}

QColor ButtonBase::foregroundColor() const
{
    QColor c = decoration()->window()->color(colorGroup(), KDecoration3::ColorRole::Foreground);

    if (!isEnabled()) {
        c.setAlphaF(c.alphaF() * ButtonAlpha::DisabledFg);
    }

    return c;
}

QColor ButtonBase::backgroundColor() const
{
    QColor onSurface = decoration()->window()->color(colorGroup(), KDecoration3::ColorRole::Foreground);

    if (!isEnabled()) {
        onSurface.setAlphaF(ButtonAlpha::Disabled);
        return onSurface;
    }

    if (isPressed()) {
        onSurface.setAlphaF(ButtonAlpha::Pressed);
    } else if (isHovered()) {
        onSurface.setAlphaF(ButtonAlpha::Hovered);
    } else {
        onSurface.setAlphaF(ButtonAlpha::Normal);
    }

    return onSurface;
}

void ButtonBase::drawIcon(QPainter *, const QRectF &, const QRectF &) const
{
}

/*
 * Transform painter into an 18-unit coordinate space mapped to circleRect.
 * The pen width is divided by the scale factor so strokes render at the
 * same physical thickness as icons drawn in pixel coordinates.
 * Must be paired with painter->restore().
 */
void ButtonBase::beginCircleSpace(QPainter *painter, const QRectF &circleRect)
{
    painter->save();
    painter->translate(circleRect.topLeft());
    const qreal s = circleRect.width() / CircleSpace::Units;
    painter->scale(s, s);

    QPen pen = painter->pen();
    pen.setWidthF(pen.widthF() / s);
    painter->setPen(pen);
}

/* Main paint: computes geometry, draws circle background, then delegates icon */
void ButtonBase::paint(QPainter *painter, const QRectF &repaintArea)
{
    Q_UNUSED(repaintArea)
    if (!decoration()) {
        return;
    }

    auto *deco = static_cast<Decoration *>(decoration());
    const int btnSize = deco->buttonSize();

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);

    const QRectF geo = geometry();
    painter->translate(geo.topLeft());

    /* Circle and stroke rects, centered in geometry (handles Fitts extension) */
    const qreal cx = geo.width() / 2.0;
    const qreal cy = geo.height() / 2.0;

    const qreal circleSize = qRound(btnSize * ButtonDimensions::CircleRatio);
    const QRectF circleRect(cx - circleSize / 2.0, cy - circleSize / 2.0,
                            circleSize, circleSize);

    const qreal strokeSpan = qRound(btnSize * ButtonDimensions::StrokeRatio);
    const QRectF strokeRect(cx - strokeSpan / 2.0, cy - strokeSpan / 2.0,
                            strokeSpan, strokeSpan);

    /* Circle background */
    const ButtonStyle style = buttonStyle();
    const bool showCircle =
        (style == ButtonStyle::AlwaysCircle) ||
        (style == ButtonStyle::CircleOnHover && (isHovered() || isPressed()));

    if (showCircle) {
        painter->setPen(Qt::NoPen);
        painter->setBrush(backgroundColor());
        painter->drawEllipse(circleRect);
    }

    /* Icon stroke */
    const QColor fg = foregroundColor();
    if (fg.isValid()) {
        QPen pen(fg);
        pen.setCapStyle(Qt::SquareCap);
        pen.setJoinStyle(Qt::MiterJoin);
        pen.setWidthF(PenWidth::Symbol * btnSize / PenWidth::Reference);

        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);

        drawIcon(painter, circleRect, strokeRect);
    }

    painter->restore();
}

} // namespace Suru
