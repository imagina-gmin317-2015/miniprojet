#include "engine/core/transform.h"


Transform::Transform(GameObject* parent) : Component(parent)
{
  setObjectName("core/Transform");
  m_position.setX(0);
  m_position.setY(0);
  m_position.setZ(0);
  m_scaling.setX(1);
  m_scaling.setY(1);
  m_scaling.setZ(1);
  m_forward = QVector3D(0,0,1);
  m_up = QVector3D(0,1,0);
  m_right = QVector3D(1,0,0);
  modelMatrix();
}

Transform::~Transform()
{

}

QMatrix4x4 Transform::modelMatrix() {
    if(m_dirty) {

        m_matrix.setToIdentity();
        m_matrix.scale(scaling());
        m_matrix.rotate(rotation());
        m_matrix.translate(position());
        m_dirty = false;
    }
    return m_matrix;
}
