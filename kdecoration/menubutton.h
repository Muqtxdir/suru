#pragma once

#include "buttonbase.h"

namespace Suru
{

class MenuButton final : public ButtonBase
{
    Q_OBJECT

public:
    explicit MenuButton(Decoration *decoration, QObject *parent = nullptr);

    void paint(QPainter *painter, const QRectF &repaintArea) override;

protected:
    ButtonStyle buttonStyle() const override { return ButtonStyle::IconOnly; }
};

} // namespace Suru
