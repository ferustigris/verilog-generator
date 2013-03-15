#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

#include "edge.h"
#include "node.h"
#include "errors.h"
#include "graphbody.h"

/*!\func
 * �����������. ����� ������� �����
 * \param
 * - ��������
 * \return ���
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
 * ����������. ����� ������� �����
 * \param ���
 * \return ���
 */
Node::~Node()
{
        /*while(edgeList.count())
		delete edgeList.first();
	graph->removeItem(this);
        graph->removeItem(&name);*/
}
/*!\func
 * �������!
 * \param ���
 * \return ���
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
 * �������� �������
 * \param
 * - �������
 * \return ���
 */
void Node::addEdge(Edge *edge)
{
    edgeList << edge;
    edge->adjust();
}
/*!\func
 * ������� �������
 * \param
 * - �������
 * \return ���
 */
void Node::delEdge(Edge *edge)
{
	int id = edgeList.indexOf(edge);
	if((edgeList.count() > id)&&(id >= 0))
		edgeList.removeAt(id);
}
/*!\func
 * ����� ������� �������?
 * \param ���
 * \return ��������� ��-��
 */
QRectF Node::boundingRect() const
{
    qreal adjust = 2;
    return QRectF(-10 - adjust, -10 - adjust,
		  23 + adjust, 23 + adjust);
}
/*!\func
 * �����������
 * \param
 * - ���������� ���������
 * - �����
 * - ��� ����������� �� ������������
 * \return ���
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
 * ��������� ����
 * \param
 * - ��� ���������
 * - ����� ��������
 * \return ���
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
 * ��������� �����
 * \param
 * - event - �������
 * \return ���
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
 * ��������� ����
 * \param
 * - event - �������
 * \return ���
 */
void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
/*!\func TShape::setName
 * ���������� ���
 * \param
 * - newN - ����� ���
 * \return ���
 */
void Node::setName(const QString&newN) {
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    name->setPlainText(newN);
}
/*!\func
 * ������ ��� ����������� ���������� ���-��
 * \param ���
 * \return ������
 */
QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(-10,-10, 20,20);
    return path;
}
/*!\func
 * ������� ID
 * \param ���
 * \return ID
 */
qint16 Node::getId()
{
    return id;
}
