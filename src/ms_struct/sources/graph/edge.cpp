#include <QPainter>
#include "edge.h"
#include <math.h>
#include <QGraphicsSceneMouseEvent>
#include "inode.h"

#include <graphbody.h>

/*!\func
 * constructor
 * \params
 * - graphBody - graph
 * - sourceNode - node 1
 * - detNode - node 2
 * \return no
 */
Edge::Edge(GraphBody *graphWidget, INode *sourceNode, INode *destNode)
    : arrowSize(10)
{
    //setAcceptedMouseButtons(0);
    source = sourceNode;
    dest = destNode;
    source->addEdge(this);
    dest->addEdge(this);
    setZValue(10);
    setFlag(ItemIsSelectable);
    adjust();
    graph = graphWidget;
    state = OFF;
    graph->addItem(this);
}
/*!\func
 * desructor
 * \params no
 * \return no
 */
Edge::~Edge()
{
    /*try
    {
        #warning: TODO: why halting?
        if(source)
            source->delEdge(this);
        if(dest)
            dest->delEdge(this);
        graph->removeItem(this);
    }catch(...)
    {
    }*/
}
/*!\func
 * sourceNode
 * \params no
 * \return no
 */
INode *Edge::sourceNode() const
{
    return source;
}
/*!\func
 * destNode
 * \params no
 * \return no
 */
INode *Edge::destNode() const
{
    return dest;
}
/*!\func
 * calc size
 * \params no
 * \return no
 */
void Edge::adjust()
{
	if (!sourceNode() || !destNode())
		return;
	prepareGeometryChange();
	sourcePoint = sourceNode()->pos();//line.p1() + edgeOffset;
	destPoint = destNode()->pos();//line.p2() - edgeOffset;
	middle = QPointF(destPoint.x(), sourcePoint.y());
}
/*!\func
 * calc size
 * \params no
 * \return no
 */
QRectF Edge::boundingRect() const
{
	if (!sourceNode() || !destNode())
		return QRectF();
	if(sourcePoint == destPoint)
	{
		return QRectF(destPoint.x() - 28, destPoint.y() - 28, 30, 30);
	}
	QPointF leftTop(qMin(sourcePoint.x(), destPoint.x())-arrowSize, qMin(sourcePoint.y(), destPoint.y())-arrowSize);
	QSizeF size(abs(sourcePoint.x() - destPoint.x())+2*arrowSize, abs(sourcePoint.y() - destPoint.y())+2*arrowSize);
	return QRectF(leftTop, size);
}
/*!\func
 * figure of edge
 * \param no
 * \return figure
 */
QPainterPath Edge::shape() const
{
	QPainterPath path;
	path.moveTo(destPoint + QPointF(arrowSize,0));
	path.lineTo(middle + QPointF(arrowSize,arrowSize));
	path.lineTo(sourcePoint + QPointF(0,arrowSize));
	path.lineTo(sourcePoint + QPointF(0,-arrowSize));
	path.lineTo(middle + QPointF(-arrowSize,-arrowSize));
	path.lineTo(destPoint + QPointF(-arrowSize,0));
	return path;
}
/*!\func
 * draw edge
 * \params no
 * \return no
 */
