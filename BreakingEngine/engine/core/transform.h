#ifndef CORE_TRANSFORM_H
#define CORE_TRANSFORM_H

#include "engine/core/common.h"
#include "engine/core/component.h"
#include "engine/core/gameobject.h"

#include <QString>
#include <QVector3D>
#include <QQuaternion>
#include <QObject>
#include <QMatrix4x4>

class Transform : public virtual Component {
    Q_OBJECT
private:
    QVector3D m_position;
    QVector3D m_scaling;
    QQuaternion m_rotation;

    QVector3D m_up;
    QVector3D m_right;
    QVector3D m_forward;

    QMatrix4x4 m_matrix;
    bool m_dirty = true;


public:
    //static const QString NAME;
    explicit Transform(GameObject* parent=0);
    virtual ~Transform();




    QMatrix4x4 modelMatrix();

    QVector3D position() const {
        return m_position;
    }
    QVector3D scaling() const {
        return m_scaling;
    }
    QQuaternion rotation() const {
        return m_rotation;
    }

    void setPosition(QVector3D pos) {
        m_position = pos;
        m_dirty = true;
    }

    void setScaling(QVector3D scale) {
        m_scaling = scale;
        m_dirty = true;
    }

    void setRotation(QQuaternion rotation) {
        m_rotation = rotation;
        m_dirty = true;
    }

    void setForward(QVector3D forward) {
        m_forward = forward.normalized();
        m_dirty = true;
    }

    void setUp(QVector3D up) {
        m_up = up.normalized();
        m_dirty = true;
    }

    void setRight(QVector3D right) {
        m_right = right.normalized();
        m_dirty = true;
    }

    QVector3D up() const {
        return m_up;
    }
    QVector3D right() const {
        return m_right;
    }
    QVector3D forward() const {
        return m_forward;
    }


    void Translate(QVector3D vLocal) {
        // Calculate the amount to move by in world coordinates
        QVector3D vWorld;
        if ( !qFuzzyIsNull( vLocal.x() ) )
            // Calculate the vector for the local x axis
            vWorld += vLocal.x() * right();

        if ( !qFuzzyIsNull( vLocal.y() ) )
            vWorld += vLocal.y() * up();

        if ( !qFuzzyIsNull( vLocal.z() ) )
            vWorld += vLocal.z() * forward();

        // Update the camera position using the calculated world vector
        m_position += vWorld;
        m_dirty = true;
    }

    void TranslateWorld(QVector3D vWorld) {
        m_position += vWorld;
        m_dirty = true;
    }

    void Rotate(QQuaternion quaternion) {
        m_forward = quaternion.rotatedVector(m_forward);
        m_up = quaternion.rotatedVector(m_up);
        m_right = quaternion.rotatedVector(m_right);
        m_rotation = quaternion;
        m_dirty = true;
    }


};

#endif // TRANSFORM_H
