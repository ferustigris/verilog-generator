#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE
class GraphBody;
class INode;

class Edge : public QGraphicsItem
{
public:
	enum States {
		OFF = 0,
		OK,
		WARNING,
		ALARM,
	};
    Edge(GraphBody *graphWidget, INode *sourceNode, INode *destNode);
    ~Edge();
    INode *sourceNode() const;
    INode *destNode() const;
    void setSourceNode(INode*);
    void setDestNode(INode*);
    void adjust();
    QStringList getWires() const;
    void setWires(const QStringList& wires);
    void addWires(const QStringList& wires);
    void delWires(const QStringList& wires);
    void Show();
    void Hide();
    void setState (const States newState);
    States getState () const;
protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
    void setTip();
	QPainterPath shape() const;
private:
    GraphBody *graph;
    INode *source, *dest;
    QPointF sourcePoint;
    QPointF destPoint;
	QPointF middle;
	qreal arrowSize;
    QStringList wires;
    States state;
};

#endif
