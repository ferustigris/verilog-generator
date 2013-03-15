#ifndef GRAPHBODY_H
#define GRAPHBODY_H

#include <QtGui/QWidget>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QMenu>
#include <QSettings>
#include "inode.h"
#include "project.h"
#include "edge.h"
#include "tdialogadditem.h"

class Node;
class NodesFactory;
class EnterInputs;
class EdgeParams;
namespace Ui {
    class GraphBody;
}
enum States {
    STATE_IDLE = 0,
    STATE_ADD_RELATION,
    STATE_LEVEL_DOWN,
};
//виджет для отображения графа
class GraphBody : public QGraphicsView {
    Q_OBJECT
public:
    explicit GraphBody(EnterInputs *parent, const QString& proj_name, const QString& wizard_name);
    virtual ~GraphBody();
    qint16 addTop(TopTypes type);
    bool addRelation(const qint16& index,const qint16& relationWith);
    QString getProjectName();
    void setProjectName(const QString&path);
    bool isTmpProject() const;
    void clear();
    void save();
    void ppMenu();
    void setCurIndex(qint16 newInd);
    qint16 getCurIndex();
    bool levelUp();
    INode* getParentNode();
    void changeEdge(Edge* e);
    void removeItem(QGraphicsItem* e);
    void addItem(QGraphicsItem* item);
    void addDevice(INode*dev);
    QList<INode*> getDevices() const;
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void drawBackground(QPainter *painter, const QRectF &rect);
    void wheelEvent(QWheelEvent *event);
    void scaleView(qreal scaleFactor);
    void setLevelsPath() const;
private:
    EnterInputs *parent;
    Ui::GraphBody *m_ui;
    QMenu ppTopMenu;
    QGraphicsScene scene;
    qint16 currentIndex;
    qint16 max_id;
    INode* rootNode;
    INode* node;
    quint8 state;
    QGraphicsLineItem* line;
    qreal factor;
    QString ms_wizard_path;
    QString project_path;
    QMap<qint16, INode* > devices;
    NodesFactory *nodesFactory;
    bool tmpProject;
    EdgeParams *edgeWindow;
    TDialogAddItem *dlg;
    QStringList mustBeRemoved;
private slots:
    void on_actionRemove_state_triggered();
    void on_actionState_machine_triggered();
    void on_actionAdd_relation_triggered();
    void on_actionLevel_down_triggered();
public slots:
    void on_actionHelp_triggered();
};

#endif // GRAPHBODY_H
