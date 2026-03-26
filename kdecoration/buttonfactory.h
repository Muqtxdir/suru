/*
 * Factory function for creating Suru decoration buttons.
 * Matches the signature expected by KDecoration3::DecorationButtonGroup.
 */

#pragma once

#include <KDecoration3/DecorationButton>

namespace KDecoration3 { class Decoration; }

namespace Suru
{

KDecoration3::DecorationButton *createButton(
    KDecoration3::DecorationButtonType type,
    KDecoration3::Decoration *decoration,
    QObject *parent);

} // namespace Suru
