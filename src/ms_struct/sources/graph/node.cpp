#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QMessageBox>

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
Node::Node(GraphBody *graphWidget, INode* _parent, qint16 _id)
{
    name = new QGraphicsTextItem("root");
    setFlag(ItemIsMovable);
    graph = graphWidget;
    setCacheMode(DeviceCoordinateCache);
    setFlags(flags() | QGraphicsItem::ItemSendsGeometryChanges);
    setZValue(12);
    name->moveBy(20,10);
    name->setZValue(8);
    parent = _parent;
    id = _id;
    graphWidget->addItem(name);
    graphWidget->addItem(this);
}
/*!\func
 * ����������. ����� ������� �����
 * \param ���
 * \return ���
 */
Node::~Node()
{
    /*try{
        while(edgeListIn.count())
                delete edgeListIn.first();
        while(edgeListOut.count())
                delete edgeListOut.first();
        foreach(Node*n, shapesList.values())
                delete n;

        shapesList.clear();
        graph->removeItem(this);
        graph->removeItem(&name);
    } catch(...)
    {
    }*/
}
/*!\func
 * �������� �������
 * \param
 * - �������
 * \return ���
 */
void Node::addEdge(Edge *edge)
{
    if(edge->destNode() == this)
    {
        if(!edgeListIn.contains(edge))
            edgeListIn << edge;
    }
    else
    {
        if(!edgeListOut.contains(edge))
            edgeListOut << edge;
    }
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
    /*if(edge->sourceNode()->getType() == TOP_DEVICE)
    {
        foreach(INode*parent, edge->destNode()->nodes())
        {
            if(parent)
            {
                int i = 0;
                while(i < parent->edgesIn().count())
                {
                    Edge*e = parent->edgesIn().at(i);
                    if(e->sourceNode() == edge->sourceNode())
                    {
                        e->sourceNode()->delEdge(e);
                        e->destNode()->delEdge(e);
                        continue;
                    }
                    i++;
                }
            }
        }
    }*/
    if(edgeListIn.contains(edge))
        edgeListIn.removeAt(edgeListIn.indexOf(edge));
    if(edgeListOut.contains(edge))
        edgeListOut.removeAt(edgeListOut.indexOf(edge));
}
/*!\func
 * ������� ������ �������
 * \param ���
 * \return ������ �������
 */
QList<Edge *> Node::edgesIn() const
{
    return edgeListIn;
}
/*!\func
 * ������� ������ �������
 * \param ���
 * \return ������ �������
 */
QList<Edge *> Node::edgesOut() const
{
    return edgeListOut;
}
/*!\func
 * ������� ������ ������
 * \param ���
 * \return ������ �������
 */
QMap<qint16, INode* > Node::nodes() const
{
    return shapesList;
}
/*!\func
 * ������� ������ ������
 * \param ���
 * \return ������ �������
 */
QList<INode*> Node::getNodes() const
{
    static QList<INode*> list;
    list.clear();
    foreach(INode*n, shapesList.values())
        list.insert(0, n);
    return list;
}
/*!\func
 * �������� ����
 * \param ���
 * \return ��������� ��-��
 */
void Node::addNode(INode*node)
{
    //node->name->setPos(node->pos() + QPoint((int)(-node->name->boundingRect().width()/2), 10));
    if(shapesList.contains(node->getId()))
    {
        shapesList[node->getId()]->Hide();
        shapesList.remove(node->getId());
    }
    shapesList.insert(node->getId(),node);
}
/*!\func
 * �������� ����
 * \param ���
 * \return ��������� ��-��
 */
void Node::removeNode(const qint32 id)
{
    if(shapesList.contains(id))
    {
        shapesList[id]->remove();
        shapesList.remove(id);
    }
}
/*!\func
 * ������� ���� � �������
 * \param ���
 * \return ��������� ��-��
 */
QString Node::getHelp() const
{
	return help;
}
/*!\func
 * images for help
 * \param ���
 * \return ��������� ��-��
 */
QStringList Node::getHelpImages() const
{
	return images;
}
/*!\func
 * ��������� ���� � �������
 * \param ���
 * \return no
 */
void Node::setHelpFile(const QString&_help)
{
	helpFile = _help;
}
/*!\func
 * ������� ���� � �������
 * \param ���
 * \return ��������� ��-��
 */
