#include "keepbelowbutton.h"
#include "surudecoration.h"

#include <QPainter>

namespace Suru
{

KeepBelowButton::KeepBelowButton(Decoration *decoration, QObject *parent)
    : ButtonBase(KDecoration3::DecorationButtonType::KeepBelow, decoration, parent)
{
}

void KeepBelowButton::drawIcon(QPainter *painter,
                               const QRectF &circleRect,
                               const QRectF &strokeRect) const
{
    Q_UNUSED(strokeRect)
    beginCircleSpace(painter, circleRect);
    /* Double downward chevrons */
    painter->drawPolyline(QVector<QPointF>{
        QPointF(4, 5), QPointF(9, 10), QPointF(14, 5)});
    painter->drawPolyline(QVector<QPointF>{
        QPointF(4, 9), QPointF(9, 14), QPointF(14, 9)});
    painter->restore();
}

} // namespace Suru
