#ifndef GS_BACKEND_2024_2025_VIEWER3D_H
#define GS_BACKEND_2024_2025_VIEWER3D_H

#include <QWidget>
#include <QColor>
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QMesh>
#include <Qt3DExtras/QDiffuseSpecularMaterial>
#include <Qt3DExtras/Qt3DWindow>
#include <QClearBuffers>

#include "Backend/Backend.h"

class Viewer3D : public QWidget {
Q_OBJECT

public:
    explicit Viewer3D(QWidget *parent = nullptr);

    void orientRocket(QQuaternion orientation) const;
    void updateColors(QColor panel, QColor highlight);

    float m_rocketVertOffset = -1.8f;
    float m_cameraDistToRocket = 40.0f;
    float m_rocketScale = 0.18f;

    QQuaternion m_rocketOrientVertically = QQuaternion::fromEulerAngles(-90.0f, 0.0f, 0.0f);

public slots:
    void telemetryAvailable(const HPRC::Telemetry &telemetry);

private:
    Qt3DCore::QTransform *m_rocketTransform = nullptr;
    Qt3DExtras::Qt3DWindow *m_view = nullptr;
    Qt3DExtras::QDiffuseSpecularMaterial *m_rocketMaterial = nullptr;

    Qt3DRender::QClearBuffers *m_clearBuffers = nullptr;
};

#endif // GS_BACKEND_2024_2025_VIEWER3D_H
