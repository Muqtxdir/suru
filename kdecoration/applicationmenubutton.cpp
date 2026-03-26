#include "applicationmenubutton.h"
#include "surudecoration.h"

#include <QPainter>

namespace Suru
{

ApplicationMenuButton::ApplicationMenuButton(Decoration *decoration, QObject *parent)
    : ButtonBase(KDecoration3::DecorationButtonType::ApplicationMenu, decoration, parent)
{
}

void ApplicationMenuButton::drawIcon(QPainter *painter,
                                     const QRectF &circleRect,
                                     const QRectF &strokeRect) const
{
    Q_UNUSED(strokeRect)
    beginCircleSpace(painter, circleRect);
    painter->drawLine(QPointF(3.5, 5), QPointF(14.5, 5));
    painter->drawLine(QPointF(3.5, 9), QPointF(14.5, 9));
    painter->drawLine(QPointF(3.5, 13), QPointF(14.5, 13));
    painter->restore();
}

} // namespace Suru
