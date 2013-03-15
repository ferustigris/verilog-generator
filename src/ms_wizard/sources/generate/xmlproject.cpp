#include "xmlproject.h"
#include <QMessageBox>
#include <QDomNodeList>
#include <QFile>
#include "errors.h"

//! class, who save project in xml
XmlProject::XmlProject(Data*data, const QString &tmpfile)
{
	fn = tmpfile;
	content = "<?xml version='1.0' encoding='utf-8'?>\n";
	content += "<project>\n";
	content += "\t<states />\n";
	content += "\t<inputs />\n";
	content += "\t<outputs />\n";
	content += "\t<info />\n";
	content += "</project>\n";
	this->data = data;
}
//! destructor
XmlProject::~XmlProject()
{

}
/*!\func
 * save project
 * \param no
 * \return no
 */
void XmlProject::load()
{
    QFile file (fn);
    file.open(QIODevice::ReadOnly);
    doc.setContent(&file);
    loadInputs();
    loadOutputs();
    loadStates();
    loadInfo();
    file.close();

}
/*!\func
 * save project
 * \param no
 * \return no
 */
void XmlProject::save()
{
	backup();
	doc.clear();
	doc.setContent(content);
	QDomNodeList lst = doc.elementsByTagName("states");
	if(lst.isEmpty())return;
	QDomElement e = lst.at(0).toElement();
	e.setNodeValue("");
	save_states(e);
	lst = doc.elementsByTagName("inputs");
	if(lst.isEmpty())return;
	e = lst.at(0).toElement();
	e.setNodeValue("");
	save_inputs(e);
	lst = doc.elementsByTagName("outputs");
	if(lst.isEmpty())return;
	e = lst.at(0).toElement();
	e.setNodeValue("");
	save_outputs(e);

	lst = doc.elementsByTagName("info");
	if(lst.isEmpty())return;
	e = lst.at(0).toElement();
	e.setNodeValue("");
	QDomElement element = doc.createElement("module");
	element.setAttribute("name", data->getModuleName());
	element.setAttribute("description", data->getModuleDescription());
	e.appendChild(element);
	QFile file (fn);
	file.open(QIODevice::WriteOnly);
	QTextStream tf(&file);
	doc.save(tf, 4);
	doc.setContent(&file);
	file.close();
}
/*!\func
 * save one level (recurent)
 * \param no
 * \return no
 */
void XmlProject::save_states(QDomElement& parent)
{
	foreach(State state, data->getStates())
	{
	    QDomElement e = doc.createElement("state");
	    e.setAttribute("name", state.getName());
	    e.setAttribute("description", state.getDescription());
	    e.setAttribute("id", state.getId());
	    e.setAttribute("x", QString::number(state.getPosition().x()));
	    e.setAttribute("y", QString::number(state.getPosition().y()));
	    parent.appendChild(e);
	    save_relations (e, state);
	    save_controls(e, state);
	}
}
/*!\func
 * save one relation (recurent)
 * \param no
 * \return no
 */
void XmlProject::save_relations(QDomElement& parent, const State&state)
{
    if(data)
    foreach(Relation rel, data->getRelations(state.getId()))
    {
	QDomElement e = doc.createElement("relation");
	e.setAttribute("condition", rel.getCondition());
	e.setAttribute("next_state_id", QString::number(rel.getDest().getId()));
	parent.appendChild(e);
    }
}
/*!\func
 * save one relation (recurent)
 * \param no
 * \return no
 */
void XmlProject::save_controls(QDomElement& parent, const State&state)
{
    if(data)
    foreach(Control ctr, data->getControls(state.getId()))
    {
	QDomElement e = doc.createElement("control");
	e.setAttribute("condition", ctr.getCondition());
	e.setAttribute("output", QString::number(ctr.getOutput().getId()));
	e.setAttribute("state", QString::number(ctr.getState().getId()));
	e.setAttribute("value", ctr.getValue());
	parent.appendChild(e);
    }
}
/*!\func
 * create backup
 * \param no
 * \return no
 */
void XmlProject::backup()
{
    //file.remove(fileName + ".backup");
    //file.copy(fileName + ".backup");
}
/*!\func
 * save one relation (recurent)
 * \param no
 * \return no
 */
