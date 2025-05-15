#pragma once

#include "Backend/Backend.h"

#include <QWidget>
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DCore/QComponent>
#include <Qt3DExtras/QDiffuseSpecularMaterial>
#include <Qt3DRender/QTexture>
#include <Qt3DRender/QMesh>
#include <Qt3DRender/QCamera>
#include <Qt3DWindow>
#include <QClearBuffers>
#include <QTextureMaterial>

class NavBall : public QWidget {
Q_OBJECT

public:
    explicit NavBall(QWidget *parent = nullptr);
    QSize sizeHint() const override { return QSize(400, 400); }
    void updateNavball(const QQuaternion& orientation);

public slots:
    void onTelemetryData(const HPRC::Telemetry& telemetry);

private:
    Qt3DExtras::Qt3DWindow *m_view;
    Qt3DCore::QEntity *m_rootEntity;
    Qt3DCore::QEntity *m_navballEntity;
    Qt3DCore::QTransform *m_navballTransform;
    Qt3DExtras::QTextureMaterial *m_material;
    Qt3DRender::QTexture2D *m_texture;

    Qt3DRender::QClearBuffers *m_clearBuffers = nullptr;
};
