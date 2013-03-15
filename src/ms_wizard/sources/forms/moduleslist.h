#ifndef MODULESLIST_H
#define MODULESLIST_H

#include <QListWidget>
#include <QSplitter>

class ModulesList : public QListWidget
{
public:
    ModulesList(QSplitter*&);
    virtual QMimeData * mimeData (const QList<QListWidgetItem *> items) const;
    QStringList mimeTypes () const;
};

#endif // MODULESLIST_H
