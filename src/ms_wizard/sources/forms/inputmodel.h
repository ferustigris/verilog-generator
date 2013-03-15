#ifndef InputModel_H
#define InputModel_H

#include <QAbstractTableModel>
#include <QStringList>


class Data;
class InputModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    InputModel(Data*data, const QStringList&headers);
    virtual int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
    virtual int columnCount ( const QModelIndex & parent = QModelIndex() ) const;
    virtual QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    virtual bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );
    Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    bool removeRow ( int row, const QModelIndex & parent = QModelIndex() );
private:
    QStringList headers;
    Data*shared_data;
    QList<qint32>ids;
public slots:
    void onAddInput(const qint32 id);
    void onRemoveInput(const qint32 id);
};


#endif // InputModel_H
