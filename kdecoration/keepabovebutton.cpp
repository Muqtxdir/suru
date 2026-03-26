#include "keepabovebutton.h"
#include "surudecoration.h"

#include <QPainter>

namespace Suru
{

KeepAboveButton::KeepAboveButton(Decoration *decoration, QObject *parent)
    : ButtonBase(KDecoration3::DecorationButtonType::KeepAbove, decoration, parent)
{
}

void KeepAboveButton::drawIcon(QPainter *painter,
                               const QRectF &circleRect,
                               const QRectF &strokeRect) const
{
    Q_UNUSED(strokeRect)
    beginCircleSpace(painter, circleRect);
    /* Double upward chevrons */
    painter->drawPolyline(QVector<QPointF>{
        QPointF(4, 9), QPointF(9, 4), QPointF(14, 9)});
    painter->drawPolyline(QVector<QPointF>{
        QPointF(4, 13), QPointF(9, 8), QPointF(14, 13)});
    painter->restore();
}

} // namespace Suru
