#ifndef KDTREE_H
#define KDTREE_H
#include <QtCore>
#include <QVector3>
#include <QVector>


template<class T>
class KDtree<T> : QObject {
  Q_OBJECT
private:
  QVector<T*> children;
public:
  explicit KDtree(Octree* parent=0);
  Octree();
  virtual ~Octree();

  bool isLeaf();
  bool isRoot();
};


#endif
