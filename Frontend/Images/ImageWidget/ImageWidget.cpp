//
// Created by William Scheirey on 1/27/25.
//

#include "ImageWidget.h"
#include <QTimer>
#include <QResizeEvent>

ImageWidget::ImageWidget(QWidget *parent): QLabel(parent)
{

}

void ImageWidget::resizeImage(int width, int height)
{
    if(originalPixmap.isNull())
        return;

    QPixmap pixmap = originalPixmap.copy().scaled(width*pixelDensityFactor, height*pixelDensityFactor, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    pixmap.setDevicePixelRatio(pixelDensityFactor);

    setPixmap(pixmap);
    repaint();
}

void ImageWidget::setImage(const QString &image)
{
    QTimer::singleShot(100, [this, image]()
    {
        originalPixmap = QPixmap(image);
        resizeImage(size().width(), size().height());
    });
}

void ImageWidget::resizeEvent(QResizeEvent *event)
{
    QLabel::resizeEvent(event);
    resizeImage(size().width(), size().height());
}
