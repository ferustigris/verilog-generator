#ifndef PROJECT_H
#define PROJECT_H
#include "edge.h"
#include "inode.h"
#include <QDomNode>
#include <QString>
#include <QFile>

class GraphBody;
class NodesFactory;
class Project
{
public:
	Project(GraphBody*gb, NodesFactory *nodesFactory, INode *_node, const QString&_path, const QString&_fn);
	~Project();
	int load();
	void save();
	static QString loadInputs (const QString&content, const QString& field = "input");
	static QString loadOutputs (const QString&content);
	static QString loadInfo (const QString&content);
private:
	INode *node;
	QDomDocument doc;
	QString fileName;
	QString path;
	QFile file;
	QString content;
	NodesFactory *nodesFactory;
	GraphBody*gb;
protected:
	void save_level(QDomElement& parent, INode* node);
	void save_devices(QDomElement& parent);
	void save_edge(QDomElement& parent, INode* node);
	void save_help(INode* node);
	void create_new();
	void backup();
	int load_level(GraphBody*gb, QDomNode& parent, INode* node);
	int load_edges(GraphBody*gb, QDomNode& parent);
	int load_edge(GraphBody*gb, QDomNode& parent);
private:
	INode* findNode(const qint32 id, INode* parent);
};

#endif // PROJECT_H
