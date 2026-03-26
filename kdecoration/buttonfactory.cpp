#include "buttonfactory.h"
#include "surudecoration.h"

#include "closebutton.h"
#include "maximizebutton.h"
#include "minimizebutton.h"
#include "menubutton.h"
#include "onalldesktopsbutton.h"
#include "keepabovebutton.h"
#include "keepbelowbutton.h"
#include "shadebutton.h"
#include "applicationmenubutton.h"
#include "contexthelpbutton.h"

#include <KDecoration3/DecoratedWindow>

namespace Suru
{

using BType = KDecoration3::DecorationButtonType;
using DW = KDecoration3::DecoratedWindow;

KDecoration3::DecorationButton *createButton(
    KDecoration3::DecorationButtonType type,
    KDecoration3::Decoration *decoration,
    QObject *parent)
{
    auto *d = qobject_cast<Decoration *>(decoration);
    if (!d) {
        return nullptr;
    }

    auto *w = d->window();
    ButtonBase *b = nullptr;

    switch (type) {
    case BType::Close:
        b = new CloseButton(d, parent);
        b->setVisible(w->isCloseable());
        QObject::connect(w, &DW::closeableChanged, b, &ButtonBase::setVisible);
        break;
    case BType::Maximize:
        b = new MaximizeButton(d, parent);
        b->setVisible(w->isMaximizeable());
        QObject::connect(w, &DW::maximizeableChanged, b, &ButtonBase::setVisible);
        break;
    case BType::Minimize:
        b = new MinimizeButton(d, parent);
        b->setVisible(w->isMinimizeable());
        QObject::connect(w, &DW::minimizeableChanged, b, &ButtonBase::setVisible);
        break;
    case BType::Shade:
        b = new ShadeButton(d, parent);
        b->setVisible(w->isShadeable());
        QObject::connect(w, &DW::shadeableChanged, b, &ButtonBase::setVisible);
        break;
    case BType::ContextHelp:
        b = new ContextHelpButton(d, parent);
        b->setVisible(w->providesContextHelp());
        QObject::connect(w, &DW::providesContextHelpChanged, b, &ButtonBase::setVisible);
        break;
    case BType::Menu:
        b = new MenuButton(d, parent);
        b->setVisible(true);
        break;
    case BType::OnAllDesktops:
        b = new OnAllDesktopsButton(d, parent);
        b->setVisible(true);
        break;
    case BType::KeepAbove:
        b = new KeepAboveButton(d, parent);
        b->setVisible(true);
        break;
    case BType::KeepBelow:
        b = new KeepBelowButton(d, parent);
        b->setVisible(true);
        break;
    case BType::ApplicationMenu:
        b = new ApplicationMenuButton(d, parent);
        b->setVisible(true);
        break;
    default:
        return nullptr;
    }

    return b;
}

} // namespace Suru
