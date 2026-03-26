#include "onalldesktopsbutton.h"
#include "surudecoration.h"

#include <KDecoration3/DecoratedWindow>

#include <QPainter>

namespace Suru
{

OnAllDesktopsButton::OnAllDesktopsButton(Decoration *decoration, QObject *parent)
    : ButtonBase(KDecoration3::DecorationButtonType::OnAllDesktops, decoration, parent)
{
}

void OnAllDesktopsButton::drawIcon(QPainter *painter,
                                   const QRectF &circleRect,
                                   const QRectF &strokeRect) const
{
    Q_UNUSED(strokeRect)
    beginCircleSpace(painter, circleRect);

    /* Save the scaled pen before switching to fill mode */
    const QPen scaledPen = painter->pen();

    painter->setPen(Qt::NoPen);
    painter->setBrush(foregroundColor());

    if (isChecked()) {
        /* Pinned: filled ring with center dot */
        painter->drawEllipse(QRectF(3, 3, 12, 12));

        QColor bg = backgroundColor();
        if (!bg.isValid()) {
            bg = decoration()->window()->color(colorGroup(),
                KDecoration3::ColorRole::TitleBar);
        }
        painter->setBrush(bg);
        painter->drawEllipse(QRectF(8, 8, 2, 2));
    } else {
        /* Unpinned: pin polygon + crossing lines */
        painter->drawPolygon(QVector<QPointF>{
            QPointF(6.5, 8.5), QPointF(12, 3), QPointF(15, 6), QPointF(9.5, 11.5)});

        QPen pen = scaledPen;
        pen.setCapStyle(Qt::RoundCap);
        painter->setPen(pen);
        painter->drawLine(QPointF(5.5, 7.5), QPointF(10.5, 12.5));
        painter->drawLine(QPointF(12, 6), QPointF(4.5, 13.5));
    }

    painter->restore();
}

} // namespace Suru
