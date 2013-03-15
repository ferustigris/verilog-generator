#include "project.h"
#include <QTextStream>
#include <QDir>
#include <QMessageBox>
#include "graphbody.h"
#include "nodefsm.h"
#include "node.h"
#include "nodedev.h"
#include "nodesfactory.h"

/*!\func
 * constructor
 * \param no
 * \return no
 */
Project::Project(GraphBody*gb, NodesFactory *nodesFactory, INode *_node, const QString&_path, const QString&_fn) : file(_path + _fn)
{
	node = _node;
        this->gb = gb;
        this->nodesFactory = nodesFactory;
	fileName = _path + _fn;
	path = _path;
	content = "<?xml version='1.0' encoding='utf-8'?>\n";
	content += "<project>\n";
	content += "\t<tops />\n";
	content += "\t<edges />\n";
	content += "</project>\n";
	create_new();
	if(file.open(QIODevice::ReadWrite))
		if (doc.setContent(&file))
		{
		}
}
/*!\func
 * destructor
 * \param no
 * \return no
 */
Project::~Project()
{
}
/*!\func
 * load project
 * \param no
 * \return no
 */
int Project::load()
{
	QDomElement docElem = doc.documentElement();
	QDomNode n = docElem.firstChild();
        qint32 id = load_level(gb, n, node);
        load_edges(gb, n);
        foreach(INode*node, gb->getDevices())
            node->Show();
        return id;
}
/*!\func
 * save project
 * \param no
 * \return no
 */
void Project::save()
{
	backup();
	if (!doc.isNull())
		doc.clear();
	file.close();
	file.remove();
	file.open(QIODevice::ReadWrite);
	doc.setContent(content);
	QDomNodeList lst = doc.elementsByTagName("tops");
	if(lst.isEmpty())return;
	QDomElement e = lst.at(0).toElement();
	e.setNodeValue("");
        save_level(e, node);
        save_devices(e);
        /*lst = doc.elementsByTagName("edges");
	if(lst.isEmpty())return;
	e = lst.at(0).toElement();
	e.setNodeValue("");
        save_edge(e, node);*/
	QTextStream tf(&file);
	doc.save(tf, 4);
	doc.setContent(&file);
}
/*!\func
 * save one level (recurent)
 * \param no
 * \return no
 */
void Project::save_level(QDomElement& parent, INode* node)
{
        foreach(INode*item, node->nodes())
	{
		QDomElement e = doc.createElement("top");
                if(NodeFSM* n = dynamic_cast<NodeFSM *>(item))
                {
                        QString fn(n->getMSfile());
                        if(fn.isEmpty())
                            do
                            {
                            fn = path + "ms_" + n->getName() + "_" + QString::number(qrand()) + ".xml";
                            } while(QFile::exists(fn));
			QFile f (fn);
			f.open(QIODevice::WriteOnly);
			QTextStream text (&f);
                        text<<n->getMS();
			f.close();
			e.setAttribute("ms", fn);
		}
                else
                if(item->getType() == TOP_DEVICE)
                {
                    e.setAttribute("device", "1");
                }
		e.setAttribute("id", item->getId());
		e.setAttribute("name", item->getName());
		e.setAttribute("x", item->pos().x());
		e.setAttribute("y", item->pos().y());
		e.setAttribute("short_description", item->getShortDesc());
		save_help(item);
		e.setAttribute("help", item->getHelpFile());
		parent.appendChild(e);
		save_level(e, item);
		save_edge(parent, item);
	}
}
/*!\func
 * save one relation (recurent)
 * \param no
 * \return no
 */
void Project::save_edge(QDomElement& parent, INode* node)
{
	foreach(Edge*item, node->edgesIn())
	{
		QDomElement e = doc.createElement("edge");
		QString data("");
		foreach(QString wire, item->getWires())
                        if(!wire.isEmpty())data += wire + "#";
		e.setAttribute("data", data);
		e.setAttribute("from", item->sourceNode()->getId());
		e.setAttribute("to", item->destNode()->getId());
		parent.appendChild(e);
	}
}
/*!\func
 * save help page
 * \params
 * - node - node
 * \return no
 */
void Project::save_help(INode* node)
{
	if(node)
	{
		if(!node->getHelp().isEmpty())
		{
			QDir dir;
			if(node->getHelpFile().isEmpty())
			{
				QString helpFile(node->getName() + ".html");
				node->setHelpFile(helpFile);
			}
			QString name(node->getHelpFile());
			name.replace(".html","");
			QString helpPath(path + "/help/" + name);
			dir.mkpath(helpPath);
			QFile file(path + "/help/" + node->getHelpFile());
			file.open(QIODevice::WriteOnly);
			QTextStream text(&file);
			text<<node->getHelp();
			file.close();
			foreach(QString str, node->getHelpImages())
			{
				QString imgname = str.section("/", -1);
				file.copy(str, helpPath + "/" + imgname);
			}
		}
	}
}
/*!\func
 * load one level (recurent)
 * \param no
 * \return no
 */
