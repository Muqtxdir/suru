#include "surudecoration.h"
#include "buttonfactory.h"
#include "metrics.h"
#include "shadowprovider.h"

#include <KDecoration3/DecoratedWindow>
#include <KDecoration3/DecorationButtonGroup>
#include <KDecoration3/DecorationSettings>
#include <KDecoration3/DecorationShadow>
#include <kdecoration3/scalehelpers.h>

#include <KPluginFactory>

#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusPendingCallWatcher>
#include <QDBusPendingReply>
#include <QPainter>
#include <QPainterPath>
#include <QVariantAnimation>

K_PLUGIN_FACTORY_WITH_JSON(SuruDecoFactory, "suru.json",
    registerPlugin<Suru::Decoration>();)

namespace Suru
{

Decoration::Decoration(QObject *parent, const QVariantList &args)
    : KDecoration3::Decoration(parent, args)
    , m_shadowAnimation(new QVariantAnimation(this))
{
}

bool Decoration::isMaximized() const
{
    return window()->isMaximized();
}

bool Decoration::isEdge(Qt::Edge edge) const
{
    const bool horizontal = (edge == Qt::LeftEdge || edge == Qt::RightEdge);
    return (horizontal ? window()->isMaximizedHorizontally()
                       : window()->isMaximizedVertically())
        || window()->adjacentScreenEdges().testFlag(edge);
}

int Decoration::buttonSize() const
{
    const int gridUnit = m_tabletMode ? settings()->gridUnit() * 2
                                      : settings()->gridUnit();
    return qRound(gridUnit * ButtonDimensions::Multiplier);
}

/*
 * Common layout invalidation: recalculate borders, reposition buttons,
 * and schedule a repaint. Called from multiple signal handlers.
 */
void Decoration::updateLayout()
{
    recalculateBorders();
    updateButtonPositions();
    update();
}

/* Initialization */

bool Decoration::init()
{
    reconfigure();
    updateScaledCornerRadius();
    setOpaque(false);

    initShadowAnimation();
    initButtonGroups();

    recalculateBorders();
    updateButtonPositions();
    updateShadow();

    connectSignals();
    initTabletMode();

    return true;
}

void Decoration::initShadowAnimation()
{
    m_shadowAnimation->setStartValue(0.0);
    m_shadowAnimation->setEndValue(1.0);
    m_shadowAnimation->setEasingCurve(QEasingCurve::OutCubic);
    m_shadowAnimation->setDuration(Animation::ShadowDuration);
    connect(m_shadowAnimation, &QVariantAnimation::valueChanged, this, [this]() {
        updateShadow();
    });
}

void Decoration::initButtonGroups()
{
    const int sp = settings()->smallSpacing();

    m_leftButtons = new KDecoration3::DecorationButtonGroup(
        KDecoration3::DecorationButtonGroup::Position::Left, this,
        &Suru::createButton);
    m_rightButtons = new KDecoration3::DecorationButtonGroup(
        KDecoration3::DecorationButtonGroup::Position::Right, this,
        &Suru::createButton);

    m_leftButtons->setSpacing(sp * m_metrics->buttonSpacing);
    m_rightButtons->setSpacing(sp * m_metrics->buttonSpacing);
}

void Decoration::connectSignals()
{
    auto *w = window();
    auto *s = settings().get();

    connect(w, &KDecoration3::DecoratedWindow::activeChanged,
            this, [this]() {
                updateAnimationState();
                recalculateBorders();
            });
    connect(w, &KDecoration3::DecoratedWindow::captionChanged,
            this, [this]() { update(); });
    connect(w, &KDecoration3::DecoratedWindow::sizeChanged,
            this, &Decoration::updateLayout);
    connect(w, &KDecoration3::DecoratedWindow::maximizedChanged,
            this, [this]() {
                updateShadow();
                updateLayout();
            });
    connect(w, &KDecoration3::DecoratedWindow::adjacentScreenEdgesChanged,
            this, &Decoration::updateLayout);

    connect(w, &KDecoration3::DecoratedWindow::nextScaleChanged,
            this, &Decoration::updateScale);

    connect(s, &KDecoration3::DecorationSettings::reconfigured,
            this, [this]() {
                reconfigure();
                updateScaledCornerRadius();
                updateShadow();
                updateLayout();
            });
}

void Decoration::initTabletMode()
{
    auto dbus = QDBusConnection::sessionBus();
    dbus.connect(QStringLiteral("org.kde.KWin"),
                 QStringLiteral("/org/kde/KWin"),
                 QStringLiteral("org.kde.KWin.TabletModeManager"),
                 QStringLiteral("tabletModeChanged"),
                 QStringLiteral("b"),
                 this,
                 SLOT(onTabletModeChanged(bool)));

    auto message = QDBusMessage::createMethodCall(
        QStringLiteral("org.kde.KWin"),
        QStringLiteral("/org/kde/KWin"),
        QStringLiteral("org.freedesktop.DBus.Properties"),
        QStringLiteral("Get"));
    message.setArguments({QStringLiteral("org.kde.KWin.TabletModeManager"),
                          QStringLiteral("tabletMode")});
    auto call = new QDBusPendingCallWatcher(dbus.asyncCall(message), this);
    connect(call, &QDBusPendingCallWatcher::finished, this, [this, call]() {
        QDBusPendingReply<QVariant> reply = *call;
        if (!reply.isError()) {
            onTabletModeChanged(reply.value().toBool());
        }
        call->deleteLater();
    });
}

void Decoration::onTabletModeChanged(bool mode)
{
    m_tabletMode = mode;
    m_metrics = mode ? &Titlebar::Tablet : &Titlebar::Desktop;
    reconfigure();
    updateLayout();
}

void Decoration::updateAnimationState()
{
    const bool active = window()->isActive();

    if (m_shadowAnimation->duration() > 0) {
        m_shadowAnimation->setDirection(active ? QAbstractAnimation::Forward
                                               : QAbstractAnimation::Backward);
        m_shadowAnimation->setEasingCurve(active ? QEasingCurve::OutCubic
                                                  : QEasingCurve::InCubic);
        if (m_shadowAnimation->state() != QAbstractAnimation::Running) {
            m_shadowAnimation->start();
        }
    } else {
        updateShadow();
    }

    update();
}

void Decoration::updateScale()
{
    updateScaledCornerRadius();
    recalculateBorders();
}

void Decoration::reconfigure()
{
    const auto *s = settings().get();
    const int sp = s->smallSpacing();

    m_titleBarHeight = qMax(qRound(s->fontMetrics().height()), buttonSize())
                     + sp * m_metrics->topMargin
                     + sp * m_metrics->bottomMargin;
}

void Decoration::updateScaledCornerRadius()
{
    m_scaledCornerRadius = KDecoration3::snapToPixelGrid(
        WindowRadius * settings()->smallSpacing(),
        window()->nextScale());
}

void Decoration::recalculateBorders()
{
    const bool maximized = isMaximized();

    setBorders(QMarginsF(0, m_titleBarHeight, 0, 0));

    if (!maximized) {
        const qreal extSize = KDecoration3::snapToPixelGrid(
            settings()->largeSpacing(), window()->nextScale());
        setResizeOnlyBorders(QMarginsF(extSize, extSize, extSize, extSize));
    } else {
        setResizeOnlyBorders(QMarginsF());
    }

    setTitleBar(QRectF(0, 0, size().width(), m_titleBarHeight));

    /* Flatten corners touching a screen edge */
    const Qt::Edges edges = window()->adjacentScreenEdges();
    qreal tl = m_scaledCornerRadius;
    qreal tr = m_scaledCornerRadius;
    qreal bl = m_scaledCornerRadius;
    qreal br = m_scaledCornerRadius;

    if (maximized) {
        tl = tr = bl = br = 0;
    } else {
        if (edges & (Qt::TopEdge | Qt::LeftEdge))    tl = 0;
        if (edges & (Qt::TopEdge | Qt::RightEdge))   tr = 0;
        if (edges & (Qt::BottomEdge | Qt::LeftEdge))  bl = 0;
        if (edges & (Qt::BottomEdge | Qt::RightEdge)) br = 0;
    }

    setBorderRadius(KDecoration3::BorderRadius(tl, tr, bl, br));
}

void Decoration::updateButtonsGeometry()
{
    const int sz = buttonSize();
    const auto buttonList = m_leftButtons->buttons() + m_rightButtons->buttons();
    for (auto *button : buttonList) {
        button->setGeometry(QRectF(0, 0, sz, sz));
    }
}

void Decoration::updateButtonPositions()
{
    updateButtonsGeometry();

    const int sp = settings()->smallSpacing();
    const int sideM = sp * m_metrics->sideMargin;
    const int topM  = sp * m_metrics->topMargin;
    const int btnSp = sp * m_metrics->buttonSpacing;

    const int vPadding = isTopEdge() ? 0 : topM;
    const int hPadding = sideM;

    m_leftButtons->setSpacing(btnSp);
    m_rightButtons->setSpacing(btnSp);

    /* Left buttons: extend first button to screen edge for Fitts's law */
    if (!m_leftButtons->buttons().isEmpty()) {
        if (isLeftEdge()) {
            auto *button = m_leftButtons->buttons().front();
            QRectF geo = button->geometry();
            geo.adjust(-hPadding, 0, 0, 0);
            button->setGeometry(geo);

            m_leftButtons->setPos(QPointF(0, vPadding));
        } else {
            m_leftButtons->setPos(QPointF(hPadding, vPadding));
        }
    }

    /* Right buttons: extend last button to screen edge for Fitts's law */
    if (!m_rightButtons->buttons().isEmpty()) {
        if (isRightEdge()) {
            auto *button = m_rightButtons->buttons().back();
            QRectF geo = button->geometry();
            geo.adjust(0, 0, hPadding, 0);
            button->setGeometry(geo);

            m_rightButtons->setPos(QPointF(
                size().width() - m_rightButtons->geometry().width(), vPadding));
        } else {
            m_rightButtons->setPos(QPointF(
                size().width() - m_rightButtons->geometry().width() - hPadding, vPadding));
        }
    }
}

void Decoration::updateShadow()
{
    if (isMaximized()) {
        setShadow(std::shared_ptr<KDecoration3::DecorationShadow>());
    } else {
        setShadow(ShadowProvider::create(qRound(m_scaledCornerRadius)));
    }
}

/* Painting */

void Decoration::paint(QPainter *painter, const QRectF &repaintArea)
{
    paintTitleBar(painter, repaintArea);

    m_leftButtons->paint(painter, repaintArea);
    m_rightButtons->paint(painter, repaintArea);
}

void Decoration::paintTitleBar(QPainter *painter, const QRectF &repaintArea)
{
    const QRectF titleRect(0, 0, size().width(), m_titleBarHeight);

    if (!titleRect.intersects(repaintArea)) {
        return;
    }

    const auto *w = window();
    const auto *s = settings().get();
    const auto cg = w->isActive() ? KDecoration3::ColorGroup::Active
                                   : KDecoration3::ColorGroup::Inactive;

    /* Titlebar background */
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(Qt::NoPen);
    painter->setBrush(w->color(cg, KDecoration3::ColorRole::TitleBar));

    const auto corners = borderRadius();
    const bool hasRounding = corners.topLeft() > 0 || corners.topRight() > 0;

    if (hasRounding) {
        const qreal tl = qMin(corners.topLeft(), titleRect.height());
        const qreal tr = qMin(corners.topRight(), titleRect.height());

        QPainterPath path;
        path.moveTo(titleRect.bottomLeft());

        if (tl > 0) {
            path.lineTo(titleRect.left(), titleRect.top() + tl);
            path.arcTo(QRectF(titleRect.left(), titleRect.top(),
                               2 * tl, 2 * tl), 180, -90);
        } else {
            path.lineTo(titleRect.topLeft());
        }

        if (tr > 0) {
            path.lineTo(titleRect.right() - tr, titleRect.top());
            path.arcTo(QRectF(titleRect.right() - 2 * tr, titleRect.top(),
                               2 * tr, 2 * tr), 90, -90);
        } else {
            path.lineTo(titleRect.topRight());
        }

        path.lineTo(titleRect.bottomRight());
        path.closeSubpath();
        painter->drawPath(path);
    } else {
        painter->drawRect(titleRect);
    }
    painter->restore();

    /* Title text */
    const int sideM = s->smallSpacing() * m_metrics->sideMargin;
    const qreal leftEdge  = m_leftButtons->geometry().right() + sideM;
    const qreal rightEdge = m_rightButtons->geometry().left() - sideM;

    if (rightEdge > leftEdge) {
        painter->save();
        painter->setFont(s->font());
        painter->setPen(w->color(cg, KDecoration3::ColorRole::Foreground));

        const QRectF captionRect(leftEdge, 0,
                                  rightEdge - leftEdge, m_titleBarHeight);

        const QString caption = painter->fontMetrics().elidedText(
            w->caption(), Qt::ElideRight, captionRect.width());

        painter->drawText(captionRect,
                          Qt::AlignCenter | Qt::TextSingleLine, caption);
        painter->restore();
    }
}

} // namespace Suru

#include "surudecoration.moc"
