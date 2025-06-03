//
// Created by William Scheirey on 1/27/25.
//

#ifndef GROUNDSTATION_2025_IMAGEWIDGET_H
#define GROUNDSTATION_2025_IMAGEWIDGET_H

#include <QLabel>
#include <QPixmap>

class ImageWidget: public QLabel
{
public:
    explicit ImageWidget(QWidget *parent = nullptr);

    float pixelDensityFactor = 1;

    QPixmap originalPixmap;

    void resizeImage(int width, int height);

    void setImage(const QString &image);

protected:
    void resizeEvent(QResizeEvent *event) override;
};


#endif //GROUNDSTATION_2025_IMAGEWIDGET_H
