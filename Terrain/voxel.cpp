/**
 *  @Author David Lonni
 *  @Author Sébastien Beugnon
 *  @Author Maxime Demaille
 */

#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>

#include "voxel.h"

Voxel::Voxel()
{
    position = QVector3D(0,0,0);
    length = 1;

    children.clear();
    parent = 0;

    card = Cardinal::NONE;
}

Voxel::Voxel(QVector3D _position, float _length)
{
    position = QVector3D(_position.x(),_position.y(),_position.z());
    length = _length;

    children.clear();
    parent = 0;

    card = Cardinal::NONE;
}

Voxel::Voxel(QVector3D _position, float _length, Cardinal c)
{
    position = QVector3D(_position.x(),_position.y(),_position.z());
    length = _length;

    children.clear();
    parent = 0;

    card = c;
}

void Voxel::set(QVector3D _position, float _length){
    position = QVector3D(_position.x(),_position.y(),_position.z());
    length = _length;

    children.clear();
    parent = 0;
}

/**
 * @brief Voxel::DisplayVoxel affiche un voxel en opengl
 */
void Voxel::DisplayVoxel(){
    glPushMatrix();
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glTranslatef(position.x(), position.y(), position.z());

    glColor3f(0.f,0.f,1.f);
    float midLength = length / 2.f;
    float yLength = 5.f;

    glCullFace(GL_FRONT);
    glBegin(GL_QUADS);
    glVertex3f(position.x() - midLength, position.y() - yLength, position.z() + midLength);
    glVertex3f(position.x() + midLength, position.y() - yLength, position.z() + midLength);
    glVertex3f(position.x() + midLength, position.y() + yLength , position.z() + midLength);
    glVertex3f(position.x() - midLength, position.y() + yLength , position.z() + midLength);

    glVertex3f(position.x() + midLength, position.y() - yLength , position.z() - midLength);
    glVertex3f(position.x() - midLength, position.y() - yLength , position.z() - midLength);
    glVertex3f(position.x() - midLength, position.y() + yLength , position.z() - midLength);
    glVertex3f(position.x() + midLength, position.y() + yLength , position.z() - midLength);

    glVertex3f(position.x() - midLength, position.y() - yLength , position.z() - midLength);
    glVertex3f(position.x() - midLength, position.y() - yLength , position.z() + midLength);
    glVertex3f(position.x() - midLength, position.y() + yLength , position.z() + midLength);
    glVertex3f(position.x() - midLength, position.y() + yLength , position.z() - midLength);

    glVertex3f(position.x() + midLength, position.y() - yLength , position.z() + midLength);
    glVertex3f(position.x() + midLength, position.y() - yLength , position.z() - midLength);
    glVertex3f(position.x() + midLength, position.y() + yLength , position.z() - midLength);
    glVertex3f(position.x() + midLength, position.y() + yLength , position.z() + midLength);
    glEnd();

    glCullFace(GL_BACK);
    glBegin(GL_QUADS);
    glVertex3f(position.x() - midLength, position.y() - yLength , position.z() + midLength);
    glVertex3f(position.x() + midLength, position.y() - yLength , position.z() + midLength);
    glVertex3f(position.x() + midLength, position.y() + yLength , position.z() + midLength);
    glVertex3f(position.x() - midLength, position.y() + yLength , position.z() + midLength);

    glVertex3f(position.x() + midLength, position.y() - yLength , position.z() - midLength);
    glVertex3f(position.x() - midLength, position.y() - yLength , position.z() - midLength);
    glVertex3f(position.x() - midLength, position.y() + yLength , position.z() - midLength);
    glVertex3f(position.x() + midLength, position.y() + yLength , position.z() - midLength);

    glVertex3f(position.x() - midLength, position.y() - yLength , position.z() - midLength);
    glVertex3f(position.x() - midLength, position.y() - yLength , position.z() + midLength);
    glVertex3f(position.x() - midLength, position.y() + yLength , position.z() + midLength);
    glVertex3f(position.x() - midLength, position.y() + yLength , position.z() - midLength);

    glVertex3f(position.x() + midLength, position.y() - yLength , position.z() + midLength);
    glVertex3f(position.x() + midLength, position.y() - yLength , position.z() - midLength);
    glVertex3f(position.x() + midLength, position.y() + yLength , position.z() - midLength);
    glVertex3f(position.x() + midLength, position.y() + yLength , position.z() + midLength);
    glEnd();

    glDisable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPopMatrix();
}