void XmlProject::save_inputs(QDomElement& parent)
{
    if(data)
	foreach(Input item, data->getInputs())
	{
		QDomElement e = doc.createElement("input");
		e.setAttribute("size", item.getSize());
		e.setAttribute("name", item.getName());
		e.setAttribute("type", item.getType());
		parent.appendChild(e);
	}
}
/*!\func
 * save one relation (recurent)
 * \param no
 * \return no
 */
void XmlProject::save_outputs(QDomElement& parent)
{
    if(data)
	foreach(Output item, data->getOutputs())
	{
		QDomElement e = doc.createElement("output");
		e.setAttribute("id", QString::number(item.getId()));
		e.setAttribute("size", item.getSize());
		e.setAttribute("name", item.getName());
		e.setAttribute("type", item.getType());
		e.setAttribute("value", item.getValue());
		parent.appendChild(e);
	}
}
//! get all states from project
/*
 * statename
 * relation next    condition
 */
void XmlProject::loadStates ()
{
    id_states.clear();
    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();
    do {
	QDomElement e = n.toElement();
	if (e.tagName() == "states")break;
	n = n.nextSibling();
    } while (!n.isNull());
    QDomNode node = n.firstChild();
    while(!node.isNull())
    {
	QDomElement e = node.toElement();
	if(e.tagName() =="state")
	{
	    qint32 oldid = e.attribute("id").toInt();
	    qint32 id = data->addState(e.attribute("name"));
	    data->setStateDescription(id, e.attribute("description"));
	    qint32 x = e.attribute("x").toInt(), y = e.attribute("y").toInt();
	    data->setStatePosition(id, QPoint(x,y));
	    id_states.insert(oldid, id);
	}
	node = node.nextSibling();
    }
    node = n.firstChild();
    while(!node.isNull())
    {
	QDomElement e = node.toElement();
	if(e.tagName() =="state")
	{
		qint32 oldid = e.attribute("id").toInt();
		QDomNode n = node.firstChild();
		while(!n.isNull())
		{
			QDomElement relation = n.toElement();
			if ( relation.tagName() =="relation")
			{
			    qint32 next_id = relation.attribute("next_state_id").toInt();
			    data->addRelation(id_states[oldid], id_states[next_id], relation.attribute("condition"));
			} else
			if ( relation.tagName() =="control")
			{
			    qint32 output_id = relation.attribute("output").toInt();
			    data->addControl(id_states[oldid], id_outputs[output_id], relation.attribute("condition"), relation.attribute("value"));
			}
			n = n.nextSibling();
		}
	}
	node = node.nextSibling();
    }
}
//! get all inputs from project
void XmlProject::loadInputs ()
{
    if(!data)return;
    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();
    do {
	    QDomElement e = n.toElement();
	    if (e.tagName() == "inputs")break;
	    n = n.nextSibling();
    } while (!n.isNull());
    QDomNode node = n.firstChild();
    while(!node.isNull())
    {
	    QDomElement e = node.toElement();
	    if(e.tagName() == "input")
		 data->addInput(e.attribute("name"), e.attribute("size").toInt());
	    node = node.nextSibling();
    }
}
//! get all outputs from project
void XmlProject::loadOutputs ()
{
    if(!data)return;
    id_outputs.clear();
    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();
    do {
	    QDomElement e = n.toElement();
	    if (e.tagName() == "outputs")break;
	    n = n.nextSibling();
    } while (!n.isNull());
    QDomNode node = n.firstChild();
    while(!node.isNull())
    {
	    QDomElement e = node.toElement();
	    if(e.tagName() == "output")
	    {
		qint32 oldid = e.attribute("id").toInt();
		qint32 newid = data->addOutput(e.attribute("name"), e.attribute("size").toInt(), e.attribute("type"), e.attribute("value"));
		id_outputs.insert(oldid, newid);
	    }
	    node = node.nextSibling();
    }
}
//! get info project
void XmlProject::loadInfo ()
{
	if(!data)return;
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
		if(e.tagName() =="module")
		{
			data->setModuleName(e.attribute("name"));
			data->setModuleDescription(e.attribute("description"));
			return;
		}
		node = node.nextSibling();
	}
}
