#ifndef TINPUTMODEL_H
#define TINPUTMODEL_H

#include <QAbstractTableModel>
#include <QItemDelegate>
#include "identificatoredit.h"

class TItem {
public:
    TItem(const QString& _name, const quint8 _begin, const quint8 _end) {
	    name = _name;
	    begin = _begin;
	    end = _end;
    }
    TItem() {
	    name = "";
	    begin = 0;
	    end = 0;
    }
public:
    QString name;
    QString description;
    QString type;
    quint8 begin;
    quint8 end;
};
class TInputItem : public TItem {
public:
    TInputItem(const QString& _name, const quint8 _begin, const quint8 _end) : TItem(_name, _begin, _end) {}
};
class TInputModel : public QAbstractTableModel
{
public:
    TInputModel();
    virtual int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
    virtual int columnCount ( const QModelIndex & parent = QModelIndex() ) const;
    virtual QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    bool insertRow ( int row, const QModelIndex & parent = QModelIndex() );
    virtual bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );
    Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    const QList<TInputItem>& getList();
    bool removeRows(int position, int rows, const QModelIndex &parent);
    bool present(const QString&name)const;
private:
    int rows;
    int columns;
    QList<TInputItem> inputs;
};
class IDelegate : public QItemDelegate
{
public:
    IDelegate(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // TINPUTMODEL_H
