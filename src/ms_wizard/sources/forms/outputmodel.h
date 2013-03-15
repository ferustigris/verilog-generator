#ifndef OutputModel_H
#define OutputModel_H

#include "inputmodel.h"
#include <QAbstractTableModel>

class Data;
class OutputModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    OutputModel(Data*data, const QStringList&headers);
    virtual int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
    virtual int columnCount ( const QModelIndex & parent = QModelIndex() ) const;
    virtual QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    virtual bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );
    Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    virtual QMimeData * mimeData ( const QModelIndexList & indexes ) const;
    bool removeRow ( int row, const QModelIndex & parent = QModelIndex() );
private:
    QStringList headers;
    QList<qint32>ids;
    Data* shared_data;
public slots:
    void onAddOutput(const qint32 id);
    void onRemoveOutput(const qint32 id);
};

#endif // OutputModel_H
