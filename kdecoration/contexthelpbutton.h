#pragma once

#include "buttonbase.h"

namespace Suru
{

class ContextHelpButton final : public ButtonBase
{
    Q_OBJECT

public:
    explicit ContextHelpButton(Decoration *decoration, QObject *parent = nullptr);

protected:
    void drawIcon(QPainter *painter, const QRectF &circleRect, const QRectF &strokeRect) const override;
};

} // namespace Suru
