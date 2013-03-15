#ifndef GRAPHBODY_H
#define GRAPHBODY_H

#include <QtGui/QWidget>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QMenu>
#include "data.h"

class Node;
class Edge;
namespace Ui {
    class GraphBody;
}
enum States {
    STATE_IDLE = 0,
    STATE_ADD_RELATION,
};
//виджет для отображения графа
class GraphBody : public QGraphicsView {
    Q_OBJECT
public:
    explicit GraphBody(QWidget *parent = 0);
    virtual ~GraphBody();
    void setData(Data*data);
    void clear();
    void ppMenu();
    void EdgeMenu();
    void setCurIndex(qint16 newInd);
    void setCurrentEdge(qint16 newInd);
    void moveNode(const qint16 id);
    void changeNode(const qint16 id);
    void changeEdge(const qint32 id);
    void removeItem(QGraphicsItem* i);
    void addItem(QGraphicsItem* i);
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void drawBackground(QPainter *painter, const QRectF &rect);
    void wheelEvent(QWheelEvent *event);
    void scaleView(qreal scaleFactor);
private:
    Ui::GraphBody *m_ui;
    QMenu ppTopMenu;
    QMenu menuEdge;
    QGraphicsScene scene;
    qint16 currentIndex, currentEdge;
    QMap<qint16, Node* > shapesList;
    QMap<qint16, Edge* > edgesList;
    quint8 state;
    qreal factor;
    QGraphicsLineItem *line;
    Data*data;
private slots:
    void on_actionRemove_state_triggered();
    void on_actionEdit_state_triggered();
    void on_actionEdit_edge_triggered();
    void on_actionRemove_relation_triggered();
    void on_actionDelete_state_triggered();
    void on_actionAdd_relation_triggered();
    void onAddState(const qint32 id);
    void onEditState(const qint32 id);
    void onRemoveState(const qint32 id);
    void onAddRelation(const qint32 id);
    void onEditRelation(const qint32 id);
    void onRemoveRelation(const qint32 id);
};

#endif // GRAPHBODY_H
