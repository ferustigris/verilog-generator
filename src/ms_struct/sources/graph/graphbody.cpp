#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include "graphbody.h"
#include "errors.h"
#include "ui_graphbody.h"
#include "enterinputs.h"
#include "edge.h"
#include "node.h"
#include "nodedev.h"
#include "nodefsm.h"
#include <math.h>
#include <QProcess>
#include <edgeparams.h>
#include <QMessageBox>
#include <QTextStream>
#include "constants.h"
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QTime>
#include "nodesfactory.h"

extern QString program_dir;
extern QString config_path;

/*!\func TGraph::TGraph
 *  --,
 * \param
 * - parent -
 * \return
 */
GraphBody::GraphBody(EnterInputs *parent, const QString& proj_name, const QString& wizard_name) :
    QGraphicsView(parent),
    m_ui(new Ui::GraphBody)
    //settings(config_path + "/" + FileNames::project_config_file, QSettings::IniFormat, parent)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    m_ui->setupUi(this);
    nodesFactory = new NodesFactory(this);
    edgeWindow = new EdgeParams(this);
    line = new QGraphicsLineItem;
    scene.setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(&scene);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    setResizeAnchor(AnchorViewCenter);
    scaleView(1);
    //scene.setSceneRect(200*factor, 100*factor, width(), height());
    setMinimumSize(400, 400);
    state = STATE_IDLE;
    currentIndex = -1;
    scene.addItem(line);
    rootNode = nodesFactory->newRoot();
    tmpProject = false;
    this->parent = parent;
    node = rootNode;
    project_path = proj_name;
    if(project_path.isEmpty())
    {
        QDir dir;
        do
        {
            project_path = dir.tempPath() + "/tmp_" + QTime::currentTime().toString("mm.ss.zzz");
        } while(dir.exists(project_path));
        dir.mkpath(project_path);
        mustBeRemoved<<project_path;
        tmpProject = true;
    }
    ms_wizard_path = wizard_name;
    Project project(this, nodesFactory, node, project_path + "/", FileNames::project_main_file);
    max_id = project.load();
    foreach(INode* n, node->nodes())
        n->Show();
    setLevelsPath();
    if(parent)parent->change(false);
}
/*!\func
 * remove path
 * \params
 * - path - directory
 * \return
 */
void rmdir(const QString path)
{
    QDir dir(path);
	dir.setCurrent(path);
	foreach(QFileInfo entry, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries))
    {
        if(entry.isDir())
            rmdir(entry.absoluteFilePath());
        else
            dir.remove(entry.absoluteFilePath());
    }
	dir.setCurrent("..");
	dir.rmdir(path);
}
/*!\func TGraph::TGraph
 *
 * \param
 * \return
 */
GraphBody::~GraphBody()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    foreach(QString tmp, mustBeRemoved)
        rmdir(tmp);
    delete nodesFactory;
    delete m_ui;
}
/*!\func
 *
 * \param no
 * \return
 */
void GraphBody::setProjectName(const QString&path)
{
    project_path = path;
}
/*!\func
 *
 * \param no
 * \return
 */
void GraphBody::ppMenu()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    ppTopMenu.clear();
    ppTopMenu.addAction(m_ui->actionAdd_relation);
    if(node->nodes().contains(currentIndex))
    {
        ppTopMenu.addAction(m_ui->actionLevel_down);
        ppTopMenu.addAction(m_ui->actionState_machine);
    }
    ppTopMenu.addAction(m_ui->actionRemove_state);
    ppTopMenu.addAction(m_ui->actionHelp);
    ppTopMenu.exec(QCursor().pos());
}
/*!\func TGraph::clear
 *
 * \param no
 * \return
 */
void GraphBody::clear()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    QMapIterator<qint16, INode* > i(node->nodes());
    while(i.hasNext()) {
	i.next();
        i.value()->Hide();
    }
}
/*!\func TGraph::mousePressEvent
 *
 * \param
 * - event -
 * \return
 */
