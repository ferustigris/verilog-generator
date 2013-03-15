#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include "graphbody.h"
#include "errors.h"
#include "ui_graphbody.h"
#include "enterinputs.h"
#include "edge.h"
#include "tdialogadditem.h"
#include "node.h"
#include <math.h>
#include <QPointer>
#include <QMessageBox>
#include <QDateTime>
#include <edgeparams.h>

/*!\func TGraph::TGraph
 * ����������� ������-����-����, ��� ����� ���������� ����
 * \param
 * - parent - ��������
 * \return ���
 */
GraphBody::GraphBody(QWidget *parent) :
    QGraphicsView(parent),
    m_ui(new Ui::GraphBody)
{
    m_ui->setupUi(this);
    ppTopMenu.addAction(m_ui->actionAdd_relation);
    ppTopMenu.addAction(m_ui->actionEdit_state);
    ppTopMenu.addAction(m_ui->actionDelete_state);
    menuEdge.addAction(m_ui->actionEdit_edge);
    menuEdge.addAction(m_ui->actionRemove_relation);
    scene.setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(&scene);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    setResizeAnchor(AnchorUnderMouse);
    scaleView(1);
    scene.setSceneRect(200*factor, 100*factor, width(), height());
    setMinimumSize(400, 200);
    state = STATE_IDLE;
    currentIndex = currentEdge = -1;
    line = new QGraphicsLineItem();
    scene.addItem(line);
}
/*!\func
 * ���������� ������ �� ������
 * \param ���
 * \return ���
 */
void GraphBody::setData(Data*data)
{
    this->data = data;
    if(data)
    {
	QList<State> list = data->getStates();
	foreach(State state, list)
	    onAddState(state.getId());
	foreach(State state, list)
	{
	    QList<Relation> rels = data->getRelations(state.getId());
	    foreach(Relation rel, rels)
		onAddRelation(rel.getId());
	}
    }
    connect(data, SIGNAL(sAddState(const qint32)), this, SLOT(onAddState(const qint32)));
    connect(data, SIGNAL(sEditState(const qint32)), this, SLOT(onEditState(const qint32)));
    connect(data, SIGNAL(sRemoveState(const qint32)), this, SLOT(onRemoveState(const qint32)));
    connect(data, SIGNAL(sAddRelation(const qint32)), this, SLOT(onAddRelation(const qint32)));
    connect(data, SIGNAL(sEditRelation(const qint32)), this, SLOT(onEditRelation(const qint32)));
    connect(data, SIGNAL(sRemoveRelation(const qint32)), this, SLOT(onRemoveRelation(const qint32)));
}
/*!\func TGraph::TGraph
 * ����������
 * \param ���
 * \return ���
 */
GraphBody::~GraphBody()
{
    clear();
    delete m_ui;
}
/*!\func
 * ������� �������
 * \param
 * \return ���
 */
void GraphBody::removeItem(QGraphicsItem* i)
{
	scene.removeItem(i);
}
/*!\func
 * �������� �������
 * \param
 * \return ���
 */
void GraphBody::addItem(QGraphicsItem* i)
{
	scene.addItem(i);
}
/*!\func
 * �������  ����
 * \param
 * \return ���
 */
void GraphBody::ppMenu()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    ppTopMenu.exec(QCursor().pos());
}
/*!\func
 * �������  ����
 * \param
 * \return ���
 */
void GraphBody::EdgeMenu()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    menuEdge.exec(QCursor().pos());
}
/*!\func TGraph::clear
 * �������� ���
 * \param
 * \return ���
 */
void GraphBody::clear()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    //foreach(Node*node, shapesList)
//        delete node;
    shapesList.clear();
    edgesList.clear();
    scene.clear();
    line = new QGraphicsLineItem();
    scene.addItem(line);
}
/*!\func TGraph::mousePressEvent
 * ���������� ������� ������� ������ ����
 * \param
 * - event - �������
 * \return ���
 */
