//
// Created by William Scheirey on 1/31/25.
//

#ifndef GROUNDSTATION_2025_PLACEHOLDER4X3IMAGE_H
#define GROUNDSTATION_2025_PLACEHOLDER4X3IMAGE_H

#include "Frontend/Images/ImageWidget/ImageWidget.h"

class Placeholder4x3Image: public ImageWidget
{
public:
    explicit Placeholder4x3Image(QWidget *parent = nullptr): ImageWidget(parent)
    {
//        this->setImage(":/Images/Placeholder4x3.png");
        this->setImage(":/Images/IMG_0481.jpg");
    }
};

#endif //GROUNDSTATION_2025_PLACEHOLDER4X3IMAGE_H
