//
// Created by William Scheirey on 2/8/25.
//

#ifndef GROUNDSTATION_2025_APRSWIDGET_H
#define GROUNDSTATION_2025_APRSWIDGET_H

#include <QWidget>
#include "Backend/APRS/KissClient.h"


QT_BEGIN_NAMESPACE
namespace Ui
{
    class AprsWidget;
}
QT_END_NAMESPACE

class AprsWidget : public QWidget
{
Q_OBJECT

public:
    explicit AprsWidget(QWidget *parent = nullptr);

    ~AprsWidget() override;

public slots:
    void direwolfOutputAvailable(const QString& output);
    void kissOutput(const KissClient::AprsOutput& output);

private:
    Ui::AprsWidget *ui;
};


#endif //GROUNDSTATION_2025_APRSWIDGET_H
