//
// Created by William Scheirey on 1/27/25.
//

#include <QtSvgWidgets/QSvgWidget>

#ifndef GROUNDSTATION_2025_LOGOSVG_H
#define GROUNDSTATION_2025_LOGOSVG_H

class LogoSvg: public QSvgWidget
{
public:
    explicit LogoSvg(QWidget *parent = nullptr): QSvgWidget(parent)
    {
        load(QString(":/Images/HPRC Logo.svg"));
    }
};

#endif //GROUNDSTATION_2025_LOGOSVG_H
