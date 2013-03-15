#ifndef ControlModel_H
#define ControlModel_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include "data.h"
#include "statemodel.h"
#include <QItemDelegate>
#include <QApplication>
#include <QVariant>

class ControlModel : public QAbstractItemModel
{
	Q_OBJECT
public:
    ControlModel(Data*data, const QStringList &headers);
    ~ControlModel();
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
    bool removeRow ( int row, const QModelIndex & parent = QModelIndex() );
private:
    QStringList mimeTypes() const;
    QStringList headers;
    QList<Statestruct> ids;
    Data* shared_data;
    void setupModelData(const QStringList &lines);
public slots:
    void onAddState(const qint32 id);
    void onAddControl(const qint32 state_id, const qint32 id);
    void onEditState(const qint32 id);
    void onRemoveState(const qint32 id);
    void onEditOutput(const qint32 id);
    void onEditControl(const qint32 id);
    void onRemoveControl(const qint32 id);
};

#endif
