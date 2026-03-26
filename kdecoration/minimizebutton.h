#pragma once

#include "buttonbase.h"

namespace Suru
{

class MinimizeButton final : public ButtonBase
{
    Q_OBJECT

public:
    explicit MinimizeButton(Decoration *decoration, QObject *parent = nullptr);

protected:
    ButtonStyle buttonStyle() const override { return ButtonStyle::AlwaysCircle; }
    void drawIcon(QPainter *painter, const QRectF &circleRect, const QRectF &strokeRect) const override;
};

} // namespace Suru