void GraphBody::mousePressEvent(QMouseEvent *event) {
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    switch(state)
    {
	case STATE_ADD_RELATION:
	    if(shapesList.contains(currentIndex))
	    {
		qint16 index = currentIndex;
		QGraphicsView::mousePressEvent(event);
		if(shapesList.contains(currentIndex))
			if(shapesList.contains(index))
				if(data)
					data->addRelation(index, currentIndex, "Empty");
	    }
	default:
	    currentIndex = currentEdge = -1;
	    state = STATE_IDLE;
    }
    QGraphicsView::mousePressEvent(event);
}
/*!\func GraphBody::mouseMoveEvent
 * ���������� ������� ����������� ����
 * \param
 * - event - �������
 * \return ���
 */
void GraphBody::mouseMoveEvent(QMouseEvent *event) {
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    switch(state) {
	case STATE_ADD_RELATION:
	    LOG(LOG_DEBUG, QString(__FUNCTION__)+": add relation");
	    if(shapesList.contains(currentIndex))
	    {
		    line->show();
		    QPointF p1(mapToScene(event->pos()));
		    Node *n = shapesList[currentIndex];
		    if(n)
			line->setLine(n->pos().x(), n->pos().y(), p1.x(), p1.y());
	    }
	    break;
	case STATE_IDLE:
	default:
	    line->hide();
    }
    QGraphicsView::mouseMoveEvent(event);
}
/*!\func GraphBody::addRelation
 * �������� �����
 * \param
 * \return ����� �������
 */
void GraphBody::onAddRelation(const qint32 id)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if(!data)return;
    if(shapesList.contains(data->getRelation(id).getSource().getId()) &&
	shapesList.contains(data->getRelation(id).getDest().getId()))
    {
	Node *source = shapesList[data->getRelation(id).getSource().getId()];
	Node *dest = shapesList[data->getRelation(id).getDest().getId()];
	if(dest && source )
	{
	    Edge * edge = new Edge(id, this, source, dest);
	    edgesList.insert(id, edge);
	    edge->setName(data->getRelation(id).getCondition());
	}
    }
}
/*!\func
 * �������� ������� �����
 * \param
 * - parent - ������������ �������
 * \return ����� �������
 */
void GraphBody::onAddState(const qint32 id)
{
	LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
	if(!data)return;
	if(data->getState(id).getId() < 0)return;
	Node*n = new Node(this, id);
	if(data->getState(id).getPosition() == QPoint())
	{
                quint32 rand1 = QTime::currentTime().msec();
                quint32 rand2 = rand1*rand1%1000;
                qreal w = - mapToScene(x(), y()).x() + mapToScene(width(), y()).x();
                qreal h = - mapToScene(x(), y()).y() + mapToScene(x(), height()).y();
                qreal posx = mapToScene(x(), y()).x() + w/1000*rand1;
                qreal posy = mapToScene(x(), y()).y() + h/1000*rand2;
                n->setPos(posx, posy);
	}
	else
		n->setPos(data->getState(id).getPosition());
	n->setName(data->getState(id).getName());
	n->setToolTip(data->getState(id).getDescription());
	shapesList.insert(id, n);
}
/*!\func
 * ������� � ��������� ���������� ����� �����
 * \param ���
 * \return ���
 */
void GraphBody::on_actionAdd_relation_triggered()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    state = STATE_ADD_RELATION;
}
/*!\func
 * ��������� ��� ����
 * \param
 * - ���������� ��� ���������
 * - ������� ���������
 * \return ���
 */
void GraphBody::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    return;
    QLinearGradient gradient(rect.topLeft(), rect.bottomRight());
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::lightGray);
    painter->fillRect(rect.intersect(rect), gradient);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(rect);
}
/*!\func
 * ������� ��������
 * \param
 * - �������
 * \return ���
 */
void GraphBody::wheelEvent(QWheelEvent *event)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    scaleView(pow(2., -event->delta() / 240.0));
}
/*!\func
 * ����������
 * \param
 * - ���������
 * \return ���
 */
