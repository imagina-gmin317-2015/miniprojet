#ifndef OCTREE_H
#define OCTREE_H
#include <QtCore>
#include <QVector3D>
#include <QVector>


template<class T>
class Octree<T> : QObject {
  Q_OBJECT
private:
  QVector<T*> children;
public:
  explicit Octree(Octree* parent=0);
  Octree();
  virtual ~Octree();

  bool isLeaf();
  bool isRoot();
};


#endif