void GraphBody::mousePressEvent(QMouseEvent *event) {
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    switch(state)
    {
	case STATE_ADD_RELATION:
            if((node->nodes().contains(currentIndex))||(devices.contains(currentIndex)))
	    {
		qint16 index = currentIndex;
		QGraphicsView::mousePressEvent(event);
		if(index != currentIndex)
		    addRelation(index, currentIndex);
	    }
	default:
	    currentIndex = -1;
            foreach(INode*n, node->nodes())
                    n->Hide(), n->Show();
	    state = STATE_IDLE;
    }
    QGraphicsView::mousePressEvent(event);
}
/*!\func GraphBody::mouseMoveEvent
 *
 * \param
 * - event -
 * \return
 */
void GraphBody::mouseMoveEvent(QMouseEvent *event) {
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    switch(state) {
	case STATE_IDLE:
            line->hide();
            if(parent&&(event->buttons()&Qt::LeftButton)&&(node->nodes().contains(currentIndex)||devices.contains(currentIndex)))parent->change(true);
            break;
	case STATE_ADD_RELATION:
	    LOG(LOG_DEBUG, QString(__FUNCTION__)+": add relation");
	    if(node->nodes().contains(currentIndex))
	    {
		line->show();
		QPointF p1(mapToScene(event->pos()));
                INode*n = node->nodes()[currentIndex];
                if(!n)break;
                line->setLine(n->pos().x(), n->pos().y(), p1.x(), p1.y());
	    }
            else
            if(devices.contains(currentIndex))
            {
                line->show();
                QPointF p1(mapToScene(event->pos()));
                INode*n = devices[currentIndex];
                if(!n)break;
                line->setLine(n->pos().x(), n->pos().y(), p1.x(), p1.y());
            }
	    break;
    }
    QGraphicsView::mouseMoveEvent(event);
}
/*!\func GraphBody::addRelation
 *
 * \param
 * - index -
 * - relationWith -
 * \return
 */
bool GraphBody::addRelation(const qint16& index,const qint16& relationWith)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if(node->nodes().contains(index) &&
	node->nodes().contains(relationWith))
    {
        INode* source = node->nodes()[index];
        INode* dest = node->nodes()[relationWith];
	if(dest && source )
	{
                Edge* e = new Edge(this, source, dest);
				e->setWires(QStringList());//<<"CLK|1|wire|Clock wire"<<"RST|1|wire|Reset wire");
                e->setState(Edge::OK);
                if(parent)parent->change(true);
                return true;
	}
    }
    else
    if(devices.contains(index) &&
        node->nodes().contains(relationWith))
    {
        INode* source = devices[index];
        INode* dest = node->nodes()[relationWith];
        if(dest && source )
        {
                Edge* e = new Edge(this, source, dest);
                e->setState(Edge::WARNING);
                INode*parent = dest->getParent();
                while(parent->getParent())
                {
                    bool present = false;
                    foreach(Edge*e, parent->edgesIn())
                        if((e->sourceNode() == source)&&(e->destNode() == parent))
                        {
                            present = true;
                            break;
                        }
                    if(!present)//exists
                    {
                        Edge* e = new Edge(this, source, parent);
                        e->setState(Edge::WARNING);
                        e->Hide();
                    }
                    parent = parent->getParent();
                };
                if(this->parent)this->parent->change(true);
                return true;
        }
    }
    else
    if(devices.contains(relationWith) &&
        node->nodes().contains(index))
    {
        INode* dest = devices[relationWith];
        INode* source = node->nodes()[index];
        if(dest && source )
        {
                Edge* e = new Edge(this, source, dest);
                e->setState(Edge::WARNING);
                INode*parent = source->getParent();
                while(parent->getParent())
                {
                    bool present = false;
                    foreach(Edge*e, parent->edgesOut())
                        if((e->sourceNode() == parent)&&(e->destNode() == source))
                        {
                            present = true;
                            break;
                        }
                    if(!present)//exists
                    {
                        Edge* e = new Edge(this, parent, dest);
                        e->setState(Edge::WARNING);
                        e->Hide();
                    }
                    parent = parent->getParent();
                };
                if(this->parent)this->parent->change(true);
                return true;
        }
    }
    return false;
}
/*!\func
 *
 * \param
 * - parent -
 * - name -
 * \return
 */