void GraphBody::scaleView(qreal scaleFactor)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    factor = scaleFactor;//matrix().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
	return;

    scale(scaleFactor, scaleFactor);
}
/*!\func
 * ���������� ������� ������
 * \param
 * - ����� ��
 * \return ���
 */
void GraphBody::setCurIndex(qint16 newInd)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    currentIndex = newInd;
}
/*!\func
 * ���������� ������� ������
 * \param
 * - ����� ��
 * \return ���
 */
void GraphBody::setCurrentEdge(qint16 newInd)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    currentEdge = newInd;
}
/*!\func
 * ��������� �������
 * \param
 * - ��
 * \return ���
 */
void GraphBody::changeNode(const qint16 id)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    static TDialogAddItem dial(data, this);
    if(shapesList.contains(id))
	    dial.showModal(id);
}
/*!\func
 * ������������� �������
 * \param
 * - ��
 * \return ���
 */
void GraphBody::moveNode(const qint16 id)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if(!shapesList.contains(id))return;
    Node *p = shapesList[id];
    if(p&&data)
	data->setStatePosition(id, p->pos().toPoint());
}
/*!\func
 * �������� ���� � ����������� ��������\�����
 * \param ���
 * \return ���
 */
void GraphBody::changeEdge(const qint32 id)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    static EdgeParams edgeWindow(data);
    edgeWindow.move(QCursor().pos());
    edgeWindow.showModal(id);
    edgeWindow.activateWindow();
}
/*!\func
 * ���������� ���������� ������
 * \param
 * \return
 */
void GraphBody::onEditRelation(const qint32 id)
{
	if(!data)return;
	if(edgesList.contains(id))
	{
		edgesList[id]->setName(data->getRelation(id).getCondition());
		if(edgesList[id]->getDestId() != data->getRelation(id).getDest().getId())
			edgesList[id]->setDest(shapesList[data->getRelation(id).getDest().getId()]);
	}
}
/*!\func
 * ���������� ���������� ���������
 * \param
 * \return
 */
void GraphBody::onEditState(const qint32 id)
{
    if(!data)return;
    if(shapesList.contains(id))
    {
	    shapesList[id]->setName(data->getState(id).getName());
	    shapesList[id]->setToolTip(data->getState(id).getDescription());
    }

}
/*!\func
 * ���������� �������� ���������
 * \param
 * \return
 */
void GraphBody::onRemoveState(const qint32 id)
{
    if(!data)return;
    if(shapesList.contains(id))
    {
        shapesList[id]->remove();
	shapesList.remove(id);
    }
}
/*!\func
 * ���������� �������� �����
 * \param
 * \return
 */
void GraphBody::onRemoveRelation(const qint32 id)
{
    if(!data)return;
    if(edgesList.contains(id))
    {
        edgesList[id]->remove();
	edgesList.remove(id);
    }
}
/*!\func
 * ������� ���������!
 * \param
 * \return
 */
void GraphBody::on_actionDelete_state_triggered()
{
	if((data)&&(currentIndex >= 0))
		data->removeState(currentIndex);
}
/*!\func
 * ������� �����
 * \param
 * \return
 */
void GraphBody::on_actionRemove_relation_triggered()
{
	if((data)&&(currentEdge >= 0))
		data->removeRelation(currentEdge);
}
/*!\func
 * ������������� �����
 * \param
 * \return
 */
void GraphBody::on_actionEdit_edge_triggered()
{
	if(edgesList.contains(currentEdge))
                changeEdge(currentEdge);
}
/*!\func
 * ������������� ���������
 * \param
 * \return
 */
void GraphBody::on_actionEdit_state_triggered()
{
    if(shapesList.contains(currentIndex)&&data)
	    changeNode(currentIndex);
}
/*!\func
 * ������� ���������
 * \param
 * \return
 */
void GraphBody::on_actionRemove_state_triggered()
{
    if(shapesList.contains(currentIndex)&&data)
    {
        shapesList[currentIndex]->remove();
        shapesList.remove(currentIndex);
    }
}
