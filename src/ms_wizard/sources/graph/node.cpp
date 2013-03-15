#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

#include "edge.h"
#include "node.h"
#include "errors.h"
#include "graphbody.h"

/*!\func
 * конструктор. класс вершины графа
 * \param
 * - родитель
 * \return нет
 */
Node::Node(GraphBody *graphWidget, qint16 _id)
    : graph(graphWidget)
{
        name = new QGraphicsTextItem("");
	setFlag(ItemIsMovable);
	setZValue(10);
	setFlags(flags() | QGraphicsItem::ItemSendsGeometryChanges);
        name->moveBy(20,20);
        name->setZValue(12);
	id = _id;
	graph->addItem(this);
        graph->addItem(name);
}
/*!\func
 * деструктор. класс вершины графа
 * \param нет
 * \return нет
 */
Node::~Node()
{
        /*while(edgeList.count())
		delete edgeList.first();
	graph->removeItem(this);
        graph->removeItem(&name);*/
}
/*!\func
 * удалить!
 * \param нет
 * \return нет
 */
void Node::remove()
{
        for(int i = 0; i < edgeList.count();i++)
                edgeList[i]->remove();
        this->hide();
        name->hide();
        edgeList.clear();
}
/*!\func
 * добавить стрелку
 * \param
 * - стрелка
 * \return нет
 */
void Node::addEdge(Edge *edge)
{
    edgeList << edge;
    edge->adjust();
}
/*!\func
 * удалить стрелку
 * \param
 * - стрелка
 * \return нет
 */
void Node::delEdge(Edge *edge)
{
	int id = edgeList.indexOf(edge);
	if((edgeList.count() > id)&&(id >= 0))
		edgeList.removeAt(id);
}
/*!\func
 * какую площадь займете?
 * \param нет
 * \return занимемая пл-дь
 */
QRectF Node::boundingRect() const
{
    qreal adjust = 2;
    return QRectF(-10 - adjust, -10 - adjust,
		  23 + adjust, 23 + adjust);
}
/*!\func
 * перерисовка
 * \param
 * - устройство отрисовки
 * - стиль
 * - при кэшировании не используется
 * \return нет
 */
void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::darkGray);
    if (option->state & QStyle::State_Sunken) {
	painter->drawEllipse(-8, -8, 20, 20);
    } else {
	painter->drawEllipse(-7, -7, 20, 20);
    }
    QRadialGradient gradient(-3, -3, 10);
    gradient.setColorAt(0, Qt::green);
    gradient.setColorAt(1, Qt::darkGreen);
    painter->setBrush(gradient);
    painter->setPen(QPen(Qt::black, 0));

    if (option->state & QStyle::State_Sunken) {
	painter->drawEllipse(-9, -9, 20, 20);
    } else {
	painter->drawEllipse(-10, -10, 20, 20);
    }
}
/*!\func
 * отпустить мышь
 * \param
 * - тип изменения
 * - новое значение
 * \return нет
 */
QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
	foreach (Edge *edge, edgeList)
	    edge->adjust();
        name->moveBy(x() - name->x() - name->boundingRect().width()/2, y() - name->y() + 15);
	graph->moveNode(id);
	break;
    default:
	break;
    };
    return QGraphicsItem::itemChange(change, value);
}
/*!\func
 * подцепить мышью
 * \param
 * - event - событие
 * \return нет
 */
void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    graph->setCurIndex(id);
    if(event->buttons() & Qt::RightButton)
    {
	graph->ppMenu();
	return;
    } else {
	update();
    }
    QGraphicsItem::mousePressEvent(event);
}
void Node::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event )
{
    if(graph)
	graph->changeNode(id);
    QGraphicsItem::mouseDoubleClickEvent(event);
}
/*!\func
 * отпустить мышь
 * \param
 * - event - событие
 * \return нет
 */
void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
/*!\func TShape::setName
 * установить имя
 * \param
 * - newN - новое имя
 * \return нет
 */
void Node::setName(const QString&newN) {
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    name->setPlainText(newN);
}
/*!\func
 * фигура для определения занимаемой пло-ди
 * \param нет
 * \return фигура
 */
QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(-10,-10, 20,20);
    return path;
}
/*!\func
 * вернуть ID
 * \param нет
 * \return ID
 */
qint16 Node::getId()
{
    return id;
}