qint16 GraphBody::addTop(TopTypes type)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    QString name("");
    qint16 id = ++max_id;
    quint32 rand1 = QTime::currentTime().msec();
    quint32 rand2 = rand1*rand1%1000;
    qreal w = - mapToScene(x(), y()).x() + mapToScene(width(), y()).x();
    qreal h = - mapToScene(x(), y()).y() + mapToScene(x(), height()).y();
    qreal posx = mapToScene(x(), y()).x() + w/1000*rand1;
    qreal posy = mapToScene(x(), y()).y() + h/1000*rand2;
    name = QString ("TOP_") + QString::number(id);
    if(type == TOP_DEVICE)
        nodesFactory->newDev(id, name, tr("No tool tip now!"), "", QPointF(posx,posy))->Show();
    else
        nodesFactory->newNode(id, node, name, tr("No tool tip now!"), "", QPointF(posx,posy))->Show();
    if(parent)parent->change(true);
    return id;
}
/*!\func
 *
 * \param
 * \return
 */
void GraphBody::on_actionAdd_relation_triggered()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    state = STATE_ADD_RELATION;
    line->show();
}
/*!\func
 *
 * \param
 * -
 * -
 * \return
 */
void GraphBody::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    // Fill
    return;
    QLinearGradient gradient(rect.topLeft(), rect.bottomRight());
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::lightGray);
    painter->fillRect(rect.intersect(rect), gradient);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(rect);
}
/*!\func
 *
 * \param
 * -
 * \return
 */
void GraphBody::wheelEvent(QWheelEvent *event)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    scaleView(pow((double)2, -event->delta() / 240.0));
}
/*!\func
 *
 * \param
 * -
 * \return
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
 *
 * \param
 * -
 * \return
 */
void GraphBody::setCurIndex(qint16 newInd)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    currentIndex = newInd;
}
/*!\func
 *
 * \param
 * -
 * \return
 */
qint16 GraphBody::getCurIndex( )
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    return currentIndex;
}
/*!\func
 *
 * \param
 * \return
 */
void GraphBody::on_actionLevel_down_triggered()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if(node->nodes().contains(currentIndex))
    {
	clear();
        node = node->nodes()[currentIndex];
        QMapIterator<qint16, INode* > i(node->nodes());
	while(i.hasNext()) {
	    i.next();
            i.value()->Show();
	}
        if(parent)
        {
            parent->setEnableUpLevel(true);
            setLevelsPath();
        }
    }
}
/*!\func
 *
 * \param
 * \return
 */
bool GraphBody::levelUp()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if(!node->getParent())return false;
    clear();
    node = node->getParent();
    QMapIterator<qint16, INode* > i(node->nodes());
    while(i.hasNext()) {
	i.next();
        i.value()->Show();
    }
    setLevelsPath();
    return node->getParent() != NULL;
}
/*!\func
 * sety levels path in status bar
 * \param
 * \return
 */
void GraphBody::setLevelsPath() const
{
    QString path("");
    INode*n = node;
    while(n)
    {
        path = "->" + n->getName() + path;
        n = n->getParent();
    }
    if(parent)
        parent->setLevelPath(path);
}
/*!\func
 *
 * \param
 * \return
 */
