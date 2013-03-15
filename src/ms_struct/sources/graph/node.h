#ifndef NODE_H
#define NODE_H

#include <QList>
#include "inode.h"

class Edge;
class GraphBody;

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

class Node : public INode
{
public:
	Node(GraphBody *graphWidget, INode* _parent, qint16 _id);
	~Node();
	virtual TopTypes getType() const;
	virtual void addEdge(Edge *edge);
	virtual void delEdge(Edge *edge);
	virtual QList<Edge *> edgesIn() const;
	virtual QList<Edge *> edgesOut() const;
	virtual QMap<qint16, INode* > nodes() const;
	virtual QList<INode*> getNodes() const;
	virtual void setName(const QString&);
	virtual QString getName() const;
	virtual INode*getParent();
	virtual void addNode(INode*node);
	virtual void removeNode(const qint32 id);
	virtual qint16 getId();
	virtual QString getHelp() const;
	virtual QStringList getHelpImages() const;
	virtual void setHelp(const QString&, const QStringList&);
	virtual QString getHelpFile() const;
	virtual void setHelpFile(const QString&);
	virtual QString getShortDesc() const;
	virtual void setShortDesc(const QString&);
	virtual void Show();
	virtual void Hide();
protected:
	QRectF boundingRect() const;
	QPainterPath shape() const;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void remove();
private:
	INode* parent;
	QList<Edge *> edgeListIn;
	QList<Edge *> edgeListOut;
	QMap<qint16, INode* > shapesList;
	GraphBody *graph;
	qint16 id;
	QString help, helpFile;//! help content and help file
	QStringList images;//! new images to help files
	QString desc;//! short desc
protected:
	QGraphicsTextItem *name;
};

#endif
