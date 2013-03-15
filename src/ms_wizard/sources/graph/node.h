#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QList>

class Edge;
class GraphBody;
QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

class Node : public QGraphicsItem
{
public:
    Node(GraphBody *graphWidget, qint16 _id);
    ~Node();
    void addEdge(Edge *edge);
    void delEdge(Edge *edge);
    enum { Type = UserType + 1 };
    int type() const { return Type; }
    QRectF boundingRect() const;
    void setName(const QString&);
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    qint16 getId();
    void remove();
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
private:
    QList<Edge *> edgeList;
    GraphBody *graph;
    QGraphicsTextItem *name;
    qint16 id;
};

#endif