void Voxel::AddChild(Voxel *v)
{
    children.push_back(v);
}

void Voxel::AddParent(Voxel *v)
{
    if(parent == 0){
        parent = v;
    }
}

/**
 * @brief Voxel::DisplayStat affiche la liste des voisins
 */
void Voxel::DisplayStat()
{
    qDebug() << "---------------------";
    qDebug() << "Parent : " << parent << " Length : " << parent->getLength() << " Cardinal " << parent->getCardinal();

    if(parent->hasChildren()){
        std::vector<Voxel*> parentChildren = parent->getChildren();
        for(unsigned int i = 0 ; i < parentChildren.size() ; i++){
            if(parentChildren[i]->hasChildren() && parentChildren[i]->getCardinal() != card){
                if(card == Cardinal::NORTH_WEST && (parentChildren[i]->getCardinal() == Cardinal::NORTH_EAST || parentChildren[i]->getCardinal() == Cardinal::SOUTH_WEST)){
                    qDebug() << "My Cardinal : " << card << " Neighbor Cardinal has children : " << parentChildren[i]->getCardinal();
                }else if(card == Cardinal::NORTH_EAST && (parentChildren[i]->getCardinal() == Cardinal::NORTH_WEST || parentChildren[i]->getCardinal() == Cardinal::SOUTH_EAST)){
                    qDebug() << "My Cardinal : " << card << " Neighbor Cardinal has children : " << parentChildren[i]->getCardinal();
                }else if(card == Cardinal::SOUTH_WEST && (parentChildren[i]->getCardinal() == Cardinal::SOUTH_EAST || parentChildren[i]->getCardinal() == Cardinal::NORTH_WEST)){
                    qDebug() << "My Cardinal : " << card << " Neighbor Cardinal has children : " << parentChildren[i]->getCardinal();
                }else if(card == Cardinal::SOUTH_EAST && (parentChildren[i]->getCardinal() == Cardinal::SOUTH_WEST || parentChildren[i]->getCardinal() == Cardinal::NORTH_EAST)){
                    qDebug() << "My Cardinal : " << card << " Neighbor Cardinal has children : " << parentChildren[i]->getCardinal();
                }
            }
        }
    }
}

/**
 * @brief Voxel::GetNeighbors
 * @return l'ensemble des voxels voisins
 */
std::vector<Voxel *> Voxel::GetNeighbors() const
{
    std::vector<Voxel*> neighbors;
    if(parent->hasChildren()){
        std::vector<Voxel*> parentChildren = parent->getChildren();
        for(unsigned int i = 0 ; i < parentChildren.size() ; i++){
            if(parentChildren[i]->hasChildren() && parentChildren[i]->getCardinal() != card){
                if(card == Cardinal::NORTH_WEST && (parentChildren[i]->getCardinal() == Cardinal::NORTH_EAST || parentChildren[i]->getCardinal() == Cardinal::SOUTH_WEST)){
                    neighbors.push_back(parentChildren[i]);
                }else if(card == Cardinal::NORTH_EAST && (parentChildren[i]->getCardinal() == Cardinal::NORTH_WEST || parentChildren[i]->getCardinal() == Cardinal::SOUTH_EAST)){
                    neighbors.push_back(parentChildren[i]);
                }else if(card == Cardinal::SOUTH_WEST && (parentChildren[i]->getCardinal() == Cardinal::SOUTH_EAST || parentChildren[i]->getCardinal() == Cardinal::NORTH_WEST)){
                    neighbors.push_back(parentChildren[i]);
                }else if(card == Cardinal::SOUTH_EAST && (parentChildren[i]->getCardinal() == Cardinal::SOUTH_WEST || parentChildren[i]->getCardinal() == Cardinal::NORTH_EAST)){
                    neighbors.push_back(parentChildren[i]);
                }
            }
        }


    }

    return neighbors;
}
