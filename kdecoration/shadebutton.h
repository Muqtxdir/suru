#pragma once

#include "buttonbase.h"

namespace Suru
{

class ShadeButton final : public ButtonBase
{
    Q_OBJECT

public:
    explicit ShadeButton(Decoration *decoration, QObject *parent = nullptr);

protected:
    void drawIcon(QPainter *painter, const QRectF &circleRect, const QRectF &strokeRect) const override;
};

} // namespace Suru
