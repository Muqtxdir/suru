#include "shadebutton.h"
#include "surudecoration.h"

#include <QPainter>

namespace Suru
{

ShadeButton::ShadeButton(Decoration *decoration, QObject *parent)
    : ButtonBase(KDecoration3::DecorationButtonType::Shade, decoration, parent)
{
}

void ShadeButton::drawIcon(QPainter *painter,
                           const QRectF &circleRect,
                           const QRectF &strokeRect) const
{
    Q_UNUSED(strokeRect)
    beginCircleSpace(painter, circleRect);

    if (isChecked()) {
        /* Shaded: line + downward chevron */
        painter->drawLine(QPointF(4, 5.5), QPointF(14, 5.5));
        painter->drawPolyline(QVector<QPointF>{
            QPointF(4, 8), QPointF(9, 13), QPointF(14, 8)});
    } else {
        /* Unshaded: line + upward chevron */
        painter->drawLine(QPointF(4, 5.5), QPointF(14, 5.5));
        painter->drawPolyline(QVector<QPointF>{
            QPointF(4, 13), QPointF(9, 8), QPointF(14, 13)});
    }

    painter->restore();
}

} // namespace Suru
