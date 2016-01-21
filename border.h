#ifndef BORDER_H
#define BORDER_H
#include <entity.h>
#include <QVector>
#include <QVector2D>



class Border : public Entity
{
public:
    Border();

    Border(QVector<QVector2D> pos);

    EntityType getEntityType() override;


    QVector<QVector2D> getPositions() const;
    void setPositions(const QVector<QVector2D> &positions);

private:

    QVector<QVector2D> _positions;
};

#endif // BORDER_H
