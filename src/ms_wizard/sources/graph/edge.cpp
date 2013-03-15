#include <QPainter>
#include "edge.h"
#include "node.h"
#include <math.h>
#include <QGraphicsSceneMouseEvent>

#include <graphbody.h>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

Edge::Edge(const qint16 _id, GraphBody *graphWidget, Node *sourceNode, Node *destNode)
    : arrowSize(10)
{
    name = new QGraphicsTextItem("Empty");
    //setAcceptedMouseButtons(0);
    source = sourceNode;
    dest = destNode;
    source->addEdge(this);
    dest->addEdge(this);
    setZValue(8);
    name->setZValue(6);
    setFlag(ItemIsSelectable);
    adjust();
    graph = graphWidget;
    graph->addItem(this);
    graph->addItem(name);
    size = 1;
    id = _id;
}

Edge::~Edge()
{
    if(dest)
        dest->delEdge(this);
    if(source)
        source->delEdge(this);
        /*graph->removeItem(&name);
        graph->removeItem(this);*/
}
void Edge::remove()
{
        dest->delEdge(this);
        source->delEdge(this);
        hide();
        name->hide();
}
void Edge::adjust()
{
    if (!source || !dest)
	return;
    QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
    qreal length = line.length();
    prepareGeometryChange();
    if (!qFuzzyCompare(length, qreal(0.0))) {
	QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);
	sourcePoint = line.p1() + edgeOffset;
	destPoint = line.p2() - edgeOffset;
    } else {
	sourcePoint = destPoint = line.p1();
    }
}

QRectF Edge::boundingRect() const
{
    if (!source || !dest)
	return QRectF();
    if(sourcePoint == destPoint)
    {
	return QRectF(destPoint.x() - 28, destPoint.y() - 28, 30, 30);
    }
    qreal penWidth = 1;
    qreal extra = (penWidth + arrowSize) / 2.0;
    return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
				      destPoint.y() - sourcePoint.y()))
	.normalized()
	.adjusted(-extra, -extra, extra, extra);
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!source || !dest)
	return;
    // Draw the line itself
    if(sourcePoint == destPoint)
    {
	painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	/*QPainterPath path;
	path.moveTo(destPoint  + QPoint(-5, -10));
	path.arcTo(destPoint.x() - 5, destPoint.y() - 20, 10, 20, 0, 180 );
	painter->drawPath(path);*/
	painter->drawEllipse((int)(destPoint.x() - 28), (int)(destPoint.y() - 28), 30, 30);
        name->setPos((destPoint+sourcePoint)/2 + QPoint(-40,-50));
    }
    else
    {
	QLineF line(sourcePoint, destPoint);
	painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter->drawLine(line);
	// Draw the arrows if there's enough room
	double angle = ::acos(line.dx() / line.length());
	if (line.dy() >= 0)
	    angle = TwoPi - angle;
	QPointF destArrowP1 = destPoint + QPointF(sin(angle - Pi / 3) * arrowSize,
						  cos(angle - Pi / 3) * arrowSize);
	QPointF destArrowP2 = destPoint + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
						  cos(angle - Pi + Pi / 3) * arrowSize);
	painter->setBrush(Qt::black);
	painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
        name->setPos((destPoint+sourcePoint)/2);
    }
    name->setDefaultTextColor(Qt::gray);
}
void Edge::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event )
{
    if(graph)
        graph->changeEdge(id);
    QGraphicsItem::mouseDoubleClickEvent(event);
}
/*!\func TShape::getName
 * вернуть имя
 * \param нет
 * \return текст имя
 */
QString Edge::getNameS() {
    return name->toPlainText();
}
/*!\func TShape::setName
 * установить новое имя
 * \param нет
 * \return текст имя
 */
void Edge::setName(const QString& newName)
{
    name->setPlainText(newName);
}
/*!\func TShape::getName
 * вернуть имя
 * \param нет
 * \return текст имя
 */
int Edge::getSize() {
    return size;
}
/*!\func TShape::setName
 * установить новое имя
 * \param нет
 * \return текст имя
 */
void Edge::setSize(const int newSize)
{
    if((newSize < 1))return;
    size = newSize;
}
/*!\func
 * вернуть ID
 * \param нет
 * \return ID
 */
qint16 Edge::getId()
{
    return id;
}
/*!\func
 * изменения!!!
 * \param нет
 * \return нет
 */
void Edge::change()
{
    //graph->changeEdge(this);
}
/*!\func
 * отдать пункт назначения
 * \param нет
 * \return нет
 */
qint32 Edge::getDestId() const
{
	return dest->getId();
}
/*!\func
 * установить состояние Б
 * \param нет
 * \return нет
 */
void Edge::setDest(Node*dest)
{
	this->dest->delEdge(this);
	this->dest = dest;
	dest->addEdge(this);
}
/*!\func
 * подцепить мышью
 * \param
 * - event - событие
 * \return нет
 */
void Edge::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    graph->setCurrentEdge(id);
    if(event->buttons() & Qt::RightButton)
    {
	graph->EdgeMenu();
	return;
    } else {
	update();
    }
    QGraphicsItem::mousePressEvent(event);
}
