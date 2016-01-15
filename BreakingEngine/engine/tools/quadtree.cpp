#include "quadtree.h"
#include <QtCore>

template<class Element,typename Comparator>
QuadTree<Element,Comparator>::QuadTree(Element* element, QuadTree* parent) : QObject(parent) {
    this->element = element;
    this->position = QuadTree::Position::LEFT_TOP;
}

template<class Element,typename Comparator>
QuadTree<Element,Comparator>::QuadTree(Element* element,QuadTreePosition position, QuadTree* parent) : QObject(parent) {
    this->element = element;
    this->position = position;
}

template<class Element,typename Comparator>
QuadTree<Element,Comparator>::~QuadTree() {
    clearSubs();
}

template<class Element,typename Comparator>
void QuadTree<Element,Comparator>::makeSubQuadTrees() {
    QuadTree<Element,Comparator>* qt = new QuadTree<Element,Comparator>
            (this->element,QuadTreePosition::LEFT_TOP,this->parent());
    qt->setParent(this);
    qt = new QuadTree<Element,Comparator>
                (this->element,QuadTreePosition::RIGHT_TOP,this->parent());
    qt->setParent(this);
    qt = new QuadTree<Element,Comparator>
                (this->element,QuadTreePosition::LEFT_BOTTOM,this->parent());
    qt->setParent(this);
    qt = new QuadTree<Element,Comparator>
                (this->element,QuadTreePosition::RIGHT_BOTTOM,this->parent());
}

template<class Element,typename Comparator>
void QuadTree<Element,Comparator>::clearSubs() {
    QObjectList subs = QObject::children();
    while(!subs.empty())
    {
        QObject* qo = subs.front();
        subs.pop_front();
        delete qo;
    }
}

template<class Element, typename Comparator>
bool QuadTree<Element,Comparator>::isRoot() {
    return QObject::parent()==nullptr;
}

template<class Element, typename Comparator>
bool QuadTree<Element,Comparator>::isLeaf() {
    return QObject::children().empty();
}

template<class Element, typename Comparator>
QuadTreePosition QuadTree<Element,Comparator>::getPosition() {
    return this->position;
}

template<class Element,typename Comparator>
QList<QuadTree<Element,Comparator>*> QuadTree<Element,Comparator>::getSubQuadTrees() {
    QObjectList list = QObject::children();
    QList<QuadTree<Element,Comparator>*> subs;
    foreach(QObject* qo, list) {
        subs.append(dynamic_cast<QuadTree<Element,Comparator>*>(qo));
    }

    return subs;
}


template<class Element,typename Comparator>
void QuadTree<Element,Comparator>::update(Comparator *tolerance) {

    if(!canSubdivide() || !mustSubdivide(tolerance)) {
        clearSubs();
        return;
    }

    makeSubQuadTrees();
    QList<QuadTree<Element,Comparator>*> subs = getSubQuadTrees();
    while(!subs.empty()) {
        QuadTree<Element,Comparator>* qt = subs.front();
        subs.pop_front();
        qt->update(tolerance);
    }

}
