#pragma once

#include <memory>

#include <QImage>

namespace KDecoration3 { class DecorationShadow; }

namespace Suru
{

class ShadowProvider
{
public:
    ShadowProvider() = delete;

    static std::shared_ptr<KDecoration3::DecorationShadow> create(int scaledRadius);
    static void invalidateCache();

private:
    static void boxBlurH(const QImage &src, QImage &dst, int radius);
    static void boxBlurV(const QImage &src, QImage &dst, int radius);
    static void boxBlur(QImage &image, int radius);

    static std::shared_ptr<KDecoration3::DecorationShadow> s_cache;
    static int s_cacheRadius;
};

} // namespace Suru
