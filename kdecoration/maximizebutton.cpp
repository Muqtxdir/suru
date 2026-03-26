#include "maximizebutton.h"
#include "surudecoration.h"

#include <KDecoration3/DecoratedWindow>

#include <QPainter>

namespace Suru
{

MaximizeButton::MaximizeButton(Decoration *decoration, QObject *parent)
    : ButtonBase(KDecoration3::DecorationButtonType::Maximize, decoration, parent)
{
    setAcceptedButtons(Qt::LeftButton | Qt::MiddleButton | Qt::RightButton);
}

void MaximizeButton::drawIcon(QPainter *painter,
                              const QRectF &circleRect,
                              const QRectF &strokeRect) const
{
    Q_UNUSED(circleRect)

    if (isChecked()) {
        /* Restore: back L-shape + front rect */
        const qreal l = strokeRect.left();
        const qreal t = strokeRect.top();
        const qreal w = strokeRect.width();
        constexpr qreal gap = Button::RestoreGap;
        const qreal side = w - gap;

        /* Back rect: only top + right edges visible as L-shape */
        const QRectF backRect(l + gap, t, side, side);
        painter->drawLine(backRect.topLeft(), backRect.topRight());
        painter->drawLine(backRect.topRight(), backRect.bottomRight());

        const QRectF frontRect(l, t + gap, side, side);
        painter->drawRect(frontRect);
    } else {
        /* Maximize: square outline filling the stroke area */
        painter->drawRect(strokeRect);
    }
}

} // namespace Suru
