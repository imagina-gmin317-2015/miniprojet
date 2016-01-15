#include "component.h"
#include "engine/core/gameobject.h"
#include "transform.h"
#include "camera.h"
#include <QtGui/QScreen>
#include <QWidget>
#include <QtGui/QGuiApplication>
#include <QOpenGLWidget>
#include "engine/rendering/breakingwindow.h"
#include <QtCore>
#include <QtGui>


Camera::Camera( GameObject* parent )
    : Component( parent ), m_upVector( 0.0f, 1.0f, 0.0f )
    , m_viewCenter( 0.0f, 0.0f, 0.0f )
    , m_cameraToCenter( 0.0f, 0.0f, -1.0f )
    , m_rightVector(QVector3D::crossProduct(m_upVector,m_cameraToCenter))
    , m_projectionType( Camera::OrthogonalProjection )
    , m_nearPlane( 0.1f )
    , m_farPlane( 1024.0f )
    , m_fieldOfView( 60.0f )
    , m_aspectRatio( 1.0f )
    , m_left( -0.5 )
    , m_right( 0.5f )
    , m_bottom( -0.5f )
    , m_top( 0.5f )
    , speed(1)
    , m_viewMatrixDirty( true )
    , m_viewProjectionMatrixDirty( true )
    , direction_vue_h(3.14f)
    , direction_vue_v(0)
{
    updateOrthogonalProjection();
}
Camera* Camera::main_camera = nullptr;


Camera::~Camera() {
}

Camera::ProjectionType Camera::projectionType() const
{
    return m_projectionType;
}

void Camera::setProjectionType(Camera::ProjectionType type)
{
    m_projectionType = type;
}

QVector3D Camera::viewCenter() const
{
    return m_viewCenter;
}

QVector3D Camera::viewVector() const
{
    return m_cameraToCenter;
}

QVector3D Camera::upVector() const
{
    return m_upVector;
}

void Camera::rotate(const QQuaternion& q) {
    m_upVector = q.rotatedVector( m_upVector );
    m_cameraToCenter = q.rotatedVector( m_cameraToCenter );
    m_viewCenter = gameObject()->transform()->position() + m_cameraToCenter;
    transform()->Rotate(q);
}

void Camera::rotateAboutViewCenter( const QQuaternion& q )
{
    m_upVector = q.rotatedVector( m_upVector );
    m_cameraToCenter = q.rotatedVector( m_cameraToCenter );
    gameObject()->transform()->setPosition( m_viewCenter - m_cameraToCenter);
}


QQuaternion Camera::tiltRotation( const float& angle ) const
{
    QVector3D xBasis = QVector3D::crossProduct( m_upVector, m_cameraToCenter.normalized() ).normalized();
    return QQuaternion::fromAxisAndAngle( xBasis, -angle );
}

QQuaternion Camera::panRotation( const float& angle ) const
{
    return QQuaternion::fromAxisAndAngle(m_upVector, angle );
}

QQuaternion Camera::panRotation( const float& angle, const QVector3D& axis ) const
{
    return QQuaternion::fromAxisAndAngle( axis, angle );
}

QQuaternion Camera::rollRotation( const float& angle )
{
    return QQuaternion::fromAxisAndAngle(transform()->forward(), -angle );
}

void Camera::tilt( const float& angle )
{
    QQuaternion q = tiltRotation( angle );
    rotate( q );
}

void Camera::pan( const float& angle )
{
    QQuaternion q = panRotation( -angle );
    rotate( q );
}

void Camera::pan( const float& angle, const QVector3D& axis )
{
    QQuaternion q = panRotation( -angle, axis );
    rotate( q );
}

void Camera::roll( const float& angle )
{
    QQuaternion q = rollRotation( -angle );
    rotate( q );
}

void Camera::tiltAboutViewCenter( const float& angle )
{
    QQuaternion q = tiltRotation( -angle );
    rotateAboutViewCenter( q );
}

void Camera::panAboutViewCenter( const float& angle )
{
    QQuaternion q = panRotation( angle );
    rotateAboutViewCenter( q );
}

void Camera::rollAboutViewCenter( const float& angle )
{
    QQuaternion q = rollRotation( angle );
    rotateAboutViewCenter( q );
}


/// EVENTS ////////////////////////////////////////////////////////////////////////

// A INTEGRER
void Camera::mouseMoveEvent(QMouseEvent *event)
{
    float mouseSpeed = 0.005f;

    if(event->type() == QEvent::MouseMove){
        float xm, ym;

        xm = (float)event->x()/BreakingWindow::widthWindow()  - 0.5f;
        ym = (float)event->y()/BreakingWindow::heightWindow() - 0.5f;

        if( xm < -0.25f )       xm = 0.25f;
        else if( xm > 0.25f )   xm = -0.25f;

        if( ym < -0.25f )       ym = -0.25f;
        else if( ym > 0.25f )   ym = 0.25f;

        direction_vue_h += mouseSpeed * float(BreakingWindow::widthWindow()/2 - event->x() );
        direction_vue_v += mouseSpeed * float(BreakingWindow::heightWindow()/2 - event->y() );

        QVector3D direction = QVector3D(
                    cos(direction_vue_v) * sin(direction_vue_h),
                    sin(direction_vue_v),
                    cos(direction_vue_v) * cos(direction_vue_h)
                    );
        direction.normalize();
        m_cameraToCenter = direction;
        transform()->setForward(direction);

        // Vecteur droite
        QVector3D droite = QVector3D(
                    sin(direction_vue_h - 3.14f/2.0f),
                    0,
                    cos(direction_vue_h - 3.14f/2.0f)
                    );
        droite.normalize();
        m_rightVector = droite;
        transform()->setRight(m_rightVector);

        // Vecteur up
        QVector3D up = QVector3D::crossProduct(droite, direction);
        up.normalize();
        m_upVector = up;
        transform()->setUp(m_upVector);
        //QCursor::setPos(BreakingWindow::widthWindow()/2 + BreakingWindow::xWindow(), BreakingWindow::heightWindow()/2 + BreakingWindow::yWindow());
    }
}

///Component Template
template <>
Camera* GameObject::getComponent();
template Camera* GameObject::getComponent();
//Add component
