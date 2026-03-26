/*
 * Two-layer composite shadow:
 *   Layer 1: large soft ambient, offset down
 *   Layer 2: smaller tight shadow, slight upward offset
 */

#include "shadowprovider.h"
#include "metrics.h"

#include <KDecoration3/DecorationShadow>

#include <QPainter>
#include <QPainterPath>

namespace Suru
{

/* Static cache */
std::shared_ptr<KDecoration3::DecorationShadow> ShadowProvider::s_cache;
int ShadowProvider::s_cacheRadius = 0;

void ShadowProvider::invalidateCache()
{
    s_cache.reset();
    s_cacheRadius = 0;
}

/* Box blur: separable, two full H+V passes for tent-like falloff */

void ShadowProvider::boxBlurH(const QImage &src, QImage &dst, int radius)
{
    const int w = src.width();
    const int h = src.height();
    const int span = 2 * radius + 1;

    for (int y = 0; y < h; ++y) {
        const QRgb *sLine = reinterpret_cast<const QRgb *>(src.constScanLine(y));
        QRgb *dLine = reinterpret_cast<QRgb *>(dst.scanLine(y));

        int sum = 0;
        for (int i = -radius; i <= radius; ++i) {
            sum += qAlpha(sLine[qBound(0, i, w - 1)]);
        }
        for (int x = 0; x < w; ++x) {
            dLine[x] = qRgba(0, 0, 0, sum / span);
            sum += qAlpha(sLine[qBound(0, x + radius + 1, w - 1)])
                 - qAlpha(sLine[qBound(0, x - radius, w - 1)]);
        }
    }
}

void ShadowProvider::boxBlurV(const QImage &src, QImage &dst, int radius)
{
    const int w = src.width();
    const int h = src.height();
    const int span = 2 * radius + 1;

    for (int x = 0; x < w; ++x) {
        int sum = 0;
        for (int i = -radius; i <= radius; ++i) {
            const QRgb *line = reinterpret_cast<const QRgb *>(
                src.constScanLine(qBound(0, i, h - 1)));
            sum += qAlpha(line[x]);
        }
        for (int y = 0; y < h; ++y) {
            QRgb *dLine = reinterpret_cast<QRgb *>(dst.scanLine(y));
            dLine[x] = qRgba(0, 0, 0, sum / span);

            const QRgb *addLine = reinterpret_cast<const QRgb *>(
                src.constScanLine(qBound(0, y + radius + 1, h - 1)));
            const QRgb *subLine = reinterpret_cast<const QRgb *>(
                src.constScanLine(qBound(0, y - radius, h - 1)));
            sum += qAlpha(addLine[x]) - qAlpha(subLine[x]);
        }
    }
}

void ShadowProvider::boxBlur(QImage &image, int radius)
{
    QImage temp(image.size(), image.format());
    temp.fill(Qt::transparent);

    boxBlurH(image, temp, radius);
    boxBlurV(temp, image, radius);
    boxBlurH(image, temp, radius);
    boxBlurV(temp, image, radius);
}

std::shared_ptr<KDecoration3::DecorationShadow> ShadowProvider::create(int scaledRadius)
{
    const int cr = qMax(1, scaledRadius);
    if (s_cache && s_cacheRadius == cr) {
        return s_cache;
    }

    const int pad    = Shadow::Padding;
    const int sample = 2 * cr + 1;
    const int dim    = sample + 2 * pad;

    /* Layer 1: large soft ambient shadow, offset down */
    QImage layer1(dim, dim, QImage::Format_ARGB32_Premultiplied);
    layer1.fill(Qt::transparent);
    {
        QPainter p(&layer1);
        p.setRenderHint(QPainter::Antialiasing);
        p.setPen(Qt::NoPen);
        p.setBrush(Qt::black);
        p.drawRoundedRect(QRectF(pad, pad + Shadow::Layer1YOffset, sample, sample), cr, cr);
    }
    boxBlur(layer1, pad / Shadow::Layer1BlurDiv);
    for (int y = 0; y < dim; ++y) {
        QRgb *line = reinterpret_cast<QRgb *>(layer1.scanLine(y));
        for (int x = 0; x < dim; ++x) {
            const int a = qMin(255, qRound(qAlpha(line[x]) * Shadow::Layer1Alpha));
            line[x] = qRgba(0, 0, 0, a);
        }
    }

    /* Layer 2: smaller tight shadow, slight upward offset */
    QImage layer2(dim, dim, QImage::Format_ARGB32_Premultiplied);
    layer2.fill(Qt::transparent);
    {
        QPainter p(&layer2);
        p.setRenderHint(QPainter::Antialiasing);
        p.setPen(Qt::NoPen);
        p.setBrush(Qt::black);
        p.drawRoundedRect(QRectF(pad, pad + Shadow::Layer2YOffset, sample, sample), cr, cr);
    }
    boxBlur(layer2, pad / Shadow::Layer2BlurDiv);
    for (int y = 0; y < dim; ++y) {
        QRgb *line = reinterpret_cast<QRgb *>(layer2.scanLine(y));
        for (int x = 0; x < dim; ++x) {
            const int a = qMin(255, qRound(qAlpha(line[x]) * Shadow::Layer2Alpha));
            line[x] = qRgba(0, 0, 0, a);
        }
    }

    /* Composite layer2 on top of layer1 */
    {
        QPainter p(&layer1);
        p.setCompositionMode(QPainter::CompositionMode_SourceOver);
        p.drawImage(0, 0, layer2);
    }

    /* Mask out inner window area */
    {
        QPainter p(&layer1);
        p.setRenderHint(QPainter::Antialiasing);
        p.setCompositionMode(QPainter::CompositionMode_DestinationOut);
        p.setPen(Qt::NoPen);
        p.setBrush(Qt::black);
        const int inset = Shadow::MaskInset;
        const QRectF inner(pad + inset, pad + inset,
                           sample - 2 * inset, sample - 2 * inset);
        p.drawRoundedRect(inner, cr, cr);
    }

    auto shadow = std::make_shared<KDecoration3::DecorationShadow>();
    shadow->setShadow(layer1);
    shadow->setPadding(QMarginsF(pad, pad, pad, pad));
    shadow->setInnerShadowRect(QRectF(pad + cr, pad + cr, 1, 1));

    s_cache = shadow;
    s_cacheRadius = cr;
    return shadow;
}

} // namespace Suru
