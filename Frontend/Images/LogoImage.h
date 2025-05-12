//
// Created by William Scheirey on 1/27/25.
//

#ifndef GROUNDSTATION_2025_LOGOIMAGE_H
#define GROUNDSTATION_2025_LOGOIMAGE_H

#include "Frontend/Images/ImageWidget/ImageWidget.h"
#include <QTimer>

class LogoImage: public ImageWidget
{
public:
    explicit LogoImage(QWidget *parent = nullptr): ImageWidget(parent)
    {
//        QTimer::singleShot(100, [this, image]()
//        {
            this->setImage(":/Images/hprc_logo.png");
//        });
    }
};

#endif //GROUNDSTATION_2025_LOGOIMAGE_H
