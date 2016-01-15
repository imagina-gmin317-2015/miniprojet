/**
 *  @Author David Lonni
 *  @Author Sébastien Beugnon
 *  @Author Maxime Demaille
 */
#ifndef VOXEL_H
#define VOXEL_H

#include <QVector3D>

#include "commonstruct.h"

/**
 * @brief The Voxel class est un élément de la classe QuadTree pour calculer la décimation du terrain.
 */
class Voxel
{
    public:
        Voxel();
        Voxel(QVector3D _position, float _length);
        Voxel(QVector3D _position, float _length, Cardinal c);

        QVector3D getPosition() const { return position; };
        void DisplayVoxel();

        void AddChild(Voxel* v);
        void AddParent(Voxel* v);

        void DisplayStat();


        bool hasChildren() const { return (children.size() > 0)?true:false; }

        //GETTER
        int getLength() const { return length; };
        int getOffsetX() const { return offsetX; };
        int getOffsetY() const { return offsetY; };

        std::vector<Voxel*> GetNeighbors() const;

        std::vector<Voxel*> getChildren() const { return children; };
        Cardinal getCardinal() const { return card; };

        //SETTER
        void set(QVector3D _position, float _length);
        void setOffset(int offX, int offY) { offsetX = offX; offsetY = offY; };

    private:
        QVector3D position;

        int offsetX;
        int offsetY;
        float length;

        Cardinal card;

        Voxel* parent;
        std::vector<Voxel*> children;
};

#endif // VOXEL_H
