#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include <QObject>
#include <QList>


enum QuadTreePosition {LEFT_TOP, RIGHT_TOP, LEFT_BOTTOM, RIGHT_BOTTOM};
template<class Element,typename Comparator>
class QuadTree : protected virtual QObject {
    Element* element;
    QuadTreePosition position;
public:
    ///Root Constructor
    explicit QuadTree(Element* element, QuadTree* parent = 0);
protected:
    ///Node Constructor
    QuadTree(Element* element,QuadTreePosition position, QuadTree* parent = 0);
public:
    ///Destructor
    virtual ~QuadTree();
    //
    virtual void makeSubQuadTrees();

    virtual void clearSubs();

    virtual bool isRoot();

    virtual bool isLeaf();

    virtual bool canSubdivide() = 0;

    virtual bool mustSubdivide(Comparator* comparator) = 0;

    virtual QuadTreePosition getPosition();

    virtual QList<QuadTree<Element,Comparator>*> getSubQuadTrees();

    virtual void update(Comparator* tolerance);
};

#endif

