#ifndef StateModel_H
#define StateModel_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QItemDelegate>
#include <QVariant>
#include "data.h"
class Statestruct
{
public:
    Statestruct(const qint32 id);
    qint32 id;
    QList<qint32> relations;
};

class StateModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    StateModel(Data*data_shared, const QStringList &headers, QObject *parent = 0);
    ~StateModel();
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    virtual bool removeRow ( int row, const QModelIndex & parent = QModelIndex() );
    qint32 getStateIdByIndex(const QModelIndex & index);
private:
    Data* shared_data;
    QList<Statestruct> ids;
    QStringList headers;
public slots:
    void onAddState(const qint32 id);
    void onEditState(const qint32 id);
    void onRemoveState(const qint32 id);
    void onEditRelation(const qint32 id);
    void onAddRelation(const qint32 id);
    void onRemoveRelation(const qint32 id);
};

#endif
