#ifndef XmlProject_H
#define XmlProject_H
#include <QDomNode>
#include <QFile>
#include "data.h"

class XmlProject
{
public:
    XmlProject(Data*data, const QString &tmpfile);
    ~XmlProject();
    void save();
    void load();
private:
    QString fn;
    QString content;
    QDomDocument doc;
    Data*data;
    QMap<qint32, qint32 > id_states;
    QMap<qint32, qint32 > id_outputs;
protected:
    void loadStates();
    void loadInputs ();
    void loadOutputs ();
    void loadInfo ();
    void save_states(QDomElement& parent);
    void save_relations(QDomElement& parent, const State&state);
    void save_controls(QDomElement& parent, const State&state);
    void save_inputs(QDomElement& parent);
    void save_outputs(QDomElement& parent);
    void backup();
};

#endif // XmlProject_H
