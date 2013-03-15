#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>
QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE
class GraphBody;
class Node;

class Edge : public QGraphicsItem
{
public:
    Edge(const qint16 _id, GraphBody *graphWidget, Node *sourceNode, Node *destNode);
    ~Edge();
    enum { Type = UserType + 1 };
    int type() const { return Type; }
    void adjust();
    QString getNameS();
    void setName(const QString& newName);
    int getSize();
    void setSize(const int newSize);
    qint32 getDestId() const;
    void setDest(Node*dest);
    qint16 getId();
    void change();
    void remove();
protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
private:
    GraphBody *graph;
    Node *source, *dest;
    QPointF sourcePoint;
    QPointF destPoint;
    qreal arrowSize;
    QGraphicsTextItem *name;
    int size;
    qint16 id;
};

#endif
