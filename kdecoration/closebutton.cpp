#include "closebutton.h"
#include "surudecoration.h"

#include <QPainter>

namespace Suru
{

CloseButton::CloseButton(Decoration *decoration, QObject *parent)
    : ButtonBase(KDecoration3::DecorationButtonType::Close, decoration, parent)
{
}

void CloseButton::drawIcon(QPainter *painter,
                           const QRectF &circleRect,
                           const QRectF &strokeRect) const
{
    Q_UNUSED(circleRect)
    painter->drawLine(strokeRect.topLeft(), strokeRect.bottomRight());
    painter->drawLine(strokeRect.topRight(), strokeRect.bottomLeft());
}

} // namespace Suru