QString Node::getHelpFile() const
{
	return helpFile;
}
/*!\func
 * ��������� ���� � �������
 * \param ���
 * \return no
 */
void Node::setHelp(const QString&_help, const QStringList&_images)
{
	help = _help;
	images = _images;
}
/*!\func
 * ������� ���� � �������
 * \param ���
 * \return ��������� ��-��
 */
QString Node::getShortDesc() const
{
	return desc;
}
/*!\func
 * ��������� ���� � �������
 * \param ���
 * \return no
 */
void Node::setShortDesc(const QString&_help)
{
    desc = _help;
    QString tmp("");
	int count = 0, lines = 0;
    foreach(QChar c, _help)
    {
        if(c == '\n')
        {
            count = 0;
            lines++;
        }
        tmp += c;
		if(count++ > 40)
        {
            count = 0;
            lines++;
            tmp  += '\n';
        }
		if((count > 30)&&(lines > 5))
        {
            tmp += "...";
            break;
        }
    }
    setToolTip(tmp);
}
/*!\func
 * ������� ��
 * \param ���
 * \return ��������� ��-��
 */
qint16 Node::getId()
{
    return id;
}
/*!\func
 * ����� ������� �������?
 * \param ���
 * \return ��������� ��-��
 */
QRectF Node::boundingRect() const
{
	return QRectF(-40, -30, 80, 60);
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
    if(graph->getCurIndex() == id)
    {
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(0xd0, 0xd0, 0xd0, 100));
		painter->drawRoundedRect(-20,-15, 40, 30, 5, 5);
    }
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::lightGray);
    if (option->state & QStyle::State_Sunken) {
            painter->drawEllipse(-8, -8, 20, 20);
    } else {
    painter->drawEllipse(-7, -7, 20, 20);
    }
    QRadialGradient gradient(-3, -3, 10);
    gradient.setColorAt(0, Qt::yellow);
    gradient.setColorAt(1, Qt::darkYellow);
    painter->setBrush(gradient);
    painter->setPen(QPen(Qt::black, 0));

    if (option->state & QStyle::State_Sunken) {
	painter->drawEllipse(-9, -9, 20, 20);
        if(!nodes().isEmpty())painter->drawImage(QRect(-8, -8, 18, 18), QImage(":/icons/down"));
    } else {
	painter->drawEllipse(-10, -10, 20, 20);
        if(!nodes().isEmpty())painter->drawImage(QRect(-9, -9, 18, 18), QImage(":/icons/down"));
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
    switch (change)
    {
    case ItemPositionHasChanged:
            break;
    case ItemPositionChange:
        foreach (Edge *edge, edgeListIn)
            edge->adjust();
        foreach (Edge *edge, edgeListOut)
            edge->adjust();
                name->moveBy(x() - name->x() - name->boundingRect().width()/2, y() - name->y() + 10);
        break;
    default:
        ;;
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
    } else {
        update();
    }
    QGraphicsItem::mousePressEvent(event);
}
/*!\func
 * ��������� �����
 * \param
 * - event - �������
 * \return ���
 */
void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    graph->setCurIndex(id);
    graph->on_actionHelp_triggered();
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
/*!\func TShape::getName
 * ������� ���
 * \param ���
 * \return ����� ���
 */
QString Node::getName() const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    return name->toPlainText();
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
 * ������ ��� ����������� ���������� ���-��
 * \param ���
 * \return ������
 */
INode*Node::getParent()
{
    return parent;
}
//! show on scene
void Node::Show()
{
    foreach(Edge * e, edgeListIn + edgeListOut)
	e->Show();
    show();
    name->setPos(pos());
    name->moveBy(x() - name->x() - name->boundingRect().width()/2, y() - name->y() + 10);
    name->show();
}
//! hide on scene
void Node::Hide()
{
    foreach(Edge * e, edgeListIn + edgeListOut)
        e->Hide();
    hide();
    name->hide();
}
//! remove
void Node::remove()
{
    Hide();
    foreach(Edge*e, edgeListIn)
        e->sourceNode()->delEdge(e);
    foreach(Edge*e, edgeListOut)
        e->destNode()->delEdge(e);
}
/*!\func
 * type of node
 * \params no
 * \return type of node
 */
TopTypes Node::getType() const
{
    return TOP_SIMPLE;
}
