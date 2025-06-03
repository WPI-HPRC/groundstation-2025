//
// Created by William Scheirey on 2/19/25.
//

#ifndef GROUNDSTATION_2025_GAUGEDISPLAY_H
#define GROUNDSTATION_2025_GAUGEDISPLAY_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui
{
    class GaugeDisplay;
}
QT_END_NAMESPACE

class GaugeDisplay : public QWidget
{
Q_OBJECT

public:
    explicit GaugeDisplay(QWidget *parent = nullptr);
    void setGaugeWidget(QWidget *widget);
    void updateNumber(const QString &numberString);
    void updateLabel(const QString &label);
    void setUnits(const QString &unitLabel);
    ~GaugeDisplay() override;

private:
    Ui::GaugeDisplay *ui;
};


#endif //GROUNDSTATION_2025_GAUGEDISPLAY_H
