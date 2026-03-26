#include "minimizebutton.h"
#include "surudecoration.h"

#include <QPainter>

namespace Suru
{

MinimizeButton::MinimizeButton(Decoration *decoration, QObject *parent)
    : ButtonBase(KDecoration3::DecorationButtonType::Minimize, decoration, parent)
{
}

void MinimizeButton::drawIcon(QPainter *painter,
                              const QRectF &circleRect,
                              const QRectF &strokeRect) const
{
    Q_UNUSED(circleRect)
    painter->drawLine(QPointF(strokeRect.left(), strokeRect.bottom()),
                      QPointF(strokeRect.right(), strokeRect.bottom()));
}

} // namespace Suru