int Project::load_level(GraphBody*gb, QDomNode& parent, INode* pnode)
{
	QDomNode node = parent.firstChild();
	static int max_id = 0;
	while(!node.isNull())
	{
		QDomElement e = node.toElement();
		if((e.tagName() =="top")&&(e.hasAttribute("id"))&&(e.hasAttribute("name")))
		{
			int id = e.attribute("id").toInt();
			if(id > max_id)max_id = id;
			INode *n = 0;
			if (e.hasAttribute("ms"))
			{
				QFile f (e.attribute("ms"));
				f.open(QIODevice::ReadOnly);
				QTextStream text (&f);
				n = nodesFactory->newFSM(id, pnode, e.attribute("name"),e.attribute("short_description"),e.attribute("help"),
										 QPointF(e.attribute("x").toDouble(), e.attribute("y").toDouble()),
										 e.attribute("ms"), text.readAll());
				f.close();
			} else
			if (e.hasAttribute("device"))
			{
				n = nodesFactory->newDev(id, e.attribute("name"),e.attribute("short_description"),e.attribute("help"),
										 QPointF(e.attribute("x").toDouble(), e.attribute("y").toDouble()));
			}
			else
			{
				n = nodesFactory->newNode(id, pnode, e.attribute("name"),e.attribute("short_description"),e.attribute("help"),
										 QPointF(e.attribute("x").toDouble(), e.attribute("y").toDouble()));
			}
			load_level(gb, node, n);
		}
		node = node.nextSibling();
	}
        //load_edge(gb, parent, pnode);
	pnode->Hide();
	return max_id;
}
/*!\func
 * load one level (recurent)
 * \param no
 * \return no
 */
int Project::load_edge(GraphBody*gb, QDomNode& parent)
{
	QDomNode node = parent.firstChild();
	while(!node.isNull())
	{
		QDomElement e = node.toElement();
		if((e.tagName() =="edge")&&(e.hasAttribute("to"))&&(e.hasAttribute("from"))&&(e.hasAttribute("data")))
		{
			int to = e.attribute("to").toInt();
			int from = e.attribute("from").toInt();
			INode *nto = findNode(to, this->node), *nfrom = findNode(from, this->node);
			foreach(INode*item, gb->getDevices())
				if(item->getId() == to)
					nto = item;
				else if(item->getId() == from)
					nfrom = item;
				if((nto)&&(nfrom))
				{
					Edge*n = new Edge(gb, nfrom, nto);
					n->setWires(e.attribute("data").split("#", QString::SkipEmptyParts));
					n->Hide();
				}
		}
		node = node.nextSibling();
	}
	return 0;
}
/*!\func
 * create new file
 * \param no
 * \return no
 */
void Project::create_new()
{
	if(!file.exists())
	{
		file.open(QFile::WriteOnly);
		QTextStream out(&file);
		out<<content;
		file.close();
	}
}
/*!\func
 * create backup
 * \param no
 * \return no
 */
void Project::backup()
{
	file.remove(fileName + ".backup");
	file.copy(fileName + ".backup");
}

//! get all inputs from project
QString Project::loadInputs (const QString&content, const QString& field)
{
	QString result;
	QDomDocument doc;
	if (!doc.setContent(content))return QString();
	QDomElement docElem = doc.documentElement();
	QDomNode n = docElem.firstChild();
	do {
		QDomElement e = n.toElement();
		if (e.tagName() == field + "s")break;
		n = n.nextSibling();
	} while (!n.isNull());
	QDomNode node = n.firstChild();
	while(!node.isNull())
	{
		QDomElement e = node.toElement();
		if(e.tagName() == field)
		{
			result += e.attribute("name") + "\t";
			result += e.attribute("type") + "\t";
			result += e.attribute("size") + "\t";
			result += e.attribute("value") + "\n";
		}
		node = node.nextSibling();
	}
	return result;
}
//! get all outputs from project
QString Project::loadOutputs (const QString&content)
{
	return loadInputs(content, "output");
}
//! get info project
QString Project::loadInfo (const QString&content)
{

	QString result;
	QDomDocument doc;
	if (!doc.setContent(content))return QString();
	QDomElement docElem = doc.documentElement();
	QDomNode n = docElem.firstChild();
	do {
		QDomElement e = n.toElement();
		if (e.tagName() == "info")break;
		n = n.nextSibling();
	} while (!n.isNull());
	QDomNode node = n.firstChild();
	while(!node.isNull())
	{
		QDomElement e = node.toElement();
		if(e.tagName() == "module")
		{
			result += e.attribute("name") + "\n";
			result += e.attribute("description");
			break;
		}
		node = node.nextSibling();
	}
	//load_edge(gb, parent, pnode);
	//pnode->Hide();
	return result;
}
/*!\func
 * save all devices
 * \param no
 * \return no
 */
void Project::save_devices(QDomElement& parent)
{
        foreach(INode*item, gb->getDevices())
        {
                QDomElement e = doc.createElement("top");
                if(item->getType() == TOP_DEVICE)
                {
                    e.setAttribute("device", "1");
                }
                e.setAttribute("id", item->getId());
                e.setAttribute("name", item->getName());
                e.setAttribute("x", item->pos().x());
                e.setAttribute("y", item->pos().y());
                e.setAttribute("short_description", item->getShortDesc());
                e.setAttribute("help", item->getHelp());
                parent.appendChild(e);
                save_edge(parent, item);
        }
}
/*!\func
 * load one level (recurent)
 * \param no
 * \return no
 */
int Project::load_edges(GraphBody*gb, QDomNode& parent)
{
        QDomNode node = parent.firstChild();
        while(!node.isNull())
        {
                QDomElement e = node.toElement();
                if((e.tagName() =="top")&&(e.hasAttribute("id"))&&(e.hasAttribute("name")))
                {
                     load_edges(gb, node);
                }
                node = node.nextSibling();
        }
        load_edge(gb, parent);
        return 0;
}
/*!\func
 * find node with id
 * \params
 * - id - id of serched node
 * \return no
 */
INode* Project::findNode(const qint32 id, INode* parent)
{
	if(!parent)return 0;
	if(parent->getId() == id)return parent;
	foreach(INode*n, parent->nodes())
	{
		if(INode* r = findNode(id, n))return r;
	}
	return 0;
}
