#include "NavBall.h"
#include <QVBoxLayout>
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QDiffuseSpecularMaterial>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QRenderSurfaceSelector>
#include <Qt3DWindow>
#include <Qt3DExtras/QSphereMesh>
#include <QViewport>
#include <QCameraSelector>
#include <QTextureMaterial>
#include <QDiffuseSpecularMapMaterial>

NavBall::NavBall(QWidget *parent) : QWidget(parent), m_view(nullptr), m_rootEntity(nullptr),
                                    m_navballEntity(nullptr), m_navballTransform(nullptr), m_material(nullptr) {
    // Initialize the 3D view
    m_view = new Qt3DExtras::Qt3DWindow();
//    m_view->defaultFrameGraph()->setParent(nullptr); // Detach default framegraph

    m_rootEntity = new Qt3DCore::QEntity();

    // Create the navball entity
    m_navballEntity = new Qt3DCore::QEntity(m_rootEntity);
    m_navballTransform = new Qt3DCore::QTransform();

    QImage textureImage(":/Models/navball.jpg");

    // Flip the image horizontally
    textureImage = textureImage.mirrored(true, false);  // Flip horizontally (mirrored along X-axis)

    // Save the flipped image to a temporary file
    QString tempFilePath = QDir::temp().absoluteFilePath("flipped_image.jpg");
    textureImage.save(tempFilePath, "JPG");

    // Create the Qt3D texture and load the image
    auto *texture = new Qt3DRender::QTexture2D(m_rootEntity);
    auto *textureImageData = new Qt3DRender::QTextureImage(m_rootEntity);

    // Set the flipped image from the temporary file
    textureImageData->setSource(QUrl::fromLocalFile(tempFilePath));
    textureImageData->setMirrored(true);

    texture->addTextureImage(textureImageData);

    m_material = new Qt3DExtras::QTextureMaterial(m_rootEntity);
    m_material->setTexture(texture);

    // Create mesh (a sphere for the navball)
    auto *sphereMesh = new Qt3DExtras::QSphereMesh(m_rootEntity);

    sphereMesh->setRadius(7);
    sphereMesh->setRings(64);
    sphereMesh->setSlices(64);

    m_navballEntity->addComponent(sphereMesh);
    m_navballEntity->addComponent(m_navballTransform);
    m_navballEntity->addComponent(m_material);

    // Set up camera
    Qt3DRender::QCamera *camera = m_view->camera();
    camera->setPosition(QVector3D(0, 0, 40));  // Adjust the camera distance
    camera->setViewCenter(QVector3D(0, 0, 0));

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

    m_view->setRootEntity(m_rootEntity);

    // Wrap in QWidget
    QWidget *container = QWidget::createWindowContainer(m_view, this);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setContentsMargins(0, 0, 0, 0);

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(container);
    setLayout(layout);

    connect(&Backend::getInstance(), &Backend::telemetryAvailable, this, &NavBall::onTelemetryData);
}

void NavBall::updateNavball(const QQuaternion &orientation) {
    m_navballTransform->setRotation(orientation);
//    m_view->update();
}

void NavBall::onTelemetryData(const HPRC::Telemetry& telemetry) {
    if (telemetry.has_payloadpacket()) {
        const auto &data = telemetry.payloadpacket();
        QQuaternion q(data.w(), data.i(), data.k(), data.j());

        QQuaternion remap = QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), -90.0f); // Pitch +90° from Z-forward to Z-down
        updateNavball(remap * q.conjugated());
    }
}
