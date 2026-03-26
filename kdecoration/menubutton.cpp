/* Menu button renders the application icon, no circle, no custom icon path. */

#include "menubutton.h"
#include "surudecoration.h"

#include <KDecoration3/DecoratedWindow>

#include <QPainter>

namespace Suru
{

MenuButton::MenuButton(Decoration *decoration, QObject *parent)
    : ButtonBase(KDecoration3::DecorationButtonType::Menu, decoration, parent)
{
    setAcceptedButtons(Qt::LeftButton | Qt::RightButton);
}

void MenuButton::paint(QPainter *painter, const QRectF &repaintArea)
{
    Q_UNUSED(repaintArea)
    if (!decoration()) {
        return;
    }

    painter->save();

    const qreal sz = geometry().width() * Button::MenuIconScale;
    const QRectF iconRect(
        geometry().center().x() - sz / 2.0,
        geometry().center().y() - sz / 2.0,
        sz, sz);
    decoration()->window()->icon().paint(painter, iconRect.toRect());

    painter->restore();
}

} // namespace Suru
