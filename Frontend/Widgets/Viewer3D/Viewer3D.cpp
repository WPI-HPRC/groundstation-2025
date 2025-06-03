#include "Viewer3D.h"
#include <QVBoxLayout>
#include <Qt3DRender/QRenderSettings>
#include <Qt3DRender/QRenderSurfaceSelector>
#include <Qt3DRender/QViewport>
#include <Qt3DRender/QCameraSelector>
#include <Qt3DRender/QClearBuffers>
#include <Qt3DExtras/QForwardRenderer>

Viewer3D::Viewer3D(QWidget *parent) : QWidget(parent) {
    setMinimumSize(1, 1);

    m_view = new Qt3DExtras::Qt3DWindow;
//    m_view->defaultFrameGraph()->setParent(nullptr); // Detach default framegraph

    auto *rootEntity = new Qt3DCore::QEntity;

    // Rocket setup
    auto *rocketEntity = new Qt3DCore::QEntity(rootEntity);
    auto *rocketMesh = new Qt3DRender::QMesh;
    rocketMesh->setSource(QUrl("qrc:/Models/HPRC_rocket.stl"));

    m_rocketMaterial = new Qt3DExtras::QDiffuseSpecularMaterial(rootEntity);
    m_rocketMaterial->setDiffuse(QColor(255, 0, 0));
    m_rocketMaterial->setAmbient(QColor(155, 0, 0));
    m_rocketMaterial->setSpecular(QColor(255, 255, 255));
    m_rocketMaterial->setShininess(1);

    m_rocketTransform = new Qt3DCore::QTransform;
    m_rocketTransform->setTranslation(QVector3D(0.0, m_rocketVertOffset, 0.0));
    m_rocketTransform->setScale3D(QVector3D(m_rocketScale, m_rocketScale, m_rocketScale));
    m_rocketTransform->setRotation(QQuaternion::fromEulerAngles(0.0f, 0.0f, 0.0f) * m_rocketOrientVertically);

    rocketEntity->addComponent(rocketMesh);
    rocketEntity->addComponent(m_rocketMaterial);
    rocketEntity->addComponent(m_rocketTransform);

    // Camera
    auto *camera = m_view->camera();
    camera->setPosition(QVector3D(0, 0, m_cameraDistToRocket));
    camera->setViewCenter(QVector3D(0, 0, 0));

    // Custom FrameGraph
    auto *surfaceSelector = new Qt3DRender::QRenderSurfaceSelector;
    surfaceSelector->setSurface(m_view);

    m_clearBuffers = new Qt3DRender::QClearBuffers(surfaceSelector);
    m_clearBuffers->setBuffers(Qt3DRender::QClearBuffers::ColorDepthBuffer);
    m_clearBuffers->setClearColor(QWidget::palette().window().color());

    auto *viewport = new Qt3DRender::QViewport(m_clearBuffers);
    viewport->setNormalizedRect(QRectF(0.0f, 0.0f, 1.0f, 1.0f));

    auto *cameraSelector = new Qt3DRender::QCameraSelector(viewport);
    cameraSelector->setCamera(camera);

//    surfaceSelector->setParent(nullptr);
    m_view->setActiveFrameGraph(surfaceSelector);

    m_view->setRootEntity(rootEntity);

    // Wrap in QWidget
    QWidget *container = QWidget::createWindowContainer(m_view, this);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setContentsMargins(0, 0, 0, 0);

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(container);
    setLayout(layout);

    connect(&Backend::getInstance(), &Backend::telemetryAvailable,
            this, &Viewer3D::telemetryAvailable);
}

void Viewer3D::orientRocket(QQuaternion orientation) const {
    m_rocketTransform->setRotation(orientation * m_rocketOrientVertically);
}

void Viewer3D::updateColors(QColor panel, QColor highlight) {
    if (m_rocketMaterial)
        m_rocketMaterial->setDiffuse(highlight);
    if (m_clearBuffers)
        m_clearBuffers->setClearColor(panel);
}

void Viewer3D::telemetryAvailable(const HPRC::Telemetry &telemetry) {
    if (telemetry.has_rocketpacket()) {
        QQuaternion q(
                telemetry.rocketpacket().w(),
                telemetry.rocketpacket().i(),
                telemetry.rocketpacket().k(),
                telemetry.rocketpacket().j()
        );
        orientRocket(q);
    }
}
