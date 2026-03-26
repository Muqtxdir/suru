#include "contexthelpbutton.h"
#include "surudecoration.h"

#include <QPainter>
#include <QPainterPath>

namespace Suru
{

ContextHelpButton::ContextHelpButton(Decoration *decoration, QObject *parent)
    : ButtonBase(KDecoration3::DecorationButtonType::ContextHelp, decoration, parent)
{
}

void ContextHelpButton::drawIcon(QPainter *painter,
                                 const QRectF &circleRect,
                                 const QRectF &strokeRect) const
{
    Q_UNUSED(strokeRect)
    beginCircleSpace(painter, circleRect);

    QPen pen = painter->pen();
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter->setPen(pen);

    QPainterPath path;
    path.moveTo(6, 6);
    path.cubicTo(6, 3.5, 12, 3.5, 12, 6);
    path.cubicTo(12, 8, 9, 8, 9, 10.5);
    painter->drawPath(path);

    painter->setPen(Qt::NoPen);
    painter->setBrush(foregroundColor());
    painter->drawEllipse(QPointF(9, 13), 1.0, 1.0);

    painter->restore();
}

} // namespace Suru
