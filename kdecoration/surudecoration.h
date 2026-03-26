#pragma once

#include "metrics.h"

#include <KDecoration3/Decoration>

class QVariantAnimation;

namespace KDecoration3
{
class DecorationButtonGroup;
}

namespace Suru
{

class Decoration : public KDecoration3::Decoration
{
    Q_OBJECT

public:
    explicit Decoration(QObject *parent, const QVariantList &args);
    ~Decoration() override = default;

    void paint(QPainter *painter, const QRectF &repaintArea) override;
    bool init() override;

    int titleBarHeight() const { return m_titleBarHeight; }
    int buttonSize() const;
    bool isMaximized() const;

    /* Edge detection (Fitts's law) */
    bool isEdge(Qt::Edge edge) const;
    bool isLeftEdge() const   { return isEdge(Qt::LeftEdge); }
    bool isRightEdge() const  { return isEdge(Qt::RightEdge); }
    bool isTopEdge() const    { return isEdge(Qt::TopEdge); }
    bool isBottomEdge() const { return isEdge(Qt::BottomEdge); }

private Q_SLOTS:
    void onTabletModeChanged(bool mode);

private:
    void initShadowAnimation();
    void initButtonGroups();
    void connectSignals();
    void initTabletMode();

    void reconfigure();
    void recalculateBorders();
    void updateButtonsGeometry();
    void updateButtonPositions();
    void updateShadow();
    void updateScaledCornerRadius();
    void updateAnimationState();
    void updateScale();
    void updateLayout();

    void paintTitleBar(QPainter *painter, const QRectF &repaintArea);

    int m_titleBarHeight = 36;
    qreal m_scaledCornerRadius = 15;

    KDecoration3::DecorationButtonGroup *m_leftButtons = nullptr;
    KDecoration3::DecorationButtonGroup *m_rightButtons = nullptr;

    /* Active state transition animation (shadow) */
    QVariantAnimation *m_shadowAnimation = nullptr;

    bool m_tabletMode = false;

    /* Metric strategy, swapped on tablet mode change */
    const TitlebarMetrics *m_metrics = &Titlebar::Desktop;
};

} // namespace Suru
