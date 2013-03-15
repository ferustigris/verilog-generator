#ifndef IncludeModel_H
#define IncludeModel_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QItemDelegate>
#include <QApplication>
#include <QFileDialog>
#include <QVariant>

class IncludeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    IncludeModel(const QStringList &headers, const QString &data,
	      QObject *parent = 0);
    ~IncludeModel();
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
			int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
		      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value,
		 int role = Qt::EditRole);
    bool setHeaderData(int section, Qt::Orientation orientation,
		       const QVariant &value, int role = Qt::EditRole);
    bool insertColumns(int position, int columns,
		       const QModelIndex &parent = QModelIndex());
    bool removeColumns(int position, int columns,
		       const QModelIndex &parent = QModelIndex());
    bool insertRows(int position, int rows,
		    const QModelIndex &parent = QModelIndex());
    bool removeRows(int position, int rows,
		    const QModelIndex &parent = QModelIndex());
    TreeItem *getItem(const QModelIndex &index) const;
private:
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
    QStringList mimeTypes() const;
    void setupModelData(const QStringList &lines, TreeItem *parent);
    TreeItem *rootItem;
};
class MachinesIncludeDelegate : public QItemDelegate
{
public:
    MachinesIncludeDelegate(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
			   const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
		       const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor,
	 const QStyleOptionViewItem &option, const QModelIndex &index) const;
};
#endif
