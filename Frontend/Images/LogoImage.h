//
// Created by William Scheirey on 1/27/25.
//

#ifndef GROUNDSTATION_2025_LOGOIMAGE_H
#define GROUNDSTATION_2025_LOGOIMAGE_H

#include "Frontend/Images/ImageWidget/ImageWidget.h"

class LogoImage: public ImageWidget
{
public:
    explicit LogoImage(QWidget *parent = nullptr): ImageWidget(parent)
    {
        this->setImage(":/Images/hprc_logo.png");
    }
};

#endif //GROUNDSTATION_2025_LOGOIMAGE_H