void GraphBody::on_actionState_machine_triggered()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if(currentIndex >= 0)
    {
	static QProcess proc;
	extern QString program_dir;
        QString dirname(QDir::tempPath()), filename (dirname + "/tmp.xml"), oldfile("");
	QFile f(filename);
	QDir dir;
	dir.mkpath(dirname);
	f.open(QIODevice::WriteOnly);
	QTextStream text(&f);
	if(node->nodes().contains(currentIndex))
        {
            if(NodeFSM* n = dynamic_cast<NodeFSM *>(node->nodes()[currentIndex]))
            {
                text<<n->getMS();
                oldfile = n->getMSfile();
            }
        }
	f.close();
	dir.setCurrent(program_dir);
        proc.execute(ms_wizard_path + " if=\"" + filename + "\"");
        //QMessageBox::information(0,ms_wizard_path,program_dir + ms_wizard_path);
	f.setFileName(filename);
	f.open(QIODevice::ReadOnly);
	{
            QTextStream text(&f);
            if(f.size()&&node->nodes().contains(currentIndex))
            {
                INode * old = node->nodes()[currentIndex];
                INode*n = nodesFactory->newFSM(old->getId(),node, old->getName(),old->getShortDesc(),
                                                     old->getHelp(),old->pos(), oldfile, text.readAll());
                foreach(Edge *e, old->edgesIn())
                {
                    e->setDestNode(n);
                    n->addEdge(e);
                }
                foreach(Edge *e, old->edgesOut())
                {
                    e->setSourceNode(n);
                    n->addEdge(e);
                }
                n->Show();
                if(parent)parent->change(true);
            }
            f.close();
	}
	QFile::remove(filename);
    }
}
/*!\func
 *
 * \param
 * \return
 */
INode* GraphBody::getParentNode()
{
	LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
	while(node->getParent())
	levelUp();
	return node;
}
/*!\func
 *     \
 * \param
 * \return
 */
void GraphBody::changeEdge(Edge* e)
{
	LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
	int w = 450;
	int h = 300;
	edgeWindow->setGeometry(width()/2 - w/2, height()/2 -h/2, w, h);
	edgeWindow->showModal(e);
	if(parent)parent->change(true);
}
/*!\func
 *
 * \param
 * \return
 */
void GraphBody::on_actionHelp_triggered()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");

    if(node->nodes().contains(currentIndex))
	   if(INode*n = node->nodes()[currentIndex])
		{
           if(dlg)
               delete dlg;
            dlg = new TDialogAddItem(n, project_path);
			dlg->show();
		}
    if(devices.contains(currentIndex))
	   if(INode*n = devices[currentIndex])
		{
           if(dlg)
               delete dlg;
            dlg = new TDialogAddItem(n, project_path);
			dlg->show();
		}
	if(parent)parent->change(true);
}
/*!\func
 * add item to scene
 * \param
 * \return
 */
void GraphBody::addItem(QGraphicsItem* n)
{
    scene.addItem(n);
}
/*!\func
 * save current project
 * \param no
 * \return no
 */
void GraphBody::save()
{
    QDir dir;
    dir.mkpath(project_path);
    Project project(this, nodesFactory, rootNode, project_path + "/", FileNames::project_main_file);
    project.save();
    if(parent)parent->change(false);
    tmpProject = false;
}
/*!\func
 * are current project in temp directory (no saved)?
 * \param no
 * \return no
 */
bool GraphBody::isTmpProject() const
{
    return tmpProject;
}
//! get proj name
QString GraphBody::getProjectName()
{
	return project_path;
}
/*!\func
 * remove state
 * \param no
 * \return no
 */
void GraphBody::on_actionRemove_state_triggered()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if(node->nodes().contains(currentIndex))
    {
        qint32 id = currentIndex;
        currentIndex = -1;
        node->removeNode(id);
        if(parent)parent->change(true);
    }
    else if(devices.contains(currentIndex))
    {
        qint32 id = currentIndex;
        currentIndex = -1;
        devices[id]->remove();
        devices.remove(id);
        if(parent)parent->change(true);
    }
}
/*!\func
 * add device to scene
 * \params
 * - dev - new device
 * \return no
 */
void GraphBody::addDevice(INode*dev)
{
    devices.insert(dev->getId(), dev);
    dev->Show();
    if(parent)parent->change(true);
}
/*!\func
 * get all devices
 * \params no
 * \return no
 */
QList<INode*> GraphBody::getDevices() const
{
    return this->devices.values();
}
