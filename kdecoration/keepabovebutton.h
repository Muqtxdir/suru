#pragma once

#include "buttonbase.h"

namespace Suru
{

class KeepAboveButton final : public ButtonBase
{
    Q_OBJECT

public:
    explicit KeepAboveButton(Decoration *decoration, QObject *parent = nullptr);

protected:
    void drawIcon(QPainter *painter, const QRectF &circleRect, const QRectF &strokeRect) const override;
};

} // namespace Suru