void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	if (!sourceNode() || !destNode())return;
	QColor color (Qt::black);
	switch (getState()) {
	case OFF:
		color = Qt::gray;
		break;
	case OK:
		color = Qt::green;
		break;
	case WARNING:
	default:
		color = Qt::red;
	}
	// Draw the line itself
	if(sourceNode()->getId() == destNode()->getId())
	{
		painter->setPen(QPen(color, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
		painter->drawEllipse(destPoint.x() - 28, destPoint.y() - 28, 30, 30);
		painter->setPen(Qt::black);
		//painter->drawText(destPoint+QPoint(-28,-14), getData());
	}
	else
	{
		painter->setPen(QPen(color, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
		float width = destNode()->boundingRect().width();
		float height = destNode()->boundingRect().height();
		QRectF rect(destNode()->pos().x() - width/2, destNode()->pos().y() - height/2, width, height);
		width = sourceNode()->boundingRect().width();
		height = sourceNode()->boundingRect().height();
		//QRectF rects(sourceNode()->pos().x() - width/2, sourceNode()->pos().y() - height/2, width, height);
		painter->setBrush(color);
		if(rect.contains(middle))
		{//arrow enter in node by left or right
			int sign = destPoint.x() > sourcePoint.x() ? 1 : -1;
			//QMessageBox::information(0,"","arrow enter in node by left or right");
			QPointF destArrowP1 = QPointF(destPoint.x()- sign*12, middle.y());
			QPointF destArrowP2 = QPointF(destPoint.x()- sign*12 - sign*arrowSize, middle.y()-arrowSize/2);
			QPointF destArrowP3 = QPointF(destPoint.x()- sign*12 - sign*arrowSize, middle.y()+arrowSize/2);
			painter->drawPolygon(QPolygonF() << destArrowP3 << destArrowP1 << destArrowP2);
		}
		else
		{//arrow enter in node by top or bottom
			int sign = destPoint.y() > sourcePoint.y() ? 1 : -1;
			//QMessageBox::information(0,"","arrow enter in node by left or right");
			QPointF destArrowP1 = QPointF(destPoint.x(), destPoint.y()- sign*12);
			QPointF destArrowP2 = QPointF(destPoint.x()-arrowSize/2, destPoint.y()- sign*12 - sign*arrowSize);
			QPointF destArrowP3 = QPointF(destPoint.x()+arrowSize/2, destPoint.y()- sign*12 - sign*arrowSize);
			painter->drawPolygon(QPolygonF() << destArrowP3 << destArrowP1 << destArrowP2);
		}
		/*if(rects.contains(middle))
			name->setPos((sourcePoint + destPoint)/2);
		else
		{
			if(sourcePoint.x() < destPoint.x())
				name->setPos(sourcePoint + QPointF(sourceNode()->boundingRect().width()/2,-name->boundingRect().height()));
			else
				name->setPos(sourcePoint + QPointF(-name->boundingRect().width()-sourceNode()->boundingRect().width()/2,-name->boundingRect().height()));
		}
		//painter->drawText((destPoint + sourcePoint)/2, getData());
		*/
		painter->drawLine(destPoint, middle);
		painter->drawLine(middle, sourcePoint);
	}
}
void Edge::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event )
{
    if(graph)
	graph->changeEdge(this);
    QGraphicsItem::mouseDoubleClickEvent(event);
}
//! show on scene
void Edge::Show()
{
    adjust();
    show();
}
//! hide on scene
void Edge::Hide()
{
    hide();
}
//! get copy wires list
QStringList Edge::getWires() const
{
	return wires;
}
/*!\func
 * set wires list
 * \params
 * - wires - list of wires
 * \return no
 */
void Edge::setWires(const QStringList& wires)
{
    QStringList added, deleted;
    foreach(QString newWire, wires)
    {
        bool present = false;
        foreach(QString oldWire, this->wires)
        {
            if(oldWire.section("|", 0, 0) == newWire.section("|", 0, 0))
            {
                present = true;
                break;
            }
        }
        if(!present)added.append(newWire);
    }
    foreach(QString oldWire, this->wires)
    {
        bool present = false;
        foreach(QString newWire, wires)
        {
            if(oldWire.section("|", 0, 0) == newWire.section("|", 0, 0))
            {
                present = true;
                break;
            }
        }
        if(!present)deleted.append(oldWire);
    }

    this->wires.clear();
    this->wires.append(wires);
    setState(WARNING);
    if (this->wires.count())setState(OK);
    if(source->getType() == TOP_DEVICE)
        foreach(INode*node, dest->nodes())
            foreach(Edge*edge, node->edgesIn())
                if(edge->source == source)
                    edge->delWires(deleted);
    if(dest->getType() == TOP_DEVICE)
        foreach(INode*node, source->nodes())
            foreach(Edge*edge, node->edgesOut())
                if(edge->dest == dest)
                    edge->delWires(deleted);
    if(source->getType() == TOP_DEVICE)
    {
        INode*parent = dest->getParent();
        if(parent)
            foreach(Edge*edge, parent->edgesIn())
                if(edge->source == source)
                    edge->addWires(added);
    }
    if(dest->getType() == TOP_DEVICE)
    {
        INode*parent = source->getParent();
        if(parent)
            foreach(Edge*edge, parent->edgesOut())
                if(edge->dest == dest)
                    edge->addWires(added);
    }
    setTip();
}
/*!\func
 * add wires list
 * \params
 * - wires - list of wires
 * \return no
 */
void Edge::addWires(const QStringList& wires)
{
    QStringList added;
    foreach(QString str, wires)
    {
        bool present = false;
        foreach(QString wire, this->wires)
        {
            if(str.section("|", 0, 0) == wire.section("|", 0, 0))
            {
                present = true;
                break;
            }
        }
        if(!present)added.append(str);
    }
    this->wires.append(added);
    if(source->getType() == TOP_DEVICE)
        if(INode*p = dest->getParent())
            if(p->getType() != TOP_ROOT)
            foreach(Edge*edge, p->edgesIn())
                if(edge->sourceNode() == source)
                    edge->addWires(added);
    if(dest->getType() == TOP_DEVICE)
        if(INode*p = source->getParent())
            if(p->getType() != TOP_ROOT)
            foreach(Edge*edge, p->edgesOut())
                if(edge->destNode() == dest)
                    edge->addWires(added);
    setState(WARNING);
    if (this->wires.count())setState(OK);
    setTip();
}
/*!\func
 * set tool tip
 * \params no
 * \return no
 */
void Edge::setTip()
{
    QString tip("");
    int count = 0;
    foreach(QString str, this->wires)
    {
        QStringList list = str.split("|");
        if(list.count() < 4)continue;
        QString tmp(list[0] + "(" + list[3] + "), ");
        tip += tmp;
        count += tmp.length();
        if(tip.length() > 80)
        {
            tip += "...";
            break;
        }
        else
        if(count > 20)
        {
            tip += "\n";
            count = 0;
        }
    }
    setToolTip(tip);
}
/*!\func
 * del wires list
 * \params
 * - wires - list of wires
 * \return no
 */
void Edge::delWires(const QStringList& wires)
{
    foreach(QString str, wires)
    {
        bool present = false;
        int i = 0;
        foreach(QString wire, this->wires)
        {
            if(str.section("|", 0, 0) == wire.section("|", 0, 0))
            {
                present = true;
                break;
            }
            i++;
        }
        if(present)
            this->wires.removeAt(i);
    }
    if(source->getType() == TOP_DEVICE)
        foreach(INode*node, dest->nodes())
            foreach(Edge*edge, node->edgesIn())
                if(edge->sourceNode() == source)
                    edge->delWires(wires);
    if(dest->getType() == TOP_DEVICE)
        foreach(INode*node, source->nodes())
            foreach(Edge*edge, node->edgesOut())
                if(edge->destNode() == dest)
                    edge->delWires(wires);
    setState(WARNING);
    if (this->wires.count())setState(OK);
    setTip();
}
//! state of edge
void Edge::setState (const States newState)
{
	state = newState;
	this->adjust();
}
//! get cur state
Edge::States Edge::getState () const
{
	return state;
}
/*!\func
 * деструктор
 * \params
 * - newValue - new source
 * \return no
 */
void Edge::setSourceNode(INode*newValue)
{
    source = newValue;
}
/*!\func
 * set dest node
 * \params
 * - newValue - new dest
 * \return no
 */
void Edge::setDestNode(INode*newValue)
{
    dest = newValue;
}
